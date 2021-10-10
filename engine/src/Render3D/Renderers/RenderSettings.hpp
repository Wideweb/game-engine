#pragma once

#include <string>
#include <vector>

namespace Engine {

enum class ToneMapping {
    LinearToneMapping = 0,
    SimpleReinhardToneMapping = 1,
    LumaBasedReinhardToneMapping = 2,
    WhitePreservingLumaBasedReinhardToneMapping = 3,
    FilmicToneMapping = 4,
    RomBinDaHouseToneMapping = 5,
    Uncharted2ToneMapping = 6
};

const std::vector<ToneMapping> c_ToneMappings = {
    ToneMapping::LinearToneMapping,
    ToneMapping::SimpleReinhardToneMapping,
    ToneMapping::LumaBasedReinhardToneMapping,
    ToneMapping::WhitePreservingLumaBasedReinhardToneMapping,
    ToneMapping::FilmicToneMapping,
    ToneMapping::RomBinDaHouseToneMapping,
    ToneMapping::Uncharted2ToneMapping,
};

static std::string getToneMappingName(ToneMapping tonMapping) {
    switch (tonMapping) {
    case ToneMapping::LinearToneMapping:
        return "Linear";
    case ToneMapping::SimpleReinhardToneMapping:
        return "Simple Reinhard";
    case ToneMapping::LumaBasedReinhardToneMapping:
        return "Luma Based Reinhard";
    case ToneMapping::WhitePreservingLumaBasedReinhardToneMapping:
        return "White Preserving Luma Based Reinhard";
    case ToneMapping::FilmicToneMapping:
        return "Filmic";
    case ToneMapping::RomBinDaHouseToneMapping:
        return "Rom Bin Da House";
    case ToneMapping::Uncharted2ToneMapping:
        return "Uncharted 2";
    default:
        return "Unknown";
    }
}

struct RenderSettings {
    bool hdr = true;
    bool bloom = true;
    ToneMapping toneMapping = ToneMapping::LinearToneMapping;
    float exposure = 1.0f;
    float gamma = 2.2f;
    float threshold = 1.0f;
    unsigned int bloomScale = 4;
    unsigned int blur = 4;
};

} // namespace Engine