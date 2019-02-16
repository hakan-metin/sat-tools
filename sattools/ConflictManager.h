// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CONFLICTMANAGER_H_
#define SATTOOLS_CONFLICTMANAGER_H_

#include "sattools/Bitset.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"

namespace sat {

class ConflictManager {
 public:
    ConflictManager() {}
    ~ConflictManager() {}

    void computeFirstUIP(const Trail& trail, Clause *conflict,
                         std::vector<Literal> *learnt);

 private:
    unsigned int computeMaxTrailIndex(const Clause* clause,
                                      const Trail& trail);

};


inline void
ConflictManager::computeFirstUIP( const Trail& trail, Clause *conflict,
                                  std::vector<Literal> *learnt) {
    const unsigned int num_variables = trail.assignment().numberOfVariables();
    unsigned int trail_index = trail.index() - 1;
    unsigned int highest_level = computeMaxTrailIndex(conflict, trail);
    int num_literal_at_highest_level_that_needs_to_be_processed = 0;
    Clause *clause_to_expand = conflict;
    SparseBitset<BooleanVariable> is_marked;

    learnt->clear();

    is_marked.ClearAndResize(BooleanVariable(num_variables));

    while (true) {
        DCHECK(clause_to_expand != nullptr);

        for (Literal literal : *clause_to_expand) {
            const BooleanVariable var = literal.variable();
            const unsigned int level = trail.info(var).level;

            if (is_marked[var] || level == 0)
                continue;

            is_marked.Set(var);

            if (level == highest_level)
                num_literal_at_highest_level_that_needs_to_be_processed++;
            else
                learnt->push_back(literal);
        }

        // Find next marked literal to expand from the trail.
        DCHECK_GT(num_literal_at_highest_level_that_needs_to_be_processed, 0);
        while (!is_marked[trail[trail_index].variable()])
            --trail_index;

        if (num_literal_at_highest_level_that_needs_to_be_processed == 1) {
            learnt->push_back(trail[trail_index].negated());

            for (Literal l : *learnt)
                std::cout << l.debugString() << " ";
            std::cout << std::endl;

            std::swap(learnt->front(), learnt->back());
            break;
        }

        const Literal literal = trail[trail_index];

        clause_to_expand = trail.reason(literal.variable());
        num_literal_at_highest_level_that_needs_to_be_processed--;
        trail_index--;
    }
}



inline unsigned int ConflictManager::computeMaxTrailIndex(const Clause* clause,
                                                          const Trail& trail) {
    uint32 index = 0;

    for (const Literal& literal : *clause)
        index = std::max(index, trail.info(literal.variable()).trail_index);

    DCHECK_GT(index, 0);

    return index;
}


// void Solver::minimizeConflictSimple(std::vector<Literal>* conflict)


// void Solver::minimizeConflict(std::vector<Literal>* conflict) {

// }

}  // namespace sat

#endif  // SATTOOLS_CONFLICTMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
