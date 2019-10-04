// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CONFLICTMANAGER_H_
#define SATTOOLS_CONFLICTMANAGER_H_

#include <vector>
#include <limits>
#include <utility>
#include <algorithm>

#include "sattools/Bitset.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"

namespace sat {

class ConflictManager {
 public:
    explicit ConflictManager(const Trail &trail) : _trail(trail) {}
    ~ConflictManager() {}

    void computeFirstUIP(Clause *conflict,
                        std::vector<Literal> *reason_used_to_infer_the_conflict,
                        std::vector<Literal> *learnt);

 private:
    const Trail& _trail;

    SparseBitset<BooleanVariable> _is_marked;
    SparseBitset<BooleanVariable> _is_independent;
    std::vector<uint32> _min_trail_index_per_level;

    std::vector<BooleanVariable> _dfs_stack;
    std::vector<BooleanVariable> _variable_to_process;

    unsigned int computeMaxTrailIndex(const Clause* clause);

    void minimizeConflict(std::vector<Literal>* conflict);

    void minimizeConflictSimple(std::vector<Literal>* conflict);
    void minimizeConflictRecursively(std::vector<Literal>* conflict);


    bool canBeInferedFromConflictVariables(BooleanVariable variable);
};

}  // namespace sat

#endif  // SATTOOLS_CONFLICTMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
