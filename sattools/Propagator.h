// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_PROPAGATOR_H_
#define SATTOOLS_PROPAGATOR_H_

#include <vector>

#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"
#include "sattools/IntTypeIndexedVector.h"

namespace sat {

class Propagator {
 public:
    Propagator();
    ~Propagator();

    bool propagate(Trail *trail);

    void attachClause(Clause *clause, Trail *trail);
    void detachClause(Clause *clause);

 private:
    unsigned int _propgation_trail_index;

    struct Watcher {
        Watcher() {}
        Watcher(Clause *c, Literal b) : clause(c), blocking_literal(b) {}
        Clause *clause;
        Literal blocking_literal;
    };

    ITIVector<LiteralIndex, std::vector<Watcher>> _watchers;

    void attachOnFalse(Literal literal, Literal blocking, Clause *clause);
    bool propagateOnFalse(Literal false_literal, Trail *trail);
};


}  // namespace sat


#endif  // SATTOOLS_PROPAGATOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
