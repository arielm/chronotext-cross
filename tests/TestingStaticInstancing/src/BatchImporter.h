#pragma once

#include "chr/gl/Batch.h"

struct Model
{
    struct Node
    {
        std::string name;

        chr::gl::Matrix matrix;

        std::vector<int> childIndices;
        std::vector<int> meshIndices;
    };

    struct Mesh
    {
        std::string name;

        chr::gl::Buffer<chr::gl::Vertex<chr::gl::XYZ.N.UV>> vertexBuffer;
        chr::gl::Buffer<GLuint> indexBuffer;

        int materialIndex;
    };

    struct Material
    {
        glm::vec3 diffuseColor;
        glm::vec3 ambientColor;
        glm::vec3 specularColor;
        float shininess;
        float opacity;

        std::string textureFilename;
    };

    std::vector<Node> nodes;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::map<std::string, chr::gl::Texture> textures;
};

class BatchImporter
{
public:
    std::vector<chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV>> import(const chr::InputSource &inputSource);

protected:
    Model model;
    std::map<int, std::vector<chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV>>> batchMap;

    void processNode(const Model::Node &node, const chr::gl::Matrix &parentMatrix);
    void processMesh(const Model::Mesh &mesh, chr::gl::Matrix &parentMatrix);
    static chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> join(const std::vector<chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV>> &batches);

    static Model importModel(const chr::InputSource &inputSource);
};
