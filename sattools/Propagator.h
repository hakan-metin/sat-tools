// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_PROPAGATOR_H_
#define SATTOOLS_PROPAGATOR_H_

#include <vector>
#include <sstream>
#include <string>

#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"
#include "sattools/IntTypeIndexedVector.h"

namespace sat {

class Propagator {
 public:
    Propagator();
    ~Propagator();

    void resize(unsigned int num_vars);

    bool propagate(Trail *trail);

    void attachClause(Clause *clause, Trail *trail);
    void detachClause(Clause *clause);


    std::string debugString() const;

 private:
    unsigned int _propgation_trail_index;

    struct Watch {
        Watch() {}
        Watch(Clause *c, Literal b) : clause(c), blocking_literal(b) {}
        Clause *clause;
        Literal blocking_literal;
    };

    ITIVector<LiteralIndex, std::vector<Watch>> _watchers;
    std::vector<Clause*> _reasons;

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
