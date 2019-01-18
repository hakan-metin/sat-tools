// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Simplifier.h"

namespace sat {

Simplifier::Simplifier(CNFModel *model) : _model(model) {
}

Simplifier::~Simplifier() {
}

bool Simplifier::simplify(Trail *trail) {
    return processAllClauses(trail);
}


void Simplifier::addClauseToProcess(Clause *clause) {
    _clauses_to_process.insert(clause);

}
bool Simplifier::processAllClauses(Trail *trail) {
    while (_clauses_to_process.size() > 0) {
        Clause *clause = *_clauses_to_process.begin();
        _clauses_to_process.erase(_clauses_to_process.begin());

        if (clause->size() == 0)  // Detached
            continue;
        if (clause->size() == 1) {
            Literal unit = clause->literals()[0];
            if (trail->assignment().literalIsFalse(unit)) {
                LOG(INFO) << "CONTRADICTION";
                return false;
            }
            if (trail->assignment().literalIsAssigned(unit))
                continue;
            trail->enqueueWithUnitReason(unit);
        }

        if (!processClauseToSimplifiy(clause))
            return false;
    }

    return true;
}

bool Simplifier::processClauseToSimplifiy(Clause *clause) {
    LiteralIndex opposite_literal;
    Literal literal = _model->findLiteralWithShortestOccurenceList(clause);

    {  // Treat positive literal
        const std::vector<Clause*> &occ_list_ref =
            _model->occurenceListOf(literal);

        for (Clause *c : occ_list_ref) {
            if (clause == c || !simplifyClause(clause, c, &opposite_literal))
                continue;

            if (opposite_literal == kNoLiteralIndex) { // subsumed
                c->lazyDetach();
                continue;
            }

            if (c->size() == 0)   // UNSAT model
                return false;

            _clauses_to_process.insert(c);

            // Clause c not contains opposite_literal anymore so remove it from
            // occurence list.
            _model->removeOcccurenceListOf(opposite_literal, c);
        }
    }

    {  // Treat the negative literal
        const std::vector<Clause*> &occ_list_ref =
            _model->occurenceListOf(literal.negated());

        for (Clause *c : occ_list_ref) {
            if (clause == c || !simplifyClause(clause, c, &opposite_literal))
                continue;

            if (opposite_literal == kNoLiteralIndex) {
                c->lazyDetach();
                continue;
            }

            if (c->size() == 0)   // UNSAT model
                return false;

            _clauses_to_process.insert(c);

            // Clause c not contains opposite_literal anymore so remove it from
            // occurence list.
            _model->removeOcccurenceListOf(opposite_literal, c);
        }
    }

    return true;
}


bool Simplifier::simplifyClause(Clause *a, Clause *b,
                                LiteralIndex *opposite_literal) {
    if (b->size() < a->size())
        return false;

    *opposite_literal = kNoLiteralIndex;
    int num_diff = 0;

    // /!\ Warning this clause is 2-watch, don't move literals 0 and 1
    // this function is called one time. If we want to simplify during the
    // search, we can sort 3th to end and treat literals 0th and 1st separately
    DCHECK(std::is_sorted(a->begin(), a->end()));
    DCHECK(std::is_sorted(b->begin(), b->end()));

    Literal *ia = a->literals();
    Literal *ib = b->literals();
    Literal *to_remove = b->literals();

    int size_diff = b->size() - a->size();

    while (ia != a->end()) {
        if (*ia == *ib) {  // same literal
            ++ia;
            ++ib;
        } else if (*ia == ib->negated()) {  // opposite literal
            if (++num_diff > 1)
                return false;
            to_remove = ib;
            ++ia;
            ++ib;
        } else if (*ia < *ib) {
            return false;  // A literal of a is not in b. (quit because sorted)
        } else {  // *ia > *ib
            ++ib;
            // A literal of b is not in a,
            // we can abort early by comparing the sizes left.
            if (--size_diff < 0)
                return false;
        }
    }

    if (num_diff == 1) {
        *opposite_literal = to_remove->index();
        b->removeLiteral(to_remove);
    }

    return true;
}

}  // namespace sat
