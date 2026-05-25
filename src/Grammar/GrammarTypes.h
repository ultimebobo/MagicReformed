#pragma once

enum class Element
{
    None,
    Fire,
    Frost,
    Shock,
    Hostile,
    Friendly
};

enum class Delivery
{
    None,
    Projectile,
    Self,
    AoE,
    Summon,
    Reanimate,
    BoundWeapon
};

enum class School
{
    None,
    Alteration,
    Conjuration,
    Destruction,
    Illusion,
    Restoration
};

enum class Level
{
    None,
    Novice,
    Apprentice,
    Adept,
    Expert,
    Master
};

struct SpellQuery
{
    Element element = Element::None;
    Delivery delivery = Delivery::None;
    School school = School::None;
    Level level = Level::None;
};
