#include "SpellSelector.h"
#include "../SpellSystem/SpellResolver.h"
#include "../Grammar/IncantationParser.h"
#include "../Settings/ConfigManager.h"

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
        logger::info("Invoked spell: {}", spell->GetName());
        if (ConfigManager::GetSingleton()->shouldDisplayResolvedSpell()) {
            std::string notification = std::string("Invoked spell: ") + spell->GetName();
            RE::DebugNotification(notification.c_str());
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        auto rightSlot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F42);
        auto leftSlot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F43);

        auto* task = SKSE::GetTaskInterface();
        auto  equipManager = RE::ActorEquipManager::GetSingleton();
        task->AddTask([equipManager, player, spell, leftSlot]() { equipManager->EquipSpell(player, spell, leftSlot); });
        task->AddTask([equipManager, player, spell, rightSlot]() { equipManager->EquipSpell(player, spell, rightSlot); });
    }
    else {
        logger::warn("Failed to invoke a spell");
        if (ConfigManager::GetSingleton()->shouldDisplayAllNotifications()) {
            RE::DebugNotification("Failed to invoke a spell");
        }
    }
}
