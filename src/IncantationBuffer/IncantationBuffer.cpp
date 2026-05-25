#include "IncantationBuffer.h"
#include "../SpellSelector/SpellSelector.h"
#include "../Input/Keys.h"

namespace logger = SKSE::log;

const uint32_t CONFIRM_KEY = Keys::NumpadPlus;

IncantationBuffer* IncantationBuffer::GetSingleton()
{
    static IncantationBuffer singleton;
    return std::addressof(singleton);
}

void IncantationBuffer::PushKey(uint32_t keycode)
{
    if (keycode == CONFIRM_KEY) {
        ConfirmSequence();
        return;
    }

    _sequence.push_back(keycode);
    _lastInputTime = std::time(nullptr);
}

void IncantationBuffer::ConfirmSequence()
{
    if (_sequence.empty()) {
        return;
    }

    logger::info("Sequence confirmed");
    SpellSelector::GetSingleton()->EquipSpell(_sequence);

    Clear();
}

void IncantationBuffer::CancelSequence()
{
    if (_sequence.empty()) {
        return;
    }

    logger::info("Sequence cancelled");
    RE::DebugNotification("Incantation cancelled");

    Clear();
}

bool IncantationBuffer::IsEmpty() const
{
    return _sequence.empty();
}

void IncantationBuffer::Clear()
{
    _sequence.clear();
    _lastInputTime = std::time_t(0);
}
