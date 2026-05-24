# ✨ MagicReformed

## The Problem

You've installed dozens of amazing spell mods. Your spell list is now enormous—hundreds of powerful, creative abilities crafted by talented modders. But here's the catch: **you keep using the same five spells** because scrolling through endless menus is exhausting.

Sound familiar? MagicReformed solves this.

## The Solution

**MagicReformed** is an SKSE plugin that lets you equip spells through an **incantation** using **keyboard shortcuts based on spell attributes**. Instead of digging through menus, press a sequence of keys to instantly select a spell matching your criteria.

### How It Works

Use your numpad to input an "incantation":
- **School of Magic** (Destruction, Conjuration, etc.)
- **Delivery Type** (Projectile, Self, AoE, Concentration)
- **Difficulty Level** (Novgice, Apprentice, Adept, Expert, Master)

The plugin instantly finds a matching spell from your spell list and equips it. **No more menu crawling. No more clicking.** Just natural, keyboard-driven spell selection.

## Key Features

✨ **Lightning-Fast Spell Selection** - Equip spells by pressing numpad combinations  
📊 **Intelligent Classification** - Automatically categorizes spells by school, delivery, level, and element  

## Project Structure

```
MagicReformed/
├── Grammar/                 # Spell query grammar and classification types
│   ├── GrammarDatabase.h/cpp    # Maps keycodes to spell attributes
│   ├── GrammarTypes.h           # Spell attribute enums (Element, School, Level, etc.)
│   ├── IncantationParser.h/cpp  # Parses input sequences
│   └── IncantationBuffer.h/cpp  # Buffers parsed incantations
├── SpellSystem/             # Core spell analysis and resolution
│   ├── SpellClassifier.h/cpp    # Detects spell attributes (school, level, delivery)
│   ├── SpellResolver.h/cpp      # Finds matching spells from player inventory
│   ├── SpellLogger.h/cpp        # Comprehensive logging system (easily toggleable)
│   └── SpellTypes.h             # Spell data structures
├── Input/                   # Input handling
│   ├── InputManager.h/cpp       # Processes keyboard input
│   └── Keys.h               # Keycode definitions
├── SpellSelector/           # Spell equipping
│   ├── SpellSelector.h/cpp      # Equips selected spells
├── MagicMode/               # Magic mode state management
│   ├── MagicModeManager.h/cpp
└── plugin.cpp               # Plugin entry point
```

## Building

### Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/) (Community edition is free)
- [CMake](https://cmake.org/download/) 3.25.1+
- [`vcpkg`](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` environment variable set

### Compile

```bash
cd MagicReformed
cmake --preset=default
cmake --build build/debug --config Debug
```

The `.dll` will be output to your SKSE plugins folder (if configured).

### Configure Output

**For Skyrim SE/AE:**
```
Environment Variable: SKYRIM_FOLDER
Value: C:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition
```

**For Mod Organizer 2 / Vortex:**
```
Environment Variable: SKYRIM_MODS_FOLDER
Value: C:\path\to\mods\folder
```
