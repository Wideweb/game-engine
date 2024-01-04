#include "Shaders.hpp"
#include "Texture.hpp"

#include "GLSLPreprocessor.hpp"

namespace Engine {

void Shaders::init() {
    Texture defaultTexture = Texture::createR8Buffer(1, 1);
    Texture defaultCubeMapTexture = CubeMapTexture::createCubeDepthBuffer(1, 1);

    auto vertexMetaData = GLSLPreprocessor::preprocess("./shaders/pass/direct.vertex.glsl");
    auto fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/pass/direct.fragment.glsl");
    defaultShader = std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode);
    defaultShader->setVertexMetaData(std::move(vertexMetaData));
    defaultShader->setFragmentMetaData(std::move(fragmentMetaData));

    vertexMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/scene-tool.vertex.glsl");
    fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/scene-tool.fragment.glsl");
    sceneToolShader = std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode);
    sceneToolShader->setVertexMetaData(std::move(vertexMetaData));
    sceneToolShader->setFragmentMetaData(std::move(fragmentMetaData));

    // size_t uniformsNumber = defaultShader->uniformKeys().size();
    // for (size_t i = 0; i < uniformsNumber; i++) {
    //     if (defaultShader->uniformTypes()[i] == Shader::Property::Type::TEXTURE) {
    //         defaultShader->setTexture(defaultShader->uniformKeys()[i], defaultTexture);
    //     }

    //     if (defaultShader->uniformTypes()[i] == Shader::Property::Type::CUBE_MAP_TEXTURE) {
    //         defaultShader->setTexture(defaultShader->uniformKeys()[i], defaultCubeMapTexture);
    //     }
    // }

    vertexMetaData = GLSLPreprocessor::preprocess("./shaders/overlay-vertex-shader.glsl");
    fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/overlay-fragment-shader.glsl");
    auto geometryMetaData = GLSLPreprocessor::preprocess("./shaders/overlay-geometry-shader.glsl");
    meshShader =
        std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode, geometryMetaData.sourceCode);
    meshShader->setVertexMetaData(std::move(vertexMetaData));
    meshShader->setFragmentMetaData(std::move(fragmentMetaData));

    vertexMetaData = GLSLPreprocessor::preprocess("./shaders/overlay-vertex-shader.glsl");
    fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/overlay-fragment-shader.glsl");
    geometryMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/edge.geometry.glsl");
    cubeEdgesShader =
        std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode, geometryMetaData.sourceCode);
    cubeEdgesShader->setVertexMetaData(std::move(vertexMetaData));
    cubeEdgesShader->setFragmentMetaData(std::move(fragmentMetaData));

    vertexMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/grid.vertex.glsl");
    fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/grid.fragment.glsl");
    gridShader = std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode);
    gridShader->setVertexMetaData(std::move(vertexMetaData));
    gridShader->setFragmentMetaData(std::move(fragmentMetaData));

    vertexMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/brush.vertex.glsl");
    fragmentMetaData = GLSLPreprocessor::preprocess("./shaders/pass/utils/brush.fragment.glsl");
    terrainBrushShader = std::make_shared<Shader>(vertexMetaData.sourceCode, fragmentMetaData.sourceCode);
    terrainBrushShader->setVertexMetaData(std::move(vertexMetaData));
    terrainBrushShader->setFragmentMetaData(std::move(fragmentMetaData));

    m_Map.insert({"default", defaultShader});
    m_Map.insert({"mesh", meshShader});
    m_Map.insert({"cubeEdges", cubeEdgesShader});
    m_Map.insert({"grid", gridShader});
    m_Map.insert({"terrainBrush", terrainBrushShader});
}

void Shaders::load(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
    if (m_Map.find(name) != m_Map.end()) {
        throw std::invalid_argument("shader name is not unique.");
    }
    m_Map.insert({name, std::make_shared<Shader>(vertexSrc, fragmentSrc)});
}

Shader *Shaders::get(const std::string &name) { return m_Map.at(name).get(); }

} // namespace Engine
