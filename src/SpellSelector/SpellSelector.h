#pragma once

class SpellSelector
{
public:
    static SpellSelector* GetSingleton();

    void EquipSpell(std::vector<uint32_t>& sequence) const;
private:
    SpellSelector() = default;
};