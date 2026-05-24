#include "SpellResolver.h"
#include "SpellClassifier.h"

#include <random>

namespace logger = SKSE::log;

SpellResolver* SpellResolver::GetSingleton()
{
    static SpellResolver singleton;
    return std::addressof(singleton);
}

bool SpellResolver::IsValidSpell(RE::SpellItem* spell)
{
    if (!spell) {
        return false;
    }

    if (spell->GetSpellType() != RE::MagicSystem::SpellType::kSpell) {

        return false;
    }

    return true;
}

RE::SpellItem* SpellResolver::ResolveSpell(const SpellQuery& query)
{
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return nullptr;
    }

    std::vector<RE::SpellItem*> matches;
    auto& addedSpells = player->GetActorRuntimeData().addedSpells;
    auto* classifier = SpellClassifier::GetSingleton();
    logger::info("Querying spells with criteria:, delivery={}, school={}, level={}", static_cast<int>(query.delivery), static_cast<int>(query.school), static_cast<int>(query.level));
    
    /**
     * Base spells
     */
    auto* baseSpells = player->GetActorBase()->GetSpellList();
    for (uint32_t i = 0; i < baseSpells->numSpells; i++) {
        auto* spell = baseSpells->spells[i];
        if (!IsValidSpell(spell)) continue;

        auto classified = classifier->Classify(spell);

        // if (classified.element != query.element) continue;
        if (classified.delivery != query.delivery) continue;
        if (classified.school != query.school) continue;
        if (classified.level != query.level) continue;

        matches.push_back(spell);
    }
    
    logger::info("{} spells to check", addedSpells.size());
    /**
     * Learned spells
     */
    for (auto* spell : addedSpells) {
        if (!IsValidSpell(spell)) continue;

        auto classified = classifier->Classify(spell);

        // if (classified.element != query.element) continue;
        if (classified.delivery != query.delivery) continue;
        if (classified.school != query.school) continue;
        if (classified.level != query.level) continue;

        matches.push_back(spell);
    }

    if (matches.empty()) {
        logger::warn("No spell matched query");
        return nullptr;
    }
    logger::info("{} spells matched query", matches.size());

    //
    // Random matching spell
    //
    static std::random_device rd;
    static std::mt19937 rng(rd());

    std::uniform_int_distribution<size_t> dist(0, matches.size() - 1);

    auto* selected = matches[dist(rng)];

    logger::info("Resolved spell: {}", selected->GetName());

    return selected;
}