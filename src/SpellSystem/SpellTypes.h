#pragma once

#include "../Grammar/GrammarTypes.h"

struct ClassifiedSpell
{
    RE::SpellItem* spell = nullptr;

    Element element = Element::None;
    Delivery delivery = Delivery::None;
    School school = School::None;
    Level level = Level::None;
    Other other = Other::None;
};
