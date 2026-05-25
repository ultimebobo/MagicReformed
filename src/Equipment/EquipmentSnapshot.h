#pragma once

class EquipmentSnapshot
{
public:
    static EquipmentSnapshot* GetSingleton();

    void SaveEquipment();
    void RestoreEquipment();

private:
    EquipmentSnapshot() = default;

    void RestoreHand(RE::PlayerCharacter* player,
        RE::ActorEquipManager* equipManager,
        RE::TESForm* savedHand,
        bool leftHand,
        const RE::TESObjectREFR::InventoryCountMap& inventoryCounts);

    RE::TESForm* _leftHand = nullptr;
    RE::TESForm* _rightHand = nullptr;
};
