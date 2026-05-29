#include "SpellSelector.h"
#include "../SpellSystem/SpellResolver.h"
#include "../Grammar/IncantationParser.h"
#include "../Settings/ConfigManager.h"

namespace logger = SKSE::log;

SpellSelector* SpellSelector::GetSingleton()
{
    static SpellSelector singleton;
    if (!singleton._rightSlot) {
        singleton._rightSlot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F42);
    }
    if (!singleton._leftSlot) {
        singleton._leftSlot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F43);
    }
    return std::addressof(singleton);
}

void SpellSelector::EquipSpell(std::vector<uint32_t>& sequence) const
{
    auto query = IncantationParser::GetSingleton()->Parse(sequence);
    auto* resolver = SpellResolver::GetSingleton();
    auto* spell = resolver->ResolveSpell(query);
    if (spell) {
        logger::info("Invoked spell: {}", spell->GetName());
        if (ConfigManager::GetSingleton()->shouldDisplayResolvedSpell()) {
            std::string notification = std::string("Invoked spell: ") + spell->GetName();
            RE::DebugNotification(notification.c_str());
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        auto equipManager = RE::ActorEquipManager::GetSingleton();
        if (!equipManager) return;

        equipManager->EquipSpell(player, spell, _leftSlot);
        equipManager->EquipSpell(player, spell, _rightSlot);
    }
    else {
        logger::warn("Failed to invoke a spell");
        if (ConfigManager::GetSingleton()->shouldDisplayAllNotifications()) {
            RE::DebugNotification("Failed to invoke a spell");
        }
    }
}
