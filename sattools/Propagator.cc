// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Propagator.h"

namespace sat {

Propagator::Propagator() : _propgation_trail_index(0) {
}

Propagator::~Propagator() {
}


void Propagator::resize(unsigned int num_vars) {
    _watchers.resize(num_vars << 2);
    _reasons.resize(num_vars);
}

bool Propagator::addClause(const std::vector<Literal>& literals, Trail *trail) {
    Clause* clause = Clause::create(literals, false);
    if (!addClause(clause, trail))
        return false;
    return true;
}

bool Propagator::addLearntClause(const std::vector<Literal>& literals,
                                 Trail *trail) {
    Clause* clause = Clause::create(literals, true);
    if (!addClause(clause, trail))
        return false;
    return true;
}

bool Propagator::addClause(Clause *clause, Trail *trail) {
    const int size = clause->size();
    Literal *literals = clause->literals();

    if (size == 1) {
        Literal unit = literals[0];
        const Assignment &assignment = trail->assignment();

        if (assignment.literalIsFalse(unit))     return false;
        if (assignment.literalIsAssigned(unit))  return true;

        trail->enqueueWithUnitReason(unit);
        if (!propagate(trail))
            return false;

        return true;
    }

    int num_literal_not_false = 0;
    for (int i = 0; i < size; ++i) {
        if (!trail->assignment().literalIsFalse(literals[i])) {
            std::swap(literals[i], literals[num_literal_not_false]);
            ++num_literal_not_false;
            if (num_literal_not_false == 2) {
                break;
            }
        }
    }

    if (num_literal_not_false == 0)
        return false;

    if (num_literal_not_false == 1) {
        // maintain literal 1 as highest decision level to assure 2-watch algo
        int max_level = trail->info(literals[1].variable()).level;
        for (int i = 2; i < size; ++i) {
            const int level = trail->info(literals[i].variable()).level;
            if (level > max_level) {
                max_level = level;
                std::swap(literals[1], literals[i]);
            }
        }
        // Propagates literals[0] if it is unassigned.
        if (!trail->assignment().literalIsTrue(literals[0])) {
            _reasons[trail->index()] = clause;
            trail->enqueueWithAssertiveReason(literals[0]);
        }
    }
    attachOnFalse(literals[0], literals[1], clause);
    attachOnFalse(literals[1], literals[0], clause);

    return true;
}


void Propagator::attachClause(Clause *clause, Trail *trail) {
    Literal* literals = clause->literals();
    CHECK(!trail->assignment().literalIsAssigned(literals[0]));
    CHECK(!trail->assignment().literalIsAssigned(literals[1]));

    attachOnFalse(literals[0], literals[1], clause);
    attachOnFalse(literals[1], literals[0], clause);
}

void
Propagator::attachOnFalse(Literal literal, Literal blocking, Clause *clause) {
    _watchers[literal.index()].emplace_back(clause, blocking);
}


void Propagator::detachClause(Clause *clause) {
    LOG(FATAL) << "Not yet Implemented";
}

bool Propagator::propagate(Trail *trail) {
    while (_propgation_trail_index < trail->index()) {
        const Literal literal = (*trail)[_propgation_trail_index++];
        LOG(INFO) << "propagate " << literal.debugString();
        if (!propagateOnFalse(literal.negated(), trail))
            return false;
    }
    return true;
}


bool Propagator::propagateOnFalse(Literal false_literal, Trail *trail) {
    const Assignment& assignment = trail->assignment();
    std::vector<Watch>& watchers = _watchers[false_literal.index()];

    _conflict = nullptr;

    auto i = watchers.begin();
    auto j = watchers.begin();

    const auto end = watchers.end();

    while (i != end) {
        const Watch& watch = *j++ = *i++;

        if (assignment.literalIsTrue(watch.blocking_literal))
            continue;

        Literal *literals = watch.clause->literals();
        Literal other(LiteralIndex(literals[0].index().value() ^
                                   literals[1].index().value() ^
                                   false_literal.index().value()));

        literals[0] = other;
        literals[1] = false_literal;

        if (assignment.literalIsTrue(other)) {
            j[-1].blocking_literal = other;
        } else {
            const Literal *literals_end = watch.clause->end();
            Literal *k = literals + 2;
            while (k != literals_end && assignment.literalIsFalse(*k))
                k++;

            if (k != literals_end) {
                const Literal lit = *k;
                if (assignment.literalIsTrue(lit)) {
                    j[-1].blocking_literal = lit;
                } else {
                    literals[1] = lit;
                    *k = false_literal;
                    attachOnFalse(lit, other, watch.clause);
                    j--;   // drop this watch
                }
            } else {  // end
                if (assignment.literalIsFalse(other)) {
                    _conflict = watch.clause;
                    break;
                } else {  // Found unit clause
                    _reasons[trail->index()] = watch.clause;
                    trail->enqueueWithAssertiveReason(other);
                }
            }
        }
    }

    if (j < i) {
        while (i != end)
            *j++ = *i++;
        watchers.resize(j - watchers.begin());
    }

    return _conflict == nullptr;
}


std::string Propagator::debugString() const {
    std::stringstream ss;

    for (const auto &watchers : _watchers) {
        for (const Watch &watch : watchers) {
            ss << watch.blocking_literal.debugString();
        }
    }

    return ss.str();
}



}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
