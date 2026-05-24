#include "SpellSelector.h"
#include "../SpellSystem/SpellResolver.h"
#include "../Grammar/IncantationParser.h"

#include <random>

namespace logger = SKSE::log;

SpellSelector* SpellSelector::GetSingleton()
{
    static SpellSelector singleton;
    return std::addressof(singleton);
}

void SpellSelector::EquipSpell(std::vector<uint32_t>& sequence) const
{
    auto query = IncantationParser::GetSingleton()->Parse(sequence);
    auto* resolver = SpellResolver::GetSingleton();
    auto* spell = resolver->ResolveSpell(query);
    if (spell) {
        logger::info("Resolved spell: {}", spell->GetName());
        RE::DebugNotification("Resolved spell: {}", spell->GetName());

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        RE::ActorEquipManager::GetSingleton()->EquipSpell(player, spell, nullptr);
    }
    else {
        logger::warn("Failed to resolve spell");
        RE::DebugNotification("Failed to resolve spell");
    }
}
