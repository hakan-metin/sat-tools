// Copyright 2017 Hakan Metin

#include "sattools/CNFModel.h"

namespace sat {

CNFModel::CNFModel() : _num_variables(0) {
}

CNFModel::~CNFModel() {
}

void CNFModel::addClause(std::vector<Literal>* literals) {
    CHECK_GT(literals->size(), static_cast<unsigned int>(0));

    /* Remove duplicate literals in clause */
    std::sort(literals->begin(), literals->end());
    auto last_element_it = std::unique(literals->begin(), literals->end());
    literals->erase(last_element_it, literals->end());

    BooleanVariable var = literals->back().variable();
    if (var > _num_variables)
        _num_variables = var.value();

    std::unique_ptr<Clause> clause
        (Clause::create(*literals, /* is_redundant= */ false));

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
    return _unary_clauses.size() + _binary_clauses.size() +
        _ternary_clauses.size() + _large_clauses.size();
}

}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
