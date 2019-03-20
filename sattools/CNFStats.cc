// Copyright 2017 Hakan Metin

#include "sattools/CNFStats.h"

namespace sat {

CNFStats::CNFStats() :
    _num_variables(0),
    _num_trivial_clauses(0),
    _num_unary_clauses(0),
    _num_binary_clauses(0),
    _num_ternary_clauses(0),
    _num_large_clauses(0) {
}

CNFStats::~CNFStats() {
    for (Clause *clause : _clauses)
        delete clause;
}

bool CNFStats::addClause(std::vector<Literal>* literals) {
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
    case 1:  _num_unary_clauses++;   break;
    case 2:  _num_binary_clauses++;  break;
    case 3:  _num_ternary_clauses++; break;
    default: _num_large_clauses++;   break;
    }


    unsigned int size = literals->size();
    unsigned int positive = 0;
    unsigned int negative = 0;

    for (Literal literal : *literals) {
        if (literal.isPositive()) positive++;
        if (literal.isNegative()) negative++;
    }


    // Update statistics.
    switch (literals->size()) {
    case 1:  _stats.unary_clauses.add(size, positive, negative);   break;
    case 2:  _stats.binary_clauses.add(size, positive, negative);  break;
    case 3:  _stats.ternary_clauses.add(size, positive, negative); break;
    default: _stats.large_clauses.add(size, positive, negative);   break;
    }


    // Add clause.
    Clause *clause = Clause::create(*literals, false);
    addClause(clause);

    return true;
}

void CNFStats::addClause(Clause *clause) {
    _clauses.push_back(clause);
}

void CNFStats::summarize() const {
    const int64 ncl = numberOfClauses();

    Printer::printSection(" Instance Informations ");
    Printer::printStat("Number of variables", numberOfVariables());
    Printer::printStat("Number of clauses", ncl);
    Printer::printStat(" |- unary clauses", numberOfUnaryClauses(), ncl);
    Printer::printStat(" |- binary clauses", numberOfBinaryClauses(), ncl);
    Printer::printStat(" |- ternary clauses", numberOfTernaryClauses(), ncl);
    Printer::printStat(" |- large clauses", numberOfLargeClauses(), ncl);
}

}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
