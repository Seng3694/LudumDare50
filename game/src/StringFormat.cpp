#include "StringFormat.hpp"

void StringFormat::formatSeconds(float s, char *buffer, const size_t bufferSize)
{
    uint32_t milliseconds = s * 100;
    const uint8_t minutes = (uint8_t)(milliseconds / 6000.0f);
    milliseconds %= 6000;
    const uint8_t seconds = (uint8_t)(milliseconds / 100.0f);
    milliseconds %= 100;

    snprintf(
        buffer, bufferSize, "%02d:%02d:%02d", minutes, seconds, milliseconds);
}
