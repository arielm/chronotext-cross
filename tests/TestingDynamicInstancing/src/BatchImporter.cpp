#include "BatchImporter.h"

#include "chr/io/GzipBinaryInputStream.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace io;

vector<IndexedVertexBatch<XYZ.N.UV>> BatchImporter::read(const InputSource &inputSource)
{
    vector<IndexedVertexBatch<XYZ.N.UV>> batches;
    map<string, Texture> textures;

    fs::path outputDirectory;
    if (inputSource.isFile())
    {
        outputDirectory = inputSource.getFilePath().parent_path();
    }

    GzipBinaryInputStream stream(inputSource);
    if (stream.good())
    {
        if (stream.readString() == "XYZ.N.UV")
        {
            auto batchCount = stream.readVarint32();
            for (auto i = 0; i < batchCount; i++)
            {
                stream.readString();

                IndexedVertexBatch<XYZ.N.UV> batch;

                batch.setShaderUniform("u_diffuse_color", stream.read<glm::vec3>());
                batch.setShaderUniform("u_ambient_color", stream.read<glm::vec3>());
                batch.setShaderUniform("u_specular_color", stream.read<glm::vec3>());
                batch.setShaderUniform("u_shininess", stream.read<float>());

                if (stream.read<bool>())
                {
                    auto textureFilename = stream.readString();
                    auto textureSize = stream.read<glm::ivec2>();

                    auto it = textures.find(textureFilename);
                    if (it == textures.end())
                    {
                        Texture::ImageRequest request;
                        if (inputSource.isFile())
                        {
                            request = Texture::ImageRequest(InputSource::file(outputDirectory / textureFilename));
                        }
                        else
                        {
                            request = Texture::ImageRequest(textureFilename);
                        }

                        bool useMipmap = math::isPowerOfTwo(textureSize.x) && math::isPowerOfTwo(textureSize.y);
                        request.setMipmap(useMipmap);

                        auto texture = Texture(request);
                        batch.setTexture(texture);

                        textures.emplace(textureFilename, texture);
                    }
                    else
                    {
                        batch.setTexture(it->second);
                    }

                    batch.setShaderUniform("u_has_texture", true);
                }

                auto vertexCount = stream.readVarint32();
                for (auto j = 0; j < vertexCount; j++)
                {
                    batch.addVertex(stream.read<glm::vec3>(), stream.read<glm::vec3>(), stream.read<glm::vec2>());
                }

                auto indexCount = stream.readVarint32();
                for (auto j = 0; j < indexCount; j++)
                {
                    batch.addIndex(stream.readVarint32());
                }

                batches.push_back(batch);
            }
        }
    }

    return batches;
}
