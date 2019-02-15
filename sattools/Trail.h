// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_TRAIL_H_
#define SATTOOLS_TRAIL_H_

#include <vector>
#include <string>
#include <sstream>

#include "sattools/Assignment.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Propagator.h"

namespace sat {

// Forward declaration
class Propagator;

struct AssignmentInfo {
    AssignmentInfo() : last_polarity(false), level(0), trail_index(0) {}
    bool   last_polarity : 1;
    uint32 type          : 4;
    uint32 level         : 27;
    uint32 trail_index   : 32;

    std::string debugString() const {
        std::stringstream ss;
        ss << "level: " << level << " |"  << "type: " << type << " | "
           << "trail_index: " << trail_index;
        return ss.str();
    }
};

struct AssignmentType {  // /!\ Must be lower than type bits in AssignmentInfo
    static const unsigned int kUnitReason = 0;
    static const unsigned int kSearchDecision = 1;
    static const unsigned int kFirstFreePropagationId = 3;
};

class Trail {
 public:
    Trail() {}
    virtual ~Trail() {}

    void registerPropagator(Propagator *propagator);

    void resize(unsigned int num_vars);

    void enqueue(Literal literal, unsigned int id);
    void enqueueWithUnitReason(Literal literal);
    void enqueueSearchDecision(Literal literal);

    void dequeue();
    void cancelUntil(unsigned int target_level);
    void newDecisionLevel();
    unsigned int currentDecisionLevel() const;

    unsigned int assignmentType(BooleanVariable var) const;
    unsigned int index() const { return _current_info.trail_index; }
    const Literal operator[](int index) const { return _trail[index]; }

    Clause *reason(BooleanVariable var) const;

    const Assignment& assignment() const { return _assignment; }

    const AssignmentInfo& info(BooleanVariable var) const {
        DCHECK_GE(var, 0);
        DCHECK_LT(var, _infos.size());
        return _infos[var.value()];
    }

    std::string debugString() const;

 private:
    Assignment _assignment;
    AssignmentInfo _current_info;
    std::vector<AssignmentInfo> _infos;
    std::vector<Literal> _trail;
    std::vector<Propagator*> _propagators;

    DISALLOW_COPY_AND_ASSIGN(Trail);
};

}  // namespace sat

#endif  // SATTOOLS_TRAIL_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
