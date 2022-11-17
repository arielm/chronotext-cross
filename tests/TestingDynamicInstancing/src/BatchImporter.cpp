#include "BatchImporter.h"

#include "chr/io/GzipBinaryInputStream.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace io;

vector<IndexedVertexBatch<XYZ.N.UV>> BatchImporter::import(const InputSource &inputSource)
{
    model = importModel(inputSource);

    processNode(model.nodes.front(), Matrix());

    vector<IndexedVertexBatch<XYZ.N.UV>> batches;
    for (auto const &it : batchMap)
    {
        auto batch = join(it.second);

        auto material = model.materials[it.first];
        bool transparent = material.opacity < 1;

        batch.setShaderUniform("u_material.diffuse", material.diffuseColor);
        batch.setShaderUniform("u_material.ambient", material.ambientColor);
        batch.setShaderUniform("u_material.specular", material.specularColor);
        batch.setShaderUniform("u_material.shininess", material.shininess);
        batch.setShaderUniform("u_material.opacity", material.opacity);

        if (material.textureFilename.empty())
        {
            batch.setShaderUniform("u_material.has_texture", false);
        }
        else
        {
            batch.setTexture(model.textures[material.textureFilename]);
            batch.setShaderUniform("u_material.has_texture", true);
        }

        if (transparent)
        {
            batches.push_back(batch);
        }
        else
        {
            batches.insert(batches.begin(), batch);
        }
    }

    return batches;
}

void BatchImporter::processNode(const Model::Node &node, const Matrix &parentMatrix)
{
    Matrix transformedMatrix = Matrix(node.matrix) * parentMatrix;

    for (auto index : node.meshIndices)
    {
        processMesh(model.meshes[index], transformedMatrix);
    }

    for (auto index : node.childIndices)
    {
        processNode(model.nodes[index], transformedMatrix);
    }
}

void BatchImporter::processMesh(const Model::Mesh &mesh, Matrix &parentMatrix)
{
    IndexedVertexBatch<XYZ.N.UV> batch(GL_TRIANGLES, mesh.vertexBuffer, mesh.indexBuffer);

    for (auto &vertex : batch.vertices())
    {
        vertex.position = parentMatrix.transformPoint(vertex.position);
        vertex.normal = parentMatrix.transformNormal(vertex.normal);
    }

    batchMap[mesh.materialIndex].push_back(batch);
}

IndexedVertexBatch<XYZ.N.UV> BatchImporter::join(const vector<IndexedVertexBatch<XYZ.N.UV>> &batches)
{
    IndexedVertexBatch<XYZ.N.UV> out;

    for (int i = 0; i < batches.size(); i++)
    {
        for (int j = 0; j < batches[i].vertexCount(); j++)
        {
            out.addVertex(batches[i].vertex(j));
        }

        for (int j = 0; j < batches[i].indexCount(); j++)
        {
            out.addIndex(batches[i].index(j));
        }

        out.incrementIndices(batches[i].vertexCount());
    }

    return out;
}

Model BatchImporter::importModel(const InputSource &inputSource)
{
    Model model;

    fs::path outputDirectory;
    if (inputSource.isFile())
    {
        outputDirectory = inputSource.getFilePath().parent_path();
    }

    GzipBinaryInputStream stream(inputSource);
    if (stream.good())
    {
        if (stream.readString() == "MODEL.2")
        {
            auto materialCount = stream.readVarint32();
            for (auto i = 0; i < materialCount; i++)
            {
                Model::Material material;

                material.diffuseColor = stream.read<glm::vec3>();
                material.ambientColor = stream.read<glm::vec3>();
                material.specularColor = stream.read<glm::vec3>();
                material.shininess = stream.read<float>();
                material.opacity = stream.read<float>();

                if (stream.read<bool>())
                {
                    material.textureFilename = stream.readString();
                    auto textureSize = stream.read<glm::ivec2>();

                    auto it = model.textures.find(material.textureFilename);
                    if (it == model.textures.end())
                    {
                        Texture::ImageRequest request;
                        if (inputSource.isFile())
                        {
                            request = Texture::ImageRequest(InputSource::file(outputDirectory / material.textureFilename));
                        }
                        else
                        {
                            request = Texture::ImageRequest(material.textureFilename);
                        }

                        bool useMipmap = math::isPowerOfTwo(textureSize.x) && math::isPowerOfTwo(textureSize.y);
                        request.setMipmap(useMipmap);

                        request.setAnisotropy(true);
                        request.setWrap(GL_REPEAT, GL_REPEAT);

                        model.textures.emplace(material.textureFilename, request);
                    }
                }

                model.materials.push_back(material);
            }

            auto nodeCount = stream.readVarint32();
            for (auto i = 0; i < nodeCount; i++)
            {
                Model::Node node;

                node.name = stream.readString();

                array<float, 16> values;
                for (int j = 0; j < 16; j++)
                {
                    values[j] = stream.read<float>();
                }
                node.matrix = Matrix(values);

                auto childCount = stream.readVarint32();
                for (auto j = 0; j < childCount; j++)
                {
                    node.childIndices.push_back(stream.readVarint32());
                }

                auto meshCount = stream.readVarint32();
                for (auto j = 0; j < meshCount; j++)
                {
                    node.meshIndices.push_back(stream.readVarint32());
                }

                model.nodes.push_back(node);
            }

            auto meshCount = stream.readVarint32();
            for (auto i = 0; i < meshCount; i++)
            {
                Model::Mesh mesh;

                mesh.name = stream.readString();
                mesh.materialIndex = stream.readVarint32();

                auto vertexCount = stream.readVarint32();
                for (auto j = 0; j < vertexCount; j++)
                {
                    mesh.vertexBuffer.add(stream.read<glm::vec3>(), stream.read<glm::vec3>(), stream.read<glm::vec2>());
                }

                auto indexCount = stream.readVarint32();
                for (auto j = 0; j < indexCount; j++)
                {
                    mesh.indexBuffer.add(stream.readVarint32());
                }

                model.meshes.push_back(mesh);
            }
        }
    }

    return model;
}
