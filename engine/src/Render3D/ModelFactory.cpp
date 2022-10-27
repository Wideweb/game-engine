#include "ModelFactory.hpp"

#include "Material.hpp"
#include "Mesh.hpp"
#include "TBN.hpp"

#include <cmath>
#include <numeric>

namespace Engine {

std::shared_ptr<Model> ModelFactory::createCube(float size) { return createCube(size, size, size, size, size, size); }

std::shared_ptr<Model> ModelFactory::createCube(float left, float right, float bottom, float top, float back,
                                                float front) {
    // clang-format off
    std::vector<glm::vec3> positions = {
        glm::vec3(-left, top, front), // 0
        glm::vec3(-left, -bottom, front), // 1
        glm::vec3(-left, top, -back), // 2
        glm::vec3(-left, -bottom, -back), // 3
        glm::vec3(right, top, front), // 4
        glm::vec3(right, -bottom, front), // 5
        glm::vec3(right, top, -back), // 6
        glm::vec3(right, -bottom, -back), // 7
    };

    std::vector<Mesh::Vertex> vertices;
    vertices.emplace_back(positions[4], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[0], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[2], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[6], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[1], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[0], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[6], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[2], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[6], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[6], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[4], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[1], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[0], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[0], glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    // clang-format on

    Render3D::Utils::tbn(vertices);

    Mesh mesh(vertices);
    auto model = std::shared_ptr<Model>(new Model3D({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<Model3D> ModelFactory::createPlane(float tileSize, int columns, int rows, bool centered) {
    // clang-format off
    glm::vec2 center = glm::vec2(0.0f, 0.0f);

    if (centered) {
        center.x = columns * tileSize / 2.0f;
        center.y = rows * tileSize / 2.0f;
    }

    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= columns; j++) {
            float x = static_cast<float>(j);
            float z = static_cast<float>(i);

            vertices.emplace_back(
                glm::vec3(x * tileSize - center.x, 0.0f, z * tileSize - center.y), 
                glm::vec3(0.0f),
                glm::vec2(x / columns, z / rows),
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(0.8f, 0.6f, 0.1f)
            );

            if (i != rows && j != columns) {
                unsigned int v1 = (i + 1) * (columns + 1) + j;
                unsigned int v2 = i * (columns + 1) + j + 1;
                unsigned int v3 = i * (columns + 1) + j;

                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);

                v1 = (i + 1) * (columns + 1) + j;
                v2 = (i + 1) * (columns + 1) + j + 1;
                v3 = i * (columns + 1) + j + 1;

                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);
            }
        }
    }
    // clang-format on

    Render3D::Utils::tbn(vertices, indices);

    Mesh mesh(vertices, indices);
    auto model = std::shared_ptr<Model3D>(new Model3D({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<Model> ModelFactory::createCircle(float radius, int segments, float lineWidth, glm::vec3 color) {
    float dAngle = 2.0f * 3.1415926f / static_cast<float>(segments);
    float angle = 0.0f;

    std::vector<glm::vec2> corclePoints;
    corclePoints.reserve(segments);
    for (int i = 0; i < segments; i++) {
        corclePoints.emplace_back(std::cosf(angle) * radius, std::sinf(angle) * radius);
        angle += dAngle;
    }

    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < segments + 1; i++) {
        glm::vec2 prev = corclePoints[(i + segments - 1) % segments];
        glm::vec2 point = corclePoints[i % segments];
        glm::vec2 next = corclePoints[(i + 1) % segments];

        glm::vec2 prevNormal = glm::vec2(point.y - prev.y, prev.x - point.x);
        glm::vec2 nextNormal = glm::vec2(next.y - point.y, point.x - next.x);
        glm::vec2 normal = glm::normalize(prevNormal + nextNormal);

        glm::vec2 v0 = point - normal * lineWidth * 0.5f;
        glm::vec2 v1 = point + normal * lineWidth * 0.5f;

        glm::vec2 uv0 = glm::vec2(0.0f, 0.0f);
        glm::vec2 uv1 = glm::vec2(1.0f, 0.0f);

        if (i % 2 == 0) {
            uv0.y = 1.0f;
            uv1.y = 1.0f;
        }

        vertices.emplace_back(glm::vec3(v0, lineWidth * 0.5f), glm::vec3(0.0f), uv0, glm::vec3(0.0f), glm::vec3(0.0f),
                              color);
        vertices.emplace_back(glm::vec3(v1, lineWidth * 0.5f), glm::vec3(0.0f), uv1, glm::vec3(0.0f), glm::vec3(0.0f),
                              color);
        vertices.emplace_back(glm::vec3(v0, lineWidth * -0.5f), glm::vec3(0.0f), uv0, glm::vec3(0.0f), glm::vec3(0.0f),
                              color);
        vertices.emplace_back(glm::vec3(v1, lineWidth * -0.5f), glm::vec3(0.0f), uv1, glm::vec3(0.0f), glm::vec3(0.0f),
                              color);

        // Front
        indices.push_back((i % segments) * 4 + 0);
        indices.push_back((i % segments) * 4 + 5);
        indices.push_back((i % segments) * 4 + 4);

        indices.push_back((i % segments) * 4 + 0);
        indices.push_back((i % segments) * 4 + 1);
        indices.push_back((i % segments) * 4 + 5);

        // Right
        indices.push_back((i % segments) * 4 + 1);
        indices.push_back((i % segments) * 4 + 7);
        indices.push_back((i % segments) * 4 + 5);

        indices.push_back((i % segments) * 4 + 1);
        indices.push_back((i % segments) * 4 + 3);
        indices.push_back((i % segments) * 4 + 7);

        // Left
        indices.push_back((i % segments) * 4 + 2);
        indices.push_back((i % segments) * 4 + 4);
        indices.push_back((i % segments) * 4 + 6);

        indices.push_back((i % segments) * 4 + 2);
        indices.push_back((i % segments) * 4 + 0);
        indices.push_back((i % segments) * 4 + 4);

        // Back
        indices.push_back((i % segments) * 4 + 3);
        indices.push_back((i % segments) * 4 + 6);
        indices.push_back((i % segments) * 4 + 7);

        indices.push_back((i % segments) * 4 + 3);
        indices.push_back((i % segments) * 4 + 2);
        indices.push_back((i % segments) * 4 + 6);
    }

    Render3D::Utils::tbn(vertices, indices);

    Mesh mesh(vertices, indices);
    auto model = std::shared_ptr<Model>(new Model3D({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<Model> ModelFactory::createFrastum(float fieldOfView, float nearPlane, float farPlane,
                                                   glm::vec3 color) {
    float tangent = std::tanf(fieldOfView / 2.0f);

    float fronDelta = nearPlane * tangent;
    float backDelta = farPlane * tangent;

    std::vector<Mesh::Vertex> vertices;

    // Front
    vertices.emplace_back(glm::vec3(-fronDelta, -fronDelta, nearPlane), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(-fronDelta, fronDelta, nearPlane), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(fronDelta, fronDelta, nearPlane), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(fronDelta, -fronDelta, nearPlane), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);

    // Back
    vertices.emplace_back(glm::vec3(-backDelta, -backDelta, farPlane), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(-backDelta, backDelta, farPlane), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(backDelta, backDelta, farPlane), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);
    vertices.emplace_back(glm::vec3(backDelta, -backDelta, farPlane), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f),
                          glm::vec3(0.0f), glm::vec3(0.0f), color);

    std::vector<unsigned int> indices;

    // Front
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(0);

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);

    // Right
    indices.push_back(6);
    indices.push_back(2);
    indices.push_back(3);

    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(6);

    // Left
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(4);

    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(1);

    // Back
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(7);

    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(5);

    // Top
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(1);

    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(6);

    // Bottom
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(4);

    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(3);

    Render3D::Utils::tbn(vertices, indices);

    Mesh mesh(std::move(vertices), indices);
    auto model = std::shared_ptr<Model>(new Model3D({std::move(mesh)}));
    model->setUp();
    return model;
}

} // namespace Engine
