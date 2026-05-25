#include "SpellLogger.h"
#include "SpellClassifier.h"
#include "SpellResolver.h"

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

    logger::info("SPELL_LOG|{}|{}|{}|{}|{}|{}|{}",
        editorID,
        name,
        SchoolToString(classified->school),
        LevelToString(classified->level),
        DeliveryToString(classified->delivery),
        ElementToString(classified->element),
        cost
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
    logger::info("SPELL_LOG|EditorID|Name|School|Level|Delivery|Element|Cost");

    auto* classifier = SpellClassifier::GetSingleton();
    auto* resolver = SpellResolver::GetSingleton();

    // Log base spells
    auto* baseSpells = player->GetActorBase()->GetSpellList();
    if (baseSpells && baseSpells->spells) {
        for (uint32_t i = 0; i < baseSpells->numSpells; i++) {
            auto* spell = baseSpells->spells[i];
            if (!resolver->IsValidSpell(spell)) continue;
            if (spell && spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
                auto classified = classifier->Classify(spell);
                LogSpell(spell, &classified);
            }
        }
    }

    // Log learned spells
    auto& addedSpells = player->GetActorRuntimeData().addedSpells;
    for (auto* spell : addedSpells) {
        if (!resolver->IsValidSpell(spell)) continue;
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
        case Element::Hostile: return "Hostile";
        case Element::Friendly: return "Friendly";
        case Element::None:
        default: return "None";
    }
}

std::string SpellLogger::DeliveryToString(Delivery delivery)
{
    switch (delivery) {
        case Delivery::Projectile: return "Projectile";
        case Delivery::Self: return "Self";
        case Delivery::AoE: return "AoE";
        case Delivery::Summon: return "Summon";
        case Delivery::Reanimate: return "Reanimate";
        case Delivery::BoundWeapon: return "BoundWeapon";
        case Delivery::None:
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
        case Level::Novice: return "Novice";
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
        logger::info("SPELL_LOG|EditorID|Name|School|Level|Delivery|Element|Cost");
    }
}
