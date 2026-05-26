#include "InputManager.h"

#include "../MagicMode/MagicModeManager.h"
#include "../IncantationBuffer/IncantationBuffer.h"
#include "../Settings/ConfigManager.h"
#include "../Grammar/GrammarDatabase.h"
#include "Keys.h"

#include <algorithm>

namespace logger = SKSE::log;

InputManager* InputManager::GetSingleton()
{
    static InputManager singleton;
    if (singleton._magicModeKey == 0) {
        auto* config = ConfigManager::GetSingleton();
        singleton._magicModeKey = config->GetKeyCode("General", "MagicMode", Keys::Numpad0);

        // Get all grammar keys from the grammar database
        auto* grammarDb = GrammarDatabase::GetSingleton();
        singleton._magicKeys = grammarDb->GetAllGrammarKeys();

        // Add the confirm key
        uint32_t confirmKey = config->GetKeyCode("General", "ResolveSpell", Keys::NumpadPlus);
        singleton._magicKeys.push_back(confirmKey);

        // Deduplicate and sort
        std::sort(singleton._magicKeys.begin(), singleton._magicKeys.end());
        singleton._magicKeys.erase(std::unique(singleton._magicKeys.begin(), singleton._magicKeys.end()), singleton._magicKeys.end());
    }
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
                if (keyCode == _magicModeKey) {
                    if (magicMode->IsActive()) {
                        magicMode->Deactivate();
                    }
                    else {
                        magicMode->Activate();
                    }
                }
                else if (std::find(_magicKeys.begin(), _magicKeys.end(), keyCode) != _magicKeys.end() && magicMode->IsActive()) {
                    auto* buffer = IncantationBuffer::GetSingleton();
                    buffer->PushKey(keyCode);
                }
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}
