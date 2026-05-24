#pragma once

#include "GrammarTypes.h"

class GrammarDatabase
{
public:
    static GrammarDatabase* GetSingleton();

    void Initialize();

    Element GetElement(uint32_t keycode);
    Delivery GetDelivery(uint32_t keycode);
    School GetSchool(uint32_t keycode);
    Level GetLevel(uint32_t keycode);

private:
    GrammarDatabase() = default;

private:
    std::unordered_map<uint32_t, Element> _elementMap;
    std::unordered_map<uint32_t, Delivery> _deliveryMap;
    std::unordered_map<uint32_t, School> _schoolMap;
    std::unordered_map<uint32_t, Level> _levelMap;
};
