// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_PROPAGATOR_H_
#define SATTOOLS_PROPAGATOR_H_

#include <vector>
#include <sstream>
#include <string>
#include <utility>

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

    bool addClause(Clause *clause, Trail *trail);
    bool addClause(const std::vector<Literal>& literals, Trail *trail);
    bool addLearntClause(const std::vector<Literal>& literals, Trail *trail);
    // bool addSBPClause(const std::vector<Literal>& literals, Trail *trail);

    bool propagate(Trail *trail);

    void attachClause(Clause *clause, Trail *trail);
    void detachClause(Clause *clause);

    Clause *conflictClause() const { return _conflict; }
    Clause *reasonClause(unsigned int index) const { return _reasons[index]; }

    std::string debugString() const;

 private:
    unsigned int _propgation_trail_index;
    Clause *_conflict;

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
