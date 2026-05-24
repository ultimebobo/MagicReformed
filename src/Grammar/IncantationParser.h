#pragma once

#include "GrammarTypes.h"

class IncantationParser
{
public:
    static IncantationParser* GetSingleton();

    SpellQuery Parse(
        const std::vector<uint32_t>& sequence);

private:
    IncantationParser() = default;
};
