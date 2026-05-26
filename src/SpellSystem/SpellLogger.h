#pragma once

#include "SpellTypes.h"

/**
 * @brief Centralized logging system for spell attributes
 * 
 * This logger can be toggled on/off by setting SPELL_LOGGING_ENABLED to true/false.
 * When enabled, logs all spell attributes to a CSV-formatted log file.
 * Logs are written to a dedicated file that can be easily processed by Python scripts.
 */
class SpellLogger
{
public:
    static SpellLogger* GetSingleton();

    // Enable/disable logging - change this to toggle logging on/off
    static constexpr bool SPELL_LOGGING_ENABLED = true;

    /**
     * @brief Log all spell attributes
     * @param spell The spell to log
     * @param classified Optional pre-classified spell data
     */
    void LogSpell(RE::SpellItem* spell, const ClassifiedSpell* classified = nullptr);

    /**
     * @brief Log player spells - call this after player spells are loaded
     */
    void LogPlayerSpells();

    /**
     * @brief Flush any pending logs to disk
     */
    void Flush();

private:
    SpellLogger() = default;

    std::string ElementToString(Element element);
    std::string DeliveryToString(Delivery delivery);
    std::string SchoolToString(School school);
    std::string LevelToString(Level level);
    std::string OtherToString(Other other);

    std::string GetSpellEditorID(RE::SpellItem* spell);
    std::string GetSpellName(RE::SpellItem* spell);

    void WriteHeader();
};
