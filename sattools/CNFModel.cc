
// Copyright 2017 Hakan Metin

#include "sattools/CNFModel.h"

namespace sat {

CNFModel::CNFModel() : _num_variables(0), _num_trivial_clauses(0) {
}

CNFModel::~CNFModel() {
}

void CNFModel::addClause(std::vector<Literal>* literals) {
    CHECK_GT(literals->size(), static_cast<unsigned int>(0));

    /* Remove duplicate literals in clause */
    std::sort(literals->begin(), literals->end());
    literals->erase(std::unique(literals->begin(), literals->end()),
                    literals->end());

    /* Check if clause is trivial */
    for (unsigned int i = 1; i < literals->size(); i++) {
        if (literals->at(i) == literals->at(i - 1).negated()) {
            _num_trivial_clauses++;
            return;
        }
    }

    BooleanVariable var = literals->back().variable();
    _num_variables = std::max<int>(_num_variables, var.value());

    std::unique_ptr<Clause> clause(Clause::create(*literals,
                                                  /* is_redundant= */ false));

    const int64 num_vars = numberOfVariables();
    for (const Literal &literal : *clause) {
        const int64 index = literal.variable().value();
        if (num_vars > index)
            _occurences.resize(numberOfVariables());
        _occurences[index]++;
    }

    switch (clause->size()) {
    case 1:  _unary_clauses.emplace_back(clause.release());   break;
    case 2:  _binary_clauses.emplace_back(clause.release());  break;
    case 3:  _ternary_clauses.emplace_back(clause.release()); break;
    default: _large_clauses.emplace_back(clause.release());   break;
    }
}

int64 CNFModel::numberOfVariables() const {
    return _num_variables + 1;  // Because start from 0
}
int64 CNFModel::numberOfClauses() const {
    return _binary_clauses.size() +
        _ternary_clauses.size() +
        _large_clauses.size();
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::unaryClauses() {
    return { &_unary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::binaryClauses() {
    return { &_binary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::ternaryClauses() {
    return { &_ternary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::largeClauses() {
    return { &_large_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::clauses() {
    return { &_binary_clauses, &_ternary_clauses, &_large_clauses };
}


RangeIterator<std::unique_ptr<Clause>> CNFModel::unaryClauses() const {
    return { &_unary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::binaryClauses() const {
    return { &_binary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::ternaryClauses() const {
    return { &_ternary_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::largeClauses() const {
    return { &_large_clauses };
}

RangeIterator<std::unique_ptr<Clause>> CNFModel::clauses() const {
    return { &_binary_clauses, &_ternary_clauses, &_large_clauses };
}


}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
