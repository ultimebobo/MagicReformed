#include "SpellLogger.h"
#include "SpellClassifier.h"

namespace logger = SKSE::log;

SpellLogger* SpellLogger::GetSingleton()
{
    static SpellLogger singleton;
    return std::addressof(singleton);
}

void SpellLogger::LogSpell(RE::SpellItem* spell, const ClassifiedSpell* classified)
{
    if (!SPELL_LOGGING_ENABLED) {
        return;
    }

    if (!spell) {
        return;
    }

    // Classify spell if not already classified
    ClassifiedSpell temp;
    if (!classified) {
        temp = SpellClassifier::GetSingleton()->Classify(spell);
        classified = &temp;
    }

    // Format: EditorID|Name|School|Level|Delivery|Element|Cost|Type
    std::string editorID = GetSpellEditorID(spell);
    std::string name = GetSpellName(spell);

    auto cost = spell->CalculateMagickaCost(nullptr);
    auto type = static_cast<int>(spell->GetSpellType());

    logger::info("SPELL_LOG|{}|{}|{}|{}|{}|{}|{}|{}",
        editorID,
        name,
        SchoolToString(classified->school),
        LevelToString(classified->level),
        DeliveryTypeToString(classified->delivery),
        ElementToString(classified->element),
        cost,
        type
    );
}

void SpellLogger::LogPlayerSpells()
{
    if (!SPELL_LOGGING_ENABLED) {
        return;
    }

    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return;
    }

    logger::info("=== SPELL LOGGING START ===");
    logger::info("SPELL_LOG|EditorID|Name|School|Level|Delivery|Element|Cost|Type");

    auto* classifier = SpellClassifier::GetSingleton();

    // Log base spells
    auto* baseSpells = player->GetActorBase()->GetSpellList();
    if (baseSpells && baseSpells->spells) {
        for (uint32_t i = 0; i < baseSpells->numSpells; i++) {
            auto* spell = baseSpells->spells[i];
            if (spell && spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
                auto classified = classifier->Classify(spell);
                LogSpell(spell, &classified);
            }
        }
    }

    // Log learned spells
    auto& addedSpells = player->GetActorRuntimeData().addedSpells;
    for (auto* spell : addedSpells) {
        if (spell && spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
            auto classified = classifier->Classify(spell);
            LogSpell(spell, &classified);
        }
    }

    logger::info("=== SPELL LOGGING END ===");
    Flush();
}

void SpellLogger::Flush()
{
    if (SPELL_LOGGING_ENABLED) {
        spdlog::get("log")->flush();
    }
}

std::string SpellLogger::ElementToString(Element element)
{
    switch (element) {
        case Element::Fire: return "Fire";
        case Element::Frost: return "Frost";
        case Element::Shock: return "Shock";
        case Element::Arcane: return "Arcane";
        case Element::Heal: return "Heal";
        case Element::None:
        default: return "None";
    }
}

std::string SpellLogger::DeliveryTypeToString(DeliveryType delivery)
{
    switch (delivery) {
        case DeliveryType::Projectile: return "Projectile";
        case DeliveryType::Self: return "Self";
        case DeliveryType::AoE: return "AoE";
        case DeliveryType::Concentration: return "Concentration";
        case DeliveryType::Summon: return "Summon";
        case DeliveryType::None:
        default: return "None";
    }
}

std::string SpellLogger::SchoolToString(School school)
{
    switch (school) {
        case School::Alteration: return "Alteration";
        case School::Conjuration: return "Conjuration";
        case School::Destruction: return "Destruction";
        case School::Illusion: return "Illusion";
        case School::Restoration: return "Restoration";
        case School::None:
        default: return "None";
    }
}

std::string SpellLogger::LevelToString(Level level)
{
    switch (level) {
        case Level::Apprentice: return "Apprentice";
        case Level::Adept: return "Adept";
        case Level::Expert: return "Expert";
        case Level::Master: return "Master";
        case Level::None:
        default: return "None";
    }
}

std::string SpellLogger::GetSpellEditorID(RE::SpellItem* spell)
{
    if (!spell) return "Unknown";
    
    auto editorID = spell->GetFormEditorID();
    if (editorID) {
        return std::string(editorID);
    }
    
    return "Unknown";
}

std::string SpellLogger::GetSpellName(RE::SpellItem* spell)
{
    if (!spell) return "Unknown";
    
    auto name = spell->GetName();
    if (name && strlen(name) > 0) {
        return name;
    }
    
    return "Unknown";
}

void SpellLogger::WriteHeader()
{
    if (SPELL_LOGGING_ENABLED) {
        logger::info("SPELL_LOG|EditorID|Name|School|Level|Delivery|Element|Cost|Type");
    }
}
