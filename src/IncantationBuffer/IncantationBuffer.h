#pragma once

#include <ctime>

class IncantationBuffer
{
public:
    static IncantationBuffer* GetSingleton();

    void PushKey(uint32_t keycode);

    void ConfirmSequence();

    void CancelSequence();

    void Update();

    bool IsEmpty() const;

private:
    IncantationBuffer() = default;

    void Clear();

private:
    std::vector<uint32_t> _sequence;

    std::time_t _lastInputTime = 0;

    // Seconds before auto-confirm
    const double _timeoutDuration = 1.2;
};