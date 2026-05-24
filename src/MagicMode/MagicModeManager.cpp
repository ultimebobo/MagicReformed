#include "MagicModeManager.h"
#include "../SpellSystem/SpellLogger.h"

MagicModeManager* MagicModeManager::GetSingleton()
{
    static MagicModeManager singleton;
    return std::addressof(singleton);
}

void MagicModeManager::Activate()
{
    if (_isActive) {
        return;
    }

    _isActive = true;

    RE::DebugNotification("Magic mode activated");
    // Log all player spells once at startup
    SpellLogger::GetSingleton()->LogPlayerSpells();
}

void MagicModeManager::Deactivate()
{
    if (!_isActive) {
        return;
    }

    _isActive = false;

    RE::DebugNotification("Magic mode deactivated");
}

bool MagicModeManager::IsActive() const
{
    return _isActive;
}

void MagicModeManager::Update()
{
    if (!_isActive) {
        return;
    }

    auto player = RE::PlayerCharacter::GetSingleton();

    if (!player) {
        return;
    }

    // Auto-disable if player sheathes weapons
    if (!player->AsActorState()->IsWeaponDrawn()) {
        Deactivate();
    }
}