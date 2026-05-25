#include "MagicModeManager.h"
#include "../SpellSystem/SpellLogger.h"
#include "../Equipment/EquipmentSnapshot.h"

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

    EquipmentSnapshot::GetSingleton()->SaveEquipment();

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

    EquipmentSnapshot::GetSingleton()->RestoreEquipment();

    _isActive = false;

    RE::DebugNotification("Magic mode deactivated");
}

bool MagicModeManager::IsActive() const
{
    return _isActive;
}
