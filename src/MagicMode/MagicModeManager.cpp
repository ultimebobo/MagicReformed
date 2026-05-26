#include "MagicModeManager.h"
#include "../SpellSystem/SpellLogger.h"
#include "../Equipment/EquipmentSnapshot.h"
#include "../Settings/ConfigManager.h"

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

    if (ConfigManager::GetSingleton()->shouldDisplayAllNotifications()) {
        RE::DebugNotification("Focusing on spells...");
    }
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

    if (ConfigManager::GetSingleton()->shouldDisplayAllNotifications()) {
        RE::DebugNotification("Not focusing on spells anymore");
    }
}

bool MagicModeManager::IsActive() const
{
    return _isActive;
}
