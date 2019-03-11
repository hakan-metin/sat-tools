
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
    for (Clause *clause : _clauses)
        delete clause;
}

bool CNFModel::addClause(std::vector<Literal>* literals) {
    CHECK_GT(literals->size(), static_cast<unsigned int>(0));

    // Remove duplicate literals in clause.
    std::sort(literals->begin(), literals->end());
    literals->erase(std::unique(literals->begin(), literals->end()),
                    literals->end());

    // Check if clause is trivial.
    for (unsigned int i = 1; i < literals->size(); i++) {
        if (literals->at(i) == literals->at(i - 1).negated()) {
            _num_trivial_clauses++;
            return false;
        }
    }

    // Update numberOfVariables().
    BooleanVariable var = literals->back().variable();
    _num_variables = std::max<int>(_num_variables, var.value());

    // Update statistics.
    switch (literals->size()) {
    case 2: _num_binary_clauses++;  break;
    case 3: _num_ternary_clauses++; break;
    default: break;
    }

    // Add clause.
    Clause *clause = Clause::create(*literals, false);
    addClause(clause);

    return true;
}

void CNFModel::addClause(Clause *clause) {
    _clauses.push_back(clause);

    // Resize internal structure.
    const unsigned int required_size = numberOfVariables() << 1;
    if (required_size > _literal_to_clauses.size())
        _literal_to_clauses.resize(required_size);

    // Update occurence list.
    for (Literal l : *clause) {
        _literal_to_clauses[l.index()].push_back(clause);
    }
}


void CNFModel::clearDetachedClauses() {
    auto i = _clauses.begin();
    auto j = _clauses.begin();
    const auto end = _clauses.end();

    // Add clauses and clean lazyDetach and satisfied clauses
    while (i != end) {
        Clause *clause = *j++ = *i++;
        if (clause->size() == 0) {
            delete clause;
            j--;
        }
    }

    if (j < i) {
        while (i != end)
            *j++ = *i++;
        _clauses.resize(j - _clauses.begin());
    }
}

void CNFModel::removeOcccurenceListOf(LiteralIndex lit_index, Clause *clause) {
    auto it = std::find(_literal_to_clauses[lit_index].begin(),
                        _literal_to_clauses[lit_index].end(), clause);
    _literal_to_clauses[lit_index].erase(it);
}

const std::vector<Clause*> CNFModel::occurenceListOf(Literal lit) const {
    return _literal_to_clauses[lit.index()];
}

Literal CNFModel::findLiteralWithShortestOccurenceList(Clause *clause) const {
    Literal result = clause->literals()[0];

    for (const Literal &literal : *clause)
        if (_literal_to_clauses[literal.index()].size() <
            _literal_to_clauses[result.index()].size())
            result = literal;
    return result;
}


}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
