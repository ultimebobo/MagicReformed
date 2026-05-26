#pragma once

#include "../Input/Keys.h"
#include <string>
#include <unordered_map>

class ConfigManager
{
public:
    static ConfigManager* GetSingleton();

    // Load configuration from ini file
    bool LoadConfigFile();

    // Get key code from ini, with fallback to default
    uint32_t GetKeyCode(const std::string& section, const std::string& key, uint32_t defaultValue);

    // Get boolean value from ini, with fallback to default
    bool GetBool(const std::string& section, const std::string& key, bool defaultValue);

    // Helper to parse key name string to keycode
    static uint32_t ParseKeyCode(const std::string& keyName);

    // Helper to convert keycode to key name
    static std::string KeyCodeToString(uint32_t keyCode);

    bool shouldDisplayResolvedSpell() const { return _displayResolvedSpell; }
    bool shouldDisplayAllNotifications() const { return _displayAllNotifications; }

private:
    ConfigManager() = default;

    // Map of key names to their codes
    static const std::unordered_map<std::string, uint32_t> KEY_MAP;

    // Cache for loaded configuration
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _config;
    bool _isLoaded = false;
    bool _displayResolvedSpell = false;
    bool _displayAllNotifications = false;
};
