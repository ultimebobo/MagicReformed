#pragma once

#include "SpellTypes.h"

class SpellResolver
{
public:
    static SpellResolver* GetSingleton();

    RE::SpellItem* ResolveSpell(const SpellQuery& query);
    bool IsValidSpell(RE::SpellItem* spell);

private:
    SpellResolver() = default;

    bool SpellMatchesQuery(RE::SpellItem* spell, const SpellQuery& query);
};
