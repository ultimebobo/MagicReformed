#include "GrammarDatabase.h"
#include "../Input/Keys.h"
#include "../Settings/ConfigManager.h"

GrammarDatabase* GrammarDatabase::GetSingleton()
{
    static GrammarDatabase singleton;
    if (singleton._schoolMap.empty()) {
        singleton.LoadConfiguration();
    }
    return std::addressof(singleton);
}

void GrammarDatabase::LoadConfiguration()
{
    // Load config file
    ConfigManager* configMgr = ConfigManager::GetSingleton();
    configMgr->LoadConfigFile();

    // Load Schools of Magic
    _schoolMap.clear();
    _schoolMap[configMgr->GetKeyCode("Schools of Magic", "Alteration", Keys::Numpad6)] = School::Alteration;
    _schoolMap[configMgr->GetKeyCode("Schools of Magic", "Conjuration", Keys::Numpad5)] = School::Conjuration;
    _schoolMap[configMgr->GetKeyCode("Schools of Magic", "Destruction", Keys::Numpad8)] = School::Destruction;
    _schoolMap[configMgr->GetKeyCode("Schools of Magic", "Illusion", Keys::Numpad4)] = School::Illusion;
    _schoolMap[configMgr->GetKeyCode("Schools of Magic", "Restoration", Keys::Numpad2)] = School::Restoration;

    // Load Delivery
    _deliveryMap.clear();
    _deliveryMap[configMgr->GetKeyCode("Delivery", "Projectile", Keys::Numpad8)] = Delivery::Projectile;
    _deliveryMap[configMgr->GetKeyCode("Delivery", "Self", Keys::Numpad2)] = Delivery::Self;
    _deliveryMap[configMgr->GetKeyCode("Delivery", "AoE", Keys::Numpad7)] = Delivery::AoE;
    _deliveryMap[configMgr->GetKeyCode("Delivery", "Summon", Keys::Numpad5)] = Delivery::Summon;
    _deliveryMap[configMgr->GetKeyCode("Delivery", "Reanimate", Keys::Numpad9)] = Delivery::Reanimate;
    _deliveryMap[configMgr->GetKeyCode("Delivery", "BoundWeapon", Keys::Numpad1)] = Delivery::BoundWeapon;

    // Load Level
    _levelMap.clear();
    _levelMap[configMgr->GetKeyCode("Level", "Novice", Keys::Numpad4)] = Level::Novice;
    _levelMap[configMgr->GetKeyCode("Level", "Apprentice", Keys::Numpad7)] = Level::Apprentice;
    _levelMap[configMgr->GetKeyCode("Level", "Adept", Keys::Numpad8)] = Level::Adept;
    _levelMap[configMgr->GetKeyCode("Level", "Expert", Keys::Numpad9)] = Level::Expert;
    _levelMap[configMgr->GetKeyCode("Level", "Master", Keys::Numpad6)] = Level::Master;

    // Load Element
    _elementMap.clear();
    _elementMap[configMgr->GetKeyCode("Element", "Fire", Keys::Numpad8)] = Element::Fire;
    _elementMap[configMgr->GetKeyCode("Element", "Frost", Keys::Numpad7)] = Element::Frost;
    _elementMap[configMgr->GetKeyCode("Element", "Shock", Keys::Numpad9)] = Element::Shock;
    _elementMap[configMgr->GetKeyCode("Element", "Hostile", Keys::Numpad4)] = Element::Hostile;
    _elementMap[configMgr->GetKeyCode("Element", "Friendly", Keys::Numpad2)] = Element::Friendly;
}

Element GrammarDatabase::GetElement(uint32_t keycode)
{
    if (_elementMap.contains(keycode)) {
        return _elementMap[keycode];
    }

    return Element::None;
}

Delivery GrammarDatabase::GetDelivery(uint32_t keycode)
{
    if (_deliveryMap.contains(keycode)) {
        return _deliveryMap[keycode];
    }

    return Delivery::None;
}

School GrammarDatabase::GetSchool(uint32_t keycode)
{
    if (_schoolMap.contains(keycode)) {
        return _schoolMap[keycode];
    }

    return School::None;
}

Level GrammarDatabase::GetLevel(uint32_t keycode)
{
    if (_levelMap.contains(keycode)) {
        return _levelMap[keycode];
    }

    return Level::None;
}
