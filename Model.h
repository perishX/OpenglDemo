#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <QOpenGLFunctions_3_3_Core>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/glm.hpp"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "Mesh.h"
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"

class Model : public QOpenGLFunctions_3_3_Core
{
private:
    std::vector<Texture> textures_loaded;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

    std::string path{};
public:
    std::vector<Mesh> meshes;
    Model(std::string path);
    ~Model();
    void load();
    void Draw(Shader shader, bool isLineMode = false);
};

#endif // MODEL_H
