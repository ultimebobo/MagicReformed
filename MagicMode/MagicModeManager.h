#pragma once

class MagicModeManager
{
public:
    static MagicModeManager* GetSingleton();

    void Activate();
    void Deactivate();

    bool IsActive() const;

    void Update();

private:
    MagicModeManager() = default;

    bool _isActive = false;
};