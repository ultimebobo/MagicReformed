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
    result.other = DetectOther(spell);

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
                // Destruction spells
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

                auto archetype = effect->baseEffect->GetArchetype();
                switch (archetype) {
                    case RE::EffectSetting::Archetype::kInvisibility:
                        return Element::Invisibility;
                    default:
                        break;
                }

                // Illusion/Restoration spells
                if (effect->baseEffect->data.flags.all(RE::EffectSetting::EffectSettingData::Flag::kHostile)) {
                    return Element::Hostile;
                } else {
                    return Element::Friendly;
                }
            }
        }
    }

    return Element::None;
}

Delivery SpellClassifier::DetectDelivery(RE::SpellItem* spell)
{
    if (!spell) {
        return Delivery::None;
    }

    if (!spell->effects.empty()) {
        for (auto* effect : spell->effects) {
            if (effect && effect->baseEffect != nullptr) {
                // Conjuration spells
                auto archetype = effect->baseEffect->GetArchetype();
                switch (archetype) {
                    case RE::EffectSetting::Archetype::kSummonCreature:
                        return Delivery::Summon;
                    case RE::EffectSetting::Archetype::kReanimate:
                        return Delivery::Reanimate;
                    case RE::EffectSetting::Archetype::kBoundWeapon:
                        return Delivery::BoundWeapon;
                    default:
                        break;
                }
            }
        }
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

    return Delivery::None;
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

Other SpellClassifier::DetectOther(RE::SpellItem* spell)
{
    if (!spell) {
        return Other::None;
    }

    if (!spell->effects.empty()) {
        for (auto* effect : spell->effects) {
            if (effect && effect->baseEffect != nullptr) {
                auto archetype = effect->baseEffect->GetArchetype();
                switch (archetype) {
                    case RE::EffectSetting::Archetype::kAbsorb:
                        return Other::Absorb;
                    case RE::EffectSetting::Archetype::kSoulTrap:
                        return Other::SoulTrap;
                    case RE::EffectSetting::Archetype::kDetectLife:
                        return Other::DetectLife;
                    case RE::EffectSetting::Archetype::kFrenzy:
                        return Other::Frenzy;
                    case RE::EffectSetting::Archetype::kInvisibility:
                        return Other::Invisibility;
                    case RE::EffectSetting::Archetype::kLight:
                        return Other::Light;
                    case RE::EffectSetting::Archetype::kParalysis:
                        return Other::Paralyze;
                    case RE::EffectSetting::Archetype::kReanimate:
                        return Other::Reanimate;
                    case RE::EffectSetting::Archetype::kCloak:
                        return Other::Cloak;
                    case RE::EffectSetting::Archetype::kOpen:
                        return Other::Open;
                    case RE::EffectSetting::Archetype::kTelekinesis:
                        return Other::Telekinesis;
                    default:
                        break;
                }
            }
        }
    }

    return Other::None;
}