#include "GrammarDatabase.h"
#include "../Input/Keys.h"

GrammarDatabase* GrammarDatabase::GetSingleton()
{
    static GrammarDatabase singleton;
    if (singleton._schoolMap.empty()) {
        singleton.Initialize();
    }
    return std::addressof(singleton);
}

void GrammarDatabase::Initialize()
{
    //
    // Elements
    //

    _elementMap[Keys::Numpad8] = Element::Fire;
    _elementMap[Keys::Numpad2] = Element::Frost;
    _elementMap[Keys::Numpad6] = Element::Shock;
    _elementMap[Keys::Numpad4] = Element::Arcane;
    _elementMap[Keys::Numpad5] = Element::Heal;

    //
    // Delivery
    //

    _deliveryMap[Keys::Numpad8] = DeliveryType::Projectile;
    _deliveryMap[Keys::Numpad2] = DeliveryType::Self;
    _deliveryMap[Keys::Numpad7] = DeliveryType::AoE;
    _deliveryMap[Keys::Numpad9] = DeliveryType::Concentration;
    _deliveryMap[Keys::Numpad5] = DeliveryType::Summon;


    //
    // School
    //

    _schoolMap[Keys::Numpad6] = School::Alteration;
    _schoolMap[Keys::Numpad5] = School::Conjuration;
    _schoolMap[Keys::Numpad8] = School::Destruction;
    _schoolMap[Keys::Numpad4] = School::Illusion;
    _schoolMap[Keys::Numpad2] = School::Restoration;

    //
    // Level
    //

    _levelMap[Keys::Numpad4] = Level::Novice;
    _levelMap[Keys::Numpad7] = Level::Apprentice;
    _levelMap[Keys::Numpad8] = Level::Adept;
    _levelMap[Keys::Numpad9] = Level::Expert;
    _levelMap[Keys::Numpad6] = Level::Master;
}

Element GrammarDatabase::GetElement(uint32_t keycode)
{
    if (_elementMap.contains(keycode)) {
        return _elementMap[keycode];
    }

    return Element::None;
}

DeliveryType GrammarDatabase::GetDelivery(uint32_t keycode)
{
    if (_deliveryMap.contains(keycode)) {
        return _deliveryMap[keycode];
    }

    return DeliveryType::None;
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
