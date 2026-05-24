#include "IncantationParser.h"
#include "GrammarDatabase.h"

IncantationParser* IncantationParser::GetSingleton()
{
    static IncantationParser singleton;
    return std::addressof(singleton);
}

SpellQuery IncantationParser::Parse(const std::vector<uint32_t>& sequence)
{
    SpellQuery query;

    auto* grammar = GrammarDatabase::GetSingleton();

    // First symbol = school
    if (sequence.size() >= 1) {
        query.school = grammar->GetSchool(sequence[0]);
    }

    // Second symbol = delivery
    if (sequence.size() >= 2) {
        query.delivery = grammar->GetDelivery(sequence[1]);
    }

    // Third symbol = level
    if (sequence.size() >= 3) {
        query.level = grammar->GetLevel(sequence[2]);
    }

    // Fourth symbol = element
    if (sequence.size() >= 4) {
        query.element = grammar->GetElement(sequence[3]);
    }

    return query;
}
