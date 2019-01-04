
// Copyright 2017 Hakan Metin

#include "sattools/CNFModel.h"

namespace sat {

CNFModel::CNFModel() :
    _num_variables(0),
    _num_trivial_clauses(0),
    _num_binary_clauses(0),
    _num_ternary_clauses(0) {
}

CNFModel::~CNFModel() {
}

void CNFModel::addClause(std::vector<Literal>* literals) {
    CHECK_GT(literals->size(), static_cast<unsigned int>(0));

    // Remove duplicate literals in clause.
    std::sort(literals->begin(), literals->end());
    literals->erase(std::unique(literals->begin(), literals->end()),
                    literals->end());

    // Check if clause is trivial.
    for (unsigned int i = 1; i < literals->size(); i++) {
        if (literals->at(i) == literals->at(i - 1).negated()) {
            _num_trivial_clauses++;
            return;
        }
    }

    // Update numberOfVariables().
    BooleanVariable var = literals->back().variable();
    _num_variables = std::max<int>(_num_variables, var.value());


    // TODO REMOVE
    const int64 clause_index = _clauses.size();
    Clause *clause = Clause::create(*literals, /* is_redundant= */ false);

    _clauses.push_back(clause);

    if (clause->size() == 2)
        _num_binary_clauses++;

    const int64 num_vars = numberOfVariables();
    for (const Literal &literal : *clause) {
        const int64 literal_index = literal.variable().value();
        // Register each literal to associate clause
        _watchers.store(literal_index, clause_index);

        if (num_vars > literal_index)
            _occurences.resize(numberOfVariables());
        _occurences[literal_index]++;
    }
    return;
    /*
    // Resize internal structure.
    const unsigned int required_size = numberOfVariables();
    if (required_size > _literal_to_clauses.size())
        _literal_to_clauses.resize(required_size);

    // Add clause.
    const ClauseIndex clause_index = _clauses.size();
    _clauses.push_back(std::vector<Literal>());
    _clauses.back().swap(*literals);

    // Update statistics.
    switch (literals->size()) {
    case 2: _num_binary_clauses++;  break;
    case 3: _num_ternary_clauses++; break;
    default: break;
    }

    // Update occurence list.
    for (Literal l : _clauses.back()) {
        _literal_to_clauses[l.index()].push_back(clause_index);
        }*/
}

}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
