#include "Model.hpp"

namespace Engine {

Model::Model(const std::vector<Mesh> &meshes) : meshes(meshes) {}

Model::~Model() {}

} // namespace Engine
