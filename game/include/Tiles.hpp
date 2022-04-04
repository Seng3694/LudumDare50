#pragma once

#include <cstdint>

enum class TileType : uint8_t
{
    LowGrass,
    HighGrass,
    Walkway,
    Boulder,
    Thunder,
    ResourceBarLeftYellow,
    ResourceBarCenterYellow,
    ResourceBarLeftRed,
    ResourceBarCenterRed,
    ResourceBarLeftGreen,
    ResourceBarCenterGreen,
    ResourceBarLeftEmpty,
    ResourceBarCenterEmpty,
    Star,
    StarEmpty,
    Button
};
