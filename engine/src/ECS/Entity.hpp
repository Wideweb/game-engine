#pragma once

#include <bitset>
#include <stdint.h>

namespace Engine {

using Entity = uint32_t;

constexpr Entity c_MaxEntities = 64100;

constexpr Entity c_NoEntity = 0;

using ComponentType = std::uint8_t;

const ComponentType c_MaxComponents = 32;

using Signature = std::bitset<c_MaxComponents>;

} // namespace Engine