#include "EquipmentSnapshot.h"

namespace logger = SKSE::log;

EquipmentSnapshot* EquipmentSnapshot::GetSingleton()
{
    static EquipmentSnapshot singleton;
    return std::addressof(singleton);
}

void EquipmentSnapshot::SaveEquipment()
{
    auto* player = RE::PlayerCharacter::GetSingleton();

    if (!player) {
        return;
    }

    _leftHand = player->GetEquippedObject(true);
    _rightHand = player->GetEquippedObject(false);

    logger::info("Equipment snapshot saved");
}

void EquipmentSnapshot::RestoreHand(
    RE::PlayerCharacter* player,
    RE::ActorEquipManager* equipManager,
    RE::TESForm* savedHand,
    bool leftHand,
    const RE::TESObjectREFR::InventoryCountMap& inventoryCounts)
{
    auto* current = player->GetEquippedObject(leftHand);
    // Only replace hand if a spell is equipped
    if (savedHand && current && current->As<RE::SpellItem>()) {
        int slotId = leftHand ? 0x13F43 : 0x13F42;
        auto slot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(slotId);

        if (auto* spell = savedHand->As<RE::SpellItem>()) {
            if (spell != current->As<RE::SpellItem>()) {
                auto* task = SKSE::GetTaskInterface();
                task->AddTask([equipManager, player, spell, slot]() { equipManager->EquipSpell(player, spell, slot); });
            }
        }
        else if (auto* object = savedHand->As<RE::TESBoundObject>()) {
            auto countIt = inventoryCounts.find(object);
            auto* otherHand = player->GetEquippedObject(!leftHand);
            if (countIt != inventoryCounts.end() && countIt->second > 0 // Item is still in inventory
                && (savedHand != otherHand || countIt->second > 1)) { // Item is not equipped in the other hand, or there are multiple copies
                
                auto* task = SKSE::GetTaskInterface();
                task->AddTask([equipManager, player, object, slot]() { equipManager->EquipObject(player, object, nullptr, 1, slot); });
            }
        }
    }
    savedHand = nullptr;
}

void EquipmentSnapshot::RestoreEquipment()
{
    auto* player = RE::PlayerCharacter::GetSingleton();

    if (!player) {
        return;
    }

    auto* equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) {
        return;
    }

    // Get inventory counts to verify items still exist
    const auto& inventoryCounts = player->GetInventoryCounts();

    RestoreHand(player, equipManager, _leftHand, true, inventoryCounts);
    RestoreHand(player, equipManager, _rightHand, false, inventoryCounts);
}
