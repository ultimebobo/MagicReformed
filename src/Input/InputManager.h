#pragma once

#include <vector>

class InputManager :
    public RE::BSTEventSink<RE::InputEvent*>
{
public:
    static InputManager* GetSingleton();

    virtual RE::BSEventNotifyControl ProcessEvent(
        RE::InputEvent* const* eventPtr,
        RE::BSTEventSource<RE::InputEvent*>* eventSource) override;

private:
    InputManager() = default;

    bool _hasWeaponState = false;
    RE::WEAPON_STATE _previousWeaponState = RE::WEAPON_STATE::kSheathed;

    void HandleWeaponStateChange(RE::WEAPON_STATE weaponState);

    uint32_t _magicModeKey = 0;
    std::vector<uint32_t> _magicKeys;
};
