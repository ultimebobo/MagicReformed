#include "SpellClassifier.h"

 namespace logger = SKSE::log;

SpellClassifier* SpellClassifier::GetSingleton()
{
    static SpellClassifier singleton;
    return std::addressof(singleton);
}

ClassifiedSpell SpellClassifier::Classify(RE::SpellItem* spell)
{
    ClassifiedSpell result;
    result.spell = spell;
    result.element = DetectElement(spell);
    result.delivery = DetectDelivery(spell);
    result.school = DetectSchool(spell);
    result.level = DetectLevel(spell);

    return result;
}

Element SpellClassifier::DetectElement(RE::SpellItem* spell)
{
    if (!spell) {
        return Element::None;
    }

    if (!spell->effects.empty()) {
        for (auto* effect : spell->effects) {
            if (effect && effect->baseEffect != nullptr) {
                RE::ActorValue resistVariable = effect->baseEffect->data.resistVariable;
                switch (resistVariable) {
                    case RE::ActorValue::kResistFire:
                        return Element::Fire;
                    case RE::ActorValue::kResistFrost:
                        return Element::Frost;
                    case RE::ActorValue::kResistShock:
                        return Element::Shock;
                    default:
                        break;
                }
            }
        }
    }

    
    return Element::Arcane;
}

Delivery SpellClassifier::DetectDelivery(RE::SpellItem* spell)
{
    if (!spell) {
        return Delivery::None;
    }

    auto delivery = spell->GetDelivery();
    switch (delivery) {
        case RE::MagicSystem::Delivery::kSelf:
            return Delivery::Self;

        case RE::MagicSystem::Delivery::kTouch:
        case RE::MagicSystem::Delivery::kAimed:
        case RE::MagicSystem::Delivery::kTargetActor:
            return Delivery::Projectile;

        case RE::MagicSystem::Delivery::kTargetLocation:
            return Delivery::AoE;

        default:
            break;
    }

    if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
        return Delivery::Concentration;
    }

    return Delivery::Projectile;
}

School SpellClassifier::DetectSchool(RE::SpellItem* spell)
{
    if (!spell) {
        return School::None;
    }

    auto school = spell->GetAssociatedSkill();
    switch (school) {
        case RE::ActorValue::kAlteration:
            return School::Alteration;

        case RE::ActorValue::kConjuration:
            return School::Conjuration;

        case RE::ActorValue::kDestruction:
            return School::Destruction;

        case RE::ActorValue::kIllusion:
            return School::Illusion;

        case RE::ActorValue::kRestoration:
            return School::Restoration;

        default:
            break;
    }

    return School::None;
}

Level SpellClassifier::DetectLevel(RE::SpellItem* spell)
{
    if (!spell) {
        return Level::None;
    }

    int32_t level = -1;
    if (!spell->effects.empty()) {
        for (auto* effect : spell->effects) {
            if (effect && effect->baseEffect != nullptr &&
                effect->baseEffect->GetMagickSkill() == spell->GetAssociatedSkill()) {
                level = effect->baseEffect->GetMinimumSkillLevel();
                break;
            }
        }
    }

    if (level < 25) {
        return Level::Novice;
    } else if (level < 50) {
        return Level::Apprentice;
    } else if (level < 75) {
        return Level::Adept;
    } else if (level < 100) {
        return Level::Expert;
    } else if (level >= 100) {
        return Level::Master;
    } else {
        return Level::None;
    }
}