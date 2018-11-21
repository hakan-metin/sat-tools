// Copyright 2017 Hakan Metin

# include "sattools/sat/Clause.h"

namespace sattools {

// static
Clause* Clause::create(const std::vector<Literal>& literals,
                       bool is_redundant) {
    Clause *clause = reinterpret_cast<Clause*>(
        ::operator new(sizeof(Clause) + literals.size() * sizeof(Literal)));

    clause->_size  = literals.size();
    for (unsigned int i = 0; i < literals.size(); i++) {
        clause->_literals[i] = literals[i];
    }

    clause->_is_redundant = is_redundant;

    return clause;
}

std::string Clause::debugString() const {
    std::string str;
    for (const Literal& literal : *this) {
        if (!str.empty()) str.append(" ");
        str.append(literal.debugString());
    }
    return str;
}


}  // namespace sattools
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
