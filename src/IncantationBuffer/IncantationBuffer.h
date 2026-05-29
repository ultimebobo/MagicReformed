#pragma once

#include <ctime>

class IncantationBuffer
{
public:
    static IncantationBuffer* GetSingleton();

    void PushKey(uint32_t keycode);
    void ConfirmSequence();

private:
    IncantationBuffer() = default;

    void Clear();

private:
    std::vector<uint32_t> _sequence;
    uint32_t _confirmKey = 0;
};
