#include "Model.h"

Model::Model(std::string path)
{
    this->path=path;
}

Model::~Model()
{
}

void Model::load(){
    initializeOpenGLFunctions();
    this->loadModel(this->path);
}

void Model::Draw(Shader shader, bool isLineMode)
{
    for (Mesh mesh : meshes)
    {
        mesh.Draw(shader, isLineMode);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import{};
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('\\'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    initializeOpenGLFunctions();
    // 处理节点所有的网格（如果有的话）
    std::cout<<"allnodes "  <<node->mNumMeshes<<std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        std::cout<<"node "<<i<<std::endl;
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::cout<<"mesh"<<std::endl;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        // 处理顶点位置、法线和纹理坐标
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.Position = position;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoords;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    // 处理材质
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                                aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                 aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> reflectMaps = loadMaterialTextures(material,
                                                                aiTextureType_AMBIENT, "texture_reflect");
        textures.insert(textures.end(), reflectMaps.begin(), reflectMaps.end());
    }

    Mesh res{vertices, indices, textures};
    res.init();
    return res;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                 std::string typeName)
{
    std::cout<<type<<" "<<typeName<<std::endl;
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (Texture t : textures_loaded)
        {
            if (t.path == str)
            {
                textures.push_back(t);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            std::cout << type << " " << str.C_Str() << std::endl;
            Texture texture;
            texture.id = this->TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = directory + "\\" + std::string(path);
    std::cout<<filename<<std::endl;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);

    // std::cout << "load image" << std::endl;
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Success to load texture" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}
