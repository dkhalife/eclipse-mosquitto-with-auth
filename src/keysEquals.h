#pragma once

#include <string.h>
#include <utility>

struct KeysEqual
{
    bool operator()(const char* lhs, const char* rhs) const
    {
        return strcmp(lhs, rhs);
    }
};
