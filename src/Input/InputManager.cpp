#include "InputManager.h"

#include "../MagicMode/MagicModeManager.h"
#include "../IncantationBuffer/IncantationBuffer.h"
#include "Keys.h"

namespace logger = SKSE::log;


// Default activation key
constexpr uint32_t ACTIVATION_KEY = Keys::Numpad0;

// Keys that can be used to write spells
constexpr uint32_t magicKeys[] = {
    Keys::Numpad1,
    Keys::Numpad2,
    Keys::Numpad3,
    Keys::Numpad4,
    Keys::Numpad5,
    Keys::Numpad6,
    Keys::Numpad7,
    Keys::Numpad8,
    Keys::Numpad9,
    Keys::NumpadPlus
};

InputManager* InputManager::GetSingleton()
{
    static InputManager singleton;
    return std::addressof(singleton);
}

void InputManager::HandleWeaponStateChange(RE::WEAPON_STATE weaponState)
{
    if (weaponState == RE::WEAPON_STATE::kDrawn) {
        logger::info("Player drew weapon");
    }
    else if (weaponState == RE::WEAPON_STATE::kSheathed) {
        logger::info("Player sheathed weapon");

        auto* magicMode = MagicModeManager::GetSingleton();
        if (magicMode->IsActive()) {
            magicMode->Deactivate();
        }
    }
}

RE::BSEventNotifyControl InputManager::ProcessEvent(
    RE::InputEvent* const* eventPtr,
    RE::BSTEventSource<RE::InputEvent*>*)
{
    if (!eventPtr) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto* inputEvent = *eventPtr;
    if (!inputEvent) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto* actorState = player->AsActorState();
    if (!actorState) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto weaponState = actorState->GetWeaponState();
    if (!_hasWeaponState) {
        _previousWeaponState = weaponState;
        _hasWeaponState = true;
    } else if (weaponState != _previousWeaponState) {
        HandleWeaponStateChange(weaponState);
        _previousWeaponState = weaponState;
    }

    for (auto* event = inputEvent; event; event = event->next) {
        if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kButton) {
            auto* buttonEvent = event->AsButtonEvent();
            if (!buttonEvent) {
                continue;
            }

            auto keyCode = buttonEvent->GetIDCode();
            auto* magicMode = MagicModeManager::GetSingleton();
            if (buttonEvent->IsDown() && actorState->IsWeaponDrawn()) {
                if (keyCode == ACTIVATION_KEY) {
                    if (magicMode->IsActive()) {
                        magicMode->Deactivate();
                    }
                    else {
                        magicMode->Activate();
                    }
                }
                else if (std::ranges::find(magicKeys, keyCode) != std::end(magicKeys) && magicMode->IsActive()) {
                    logger::info("0 {:X}", keyCode);
                    auto* buffer = IncantationBuffer::GetSingleton();
                    buffer->PushKey(keyCode);
                }
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}
