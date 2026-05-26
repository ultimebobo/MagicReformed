#pragma once

enum class Element
{
    None,
    Fire,
    Frost,
    Shock,
    Invisibility,
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

// Used to get info, not in the actual grammar
enum class Other
{
    None,
    Absorb,
    SoulTrap,
    DetectLife,
    Frenzy,
    Invisibility,
    Light,
    Paralyze,
    Reanimate,
    Cloak,
    Open,
    Telekinesis
};

struct SpellQuery
{
    Element element = Element::None;
    Delivery delivery = Delivery::None;
    School school = School::None;
    Level level = Level::None;
    Other other = Other::None;
};
