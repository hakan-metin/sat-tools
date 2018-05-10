// Copyright 2017 Hakan Metin

# include "sattools/Clause.h"

namespace sat {

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

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
