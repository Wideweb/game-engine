#pragma once

#include <bitset>
#include <stdint.h>

namespace Engine {

using Entity = uint32_t;

constexpr Entity c_MaxEntities = 500;

using ComponentType = std::uint8_t;

const ComponentType c_MaxComponents = 32;

using Signature = std::bitset<c_MaxComponents>;

} // namespace Engine