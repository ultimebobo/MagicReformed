#pragma once

#include "SpellTypes.h"

class SpellClassifier
{
public:
    static SpellClassifier* GetSingleton();
    ClassifiedSpell Classify(RE::SpellItem* spell);

private:
    SpellClassifier() = default;

    Element DetectElement(RE::SpellItem* spell);
    Delivery DetectDelivery(RE::SpellItem* spell);
    School DetectSchool(RE::SpellItem* spell);
    Level DetectLevel(RE::SpellItem* spell);
    Other DetectOther(RE::SpellItem* spell);
};
