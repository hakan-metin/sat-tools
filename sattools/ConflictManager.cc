// Copyright 2017 Hakan Metin

#include "sattools/ConflictManager.h"

namespace sat {


void ConflictManager::computeFirstUIP(Clause *conflict,
                       std::vector<Literal> *reason_used_to_infer_the_conflict,
                       std::vector<Literal> *learnt) {
    const unsigned int num_variables = _trail.assignment().numberOfVariables();
    unsigned int trail_index = computeMaxTrailIndex(conflict);
    const BooleanVariable var = _trail[trail_index].variable();
    const unsigned int highest_level = _trail.decisionLevel(var);
    int num_literal_at_highest_level_that_needs_to_be_processed = 0;
    Clause *clause_to_expand = conflict;

    reason_used_to_infer_the_conflict->clear();
    learnt->clear();

    _is_marked.ClearAndResize(BooleanVariable(num_variables));

    while (true) {
        DCHECK(clause_to_expand != nullptr);

        for (Literal literal : *clause_to_expand) {
            const BooleanVariable var = literal.variable();
            const unsigned int level = _trail.info(var).level;

            if (_is_marked[var] || level == 0)
                continue;

            reason_used_to_infer_the_conflict->push_back(literal);
            _is_marked.Set(var);

            if (level == highest_level)
                num_literal_at_highest_level_that_needs_to_be_processed++;
            else
                learnt->push_back(literal);
        }

        // Find next marked literal to expand from the trail.
        DCHECK_GT(num_literal_at_highest_level_that_needs_to_be_processed, 0);
        while (!_is_marked[_trail[trail_index].variable()])
            --trail_index;

        if (num_literal_at_highest_level_that_needs_to_be_processed == 1) {
            learnt->push_back(_trail[trail_index].negated());
            std::swap(learnt->front(), learnt->back());
            break;
        }

        const Literal literal = _trail[trail_index];

        clause_to_expand = _trail.reason(literal.variable());
        num_literal_at_highest_level_that_needs_to_be_processed--;
        trail_index--;
    }

    minimizeConflict(learnt);
}


unsigned int ConflictManager::computeMaxTrailIndex(const Clause* clause) {
    uint32 index = 0;

    for (const Literal& literal : *clause)
        index = std::max(index, _trail.info(literal.variable()).trail_index);

    DCHECK_GT(index, 0);

    return index;
}

void ConflictManager::minimizeConflict(std::vector<Literal>* conflict) {
    minimizeConflictRecursively(conflict);
}


void ConflictManager::minimizeConflictSimple(std::vector<Literal>* conflict) {
    auto i = conflict->begin() + 1;
    auto j = conflict->begin() + 1;

    while (i != conflict->end()) {
        BooleanVariable v = i->variable();
        Clause *reason = _trail.reason(v);
        if (reason == nullptr) {
            *j++ = *i;
        } else {
            for (Literal literal : *reason) {
                const BooleanVariable v = literal.variable();
                if (!_is_marked[v] && _trail.decisionLevel(v) > 0) {
                    *j++ = *i;
                    break;
                }
            }
        }
        ++i;
    }
    conflict->erase(j, i);
}


void
ConflictManager::minimizeConflictRecursively(std::vector<Literal>* conflict) {
    auto i = conflict->begin() + 1;
    auto j = conflict->begin() + 1;

    _dfs_stack.clear();

    while (i != conflict->end()) {
        const BooleanVariable var = i->variable();
        const Clause *reason = _trail.reason(var);

        _dfs_stack.push_back(var);

        if (reason == nullptr || !canBeInferedFromConflictVariables(var))
            *j++ = *i;
        ++i;
    }
    conflict->erase(j, i);
}

bool
ConflictManager::canBeInferedFromConflictVariables(BooleanVariable variable) {
    _variable_to_process.clear();
    _variable_to_process.push_back(variable);

    unsigned int size = _dfs_stack.size();

    while (!_variable_to_process.empty()) {
        const BooleanVariable current_var = _variable_to_process.back();
        _variable_to_process.pop_back();

        Clause *clause = _trail.reason(current_var);

        CHECK_NOTNULL(clause);

        auto i = clause->begin() + 1;
        while (i != clause->end()) {
            const BooleanVariable var = i->variable();

            if (!_is_marked[var] && _trail.decisionLevel(var) > 0) {
                if (_trail.reason(var) != nullptr) {
                    _is_marked.Set(var);
                    _variable_to_process.push_back(var);
                    _dfs_stack.push_back(var);
                } else {
                    for (unsigned int i = size; i < _dfs_stack.size(); i++)
                        _is_marked.Clear(_dfs_stack[i]);
                    _dfs_stack.erase(_dfs_stack.begin()+size, _dfs_stack.end());
                    return false;
                }
            }
            ++i;
        }
    }
    return true;
}


# if 0
void
ConflictManager::minimizeConflictRecursively(std::vector<Literal>* conflict) {
    const unsigned int num_variables = _trail.assignment().numberOfVariables();
    const unsigned int current_decision_level = _trail.currentDecisionLevel();

    LOG(INFO) << " Start Minimize";

    _is_independent.ClearAndResize(BooleanVariable(num_variables));

    if (current_decision_level >= _min_trail_index_per_level.size())
        _min_trail_index_per_level.resize(current_decision_level + 1,
                                          std::numeric_limits<uint32>::max());

    for (BooleanVariable var : _is_marked.PositionsSetAtLeastOnce()) {
        const unsigned int level = _trail.decisionLevel(var);
        _min_trail_index_per_level[level] = std::min(
               _min_trail_index_per_level[level], _trail.info(var).trail_index);
    }

    auto i = conflict->begin() + 1;
    auto j = conflict->begin() + 1;

    while (i != conflict->end()) {
        const BooleanVariable var = i->variable();
        const unsigned int level = _trail.decisionLevel(var);

        if (_trail.info(var).trail_index <= _min_trail_index_per_level[level] ||
            !canBeInferedFromConflictVariables(var)) {
            _is_independent.Set(var);
            *j++ = *i;
        }
        ++i;
    }
    conflict->erase(j, i);
    _min_trail_index_per_level.clear();
}


bool
ConflictManager::canBeInferedFromConflictVariables(BooleanVariable variable) {
    _dfs_stack.clear();
    _dfs_stack.push_back(variable);
    _variable_to_process.clear();
    _variable_to_process.push_back(variable);

    DCHECK(_is_marked[variable]);

    LOG(INFO) << "Try inferred " << variable;

    Clause *reason = _trail.reason(variable);
    for (Literal literal : *reason) {
        const BooleanVariable var = literal.variable();

        DCHECK_NE(var, variable);

        if (_is_marked[var])
            continue;


        const unsigned int level = _trail.decisionLevel(var);
        if (level == 0) {
            _is_marked.Set(var);
            continue;
        }

        if (_trail.info(var).trail_index <= _min_trail_index_per_level[level] ||
            _is_independent[var])
            return false;

        _variable_to_process.push_back(var);
    }


    while (!_variable_to_process.empty()) {
        const BooleanVariable current_var = _variable_to_process.back();
        LOG(INFO) << "process " << current_var;
        if (current_var == _dfs_stack.back()) {
            if (_dfs_stack.size() > 1) {
                DCHECK(!_is_marked[current_var]);
                _is_marked.Set(current_var);
            }

            _dfs_stack.pop_back();
            _variable_to_process.pop_back();
            continue;
        }

        if (_is_marked[current_var]) {
            _variable_to_process.pop_back();
            continue;
        }

        DCHECK(!_is_independent[current_var]);

        _dfs_stack.push_back(current_var);
        bool abort_early = false;
        for (Literal literal : *(_trail.reason(current_var))) {
            const BooleanVariable var = literal.variable();
            const unsigned int level = _trail.decisionLevel(var);

            if (level == 0 || _is_marked[var])
                continue;

           if (_trail.info(var).trail_index <= _min_trail_index_per_level[level]
               || _is_independent[var]) {
               abort_early = true;
               break;
           }

           DCHECK_NE(var, current_var);
           _variable_to_process.push_back(var);
        }

        if (abort_early)
            break;
    }

    for (const BooleanVariable var : _dfs_stack) {
        _is_independent.Set(var);
    }
    return _dfs_stack.empty();
}
#endif

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
