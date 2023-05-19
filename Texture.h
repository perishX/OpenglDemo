#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Texture
{
    unsigned int id;
    std::string type;
    aiString path;
};

#endif // TEXTURE_H
