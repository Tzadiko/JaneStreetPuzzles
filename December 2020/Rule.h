#pragma once
#include <vector>

#include "Direction.h"

struct Rule
{
    size_t val_;
    View view_;
    size_t viewNumber_; // Row or column number.
    Direction direction_;
};

using Rules = std::vector<Rule>;