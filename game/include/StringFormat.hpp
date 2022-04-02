#pragma once

#include <cstdint>
#include <cstdio>

class StringFormat
{
  public:
    static void formatSeconds(float s, char *buffer, const size_t bufferSize);
};