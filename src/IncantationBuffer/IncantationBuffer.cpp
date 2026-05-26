#include "IncantationBuffer.h"
#include "../SpellSelector/SpellSelector.h"
#include "../Settings/ConfigManager.h"
#include "../Input/Keys.h"

namespace logger = SKSE::log;

IncantationBuffer* IncantationBuffer::GetSingleton()
{
    static IncantationBuffer singleton;
    if (singleton._confirmKey == 0) {
        // Load confirm key from config
        auto* config = ConfigManager::GetSingleton();
        singleton._confirmKey = config->GetKeyCode("General", "ResolveSpell", Keys::NumpadPlus);
    }
    return std::addressof(singleton);
}

void IncantationBuffer::PushKey(uint32_t keycode)
{
    if (keycode == _confirmKey) {
        ConfirmSequence();
        return;
    }

    _sequence.push_back(keycode);
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

void IncantationBuffer::Clear()
{
    _sequence.clear();
}
