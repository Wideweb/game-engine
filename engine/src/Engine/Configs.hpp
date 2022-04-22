#pragma once

#include <array>
#include <string>

namespace Engine::Configs {

const std::array<std::string, 3> c_TextureExtensions = {"jpg", "jpeg", "png"};
const std::array<std::string, 2> c_SceneObjectExtensions = {"obj", "fbx"};
const std::array<std::string, 2> c_3DModelExtensions = {"obj", "fbx"};
const std::array<std::string, 1> c_ScriptExtensions = {"lua"};
const std::array<std::string, 1> c_FontExtensions = {"ttf"};

const std::string c_EditToolsModelPrefix = "##Edit_Tools##";

}