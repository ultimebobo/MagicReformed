#include "ConfigManager.h"
#include "SimpleIni.h"
#include <fstream>
#include <algorithm>
#include <cctype>

// Map of key names to keycodes
const std::unordered_map<std::string, uint32_t> ConfigManager::KEY_MAP = {
    {"Numpad0", Keys::Numpad0},
    {"Numpad1", Keys::Numpad1},
    {"Numpad2", Keys::Numpad2},
    {"Numpad3", Keys::Numpad3},
    {"Numpad4", Keys::Numpad4},
    {"Numpad5", Keys::Numpad5},
    {"Numpad6", Keys::Numpad6},
    {"Numpad7", Keys::Numpad7},
    {"Numpad8", Keys::Numpad8},
    {"Numpad9", Keys::Numpad9},
    {"NumpadPlus", Keys::NumpadPlus},
};

ConfigManager* ConfigManager::GetSingleton()
{
    static ConfigManager singleton;
    if (!singleton._isLoaded) {
        singleton.LoadConfigFile();
    }
    return std::addressof(singleton);
}

bool ConfigManager::LoadConfigFile()
{
    CSimpleIniA ini;
    ini.SetUnicode();

    SI_Error rc = ini.LoadFile("Data/SKSE/Plugins/MagicReformed.ini");
    if (rc < 0) {
        return false; // File not found or error reading
    }

    // Cache all sections and keys
    CSimpleIniA::TNamesDepend sections;
    ini.GetAllSections(sections);

    for (const auto& section : sections) {
        CSimpleIniA::TNamesDepend keys;
        ini.GetAllKeys(section.pItem, keys);

        for (const auto& key : keys) {
            const char* value = ini.GetValue(section.pItem, key.pItem);
            if (value) {
                _config[section.pItem][key.pItem] = value;
            }
        }
    }

    _isLoaded = true;
    _displayAllNotifications = GetBool("General", "DisplayNotifications", false);
    _displayResolvedSpell = GetBool("General", "DisplaySpell", true);

    return true;
}

uint32_t ConfigManager::GetKeyCode(const std::string& section, const std::string& key, uint32_t defaultValue)
{
    if (!_isLoaded) {
        return defaultValue;
    }

    auto sectionIt = _config.find(section);
    if (sectionIt == _config.end()) {
        return defaultValue;
    }

    auto keyIt = sectionIt->second.find(key);
    if (keyIt == sectionIt->second.end()) {
        return defaultValue;
    }

    uint32_t keyCode = ParseKeyCode(keyIt->second);
    SKSE::log::info("Config key [{}] in section [{}] has value [{}]. Parsed key code: {}.", key, section, keyIt->second, keyCode);
    return (keyCode != 0) ? keyCode : defaultValue;
}

bool ConfigManager::GetBool(const std::string& section, const std::string& key, bool defaultValue)
{
    if (!_isLoaded) {
        return defaultValue;
    }

    auto sectionIt = _config.find(section);
    if (sectionIt == _config.end()) {
        return defaultValue;
    }

    auto keyIt = sectionIt->second.find(key);
    if (keyIt == sectionIt->second.end()) {
        return defaultValue;
    }

    std::string value = keyIt->second;
    // Trim whitespace
    value.erase(0, value.find_first_not_of(" \t\r\n"));
    value.erase(value.find_last_not_of(" \t\r\n") + 1);

    // Convert to lowercase for comparison
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });

    // Check for true values
    if (value == "true" || value == "1" || value == "yes" || value == "on") {
        return true;
    }

    // Check for false values
    if (value == "false" || value == "0" || value == "no" || value == "off") {
        return false;
    }

    return defaultValue;
}

uint32_t ConfigManager::ParseKeyCode(const std::string& keyName)
{
    // Trim whitespace
    std::string trimmed = keyName;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

    auto it = KEY_MAP.find(trimmed);
    if (it != KEY_MAP.end()) {
        return it->second;
    }

    // Try to parse as hex number (0x4C format)
    if (trimmed.size() >= 2 && trimmed[0] == '0' && (trimmed[1] == 'x' || trimmed[1] == 'X')) {
        try {
            return static_cast<uint32_t>(std::stoul(trimmed, nullptr, 16));
        } catch (...) {
            return 0;
        }
    }

    // Try to parse as decimal
    try {
        return static_cast<uint32_t>(std::stoul(trimmed));
    } catch (...) {
        return 0;
    }
}
