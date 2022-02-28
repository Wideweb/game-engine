#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "assimp/color4.h"
#include "assimp/matrix4x4.h"
#include "assimp/quaternion.h"
#include "assimp/vector3.h"

namespace Engine {

class PrimitivesMapper {
  public:
    inline static glm::mat4 map(const aiMatrix4x4 &from) {
        glm::mat4 to;

        to[0][0] = static_cast<float>(from.a1);
        to[0][1] = static_cast<float>(from.b1);
        to[0][2] = static_cast<float>(from.c1);
        to[0][3] = static_cast<float>(from.d1);
        to[1][0] = static_cast<float>(from.a2);
        to[1][1] = static_cast<float>(from.b2);
        to[1][2] = static_cast<float>(from.c2);
        to[1][3] = static_cast<float>(from.d2);
        to[2][0] = static_cast<float>(from.a3);
        to[2][1] = static_cast<float>(from.b3);
        to[2][2] = static_cast<float>(from.c3);
        to[2][3] = static_cast<float>(from.d3);
        to[3][0] = static_cast<float>(from.a4);
        to[3][1] = static_cast<float>(from.b4);
        to[3][2] = static_cast<float>(from.c4);
        to[3][3] = static_cast<float>(from.d4);

        return to;
    }

    inline static glm::vec3 map(const aiVector3D &from) {
        glm::vec3 to;

        to.x = static_cast<float>(from.x);
        to.y = static_cast<float>(from.y);
        to.z = static_cast<float>(from.z);

        return to;
    }

    inline static glm::vec4 map(const aiColor4D &from) {
        glm::vec4 to;

        to.r = static_cast<float>(from.r);
        to.g = static_cast<float>(from.g);
        to.b = static_cast<float>(from.b);
        to.a = static_cast<float>(from.a);

        return to;
    }

    inline static glm::quat map(const aiQuaternion &from) {
        glm::quat to;

        to.x = static_cast<float>(from.x);
        to.y = static_cast<float>(from.y);
        to.z = static_cast<float>(from.z);
        to.w = static_cast<float>(from.w);

        return to;
    }
};

} // namespace Engine