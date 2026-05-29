#pragma once

class MagicModeManager
{
public:
    static MagicModeManager* GetSingleton();

    void Activate();
    void Deactivate();

    bool IsActive() const;

private:
    MagicModeManager() = default;

    bool _isActive = false;
};
