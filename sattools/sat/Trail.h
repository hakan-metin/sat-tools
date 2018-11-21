// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SAT_TRAIL_H_
#define SATTOOLS_SAT_TRAIL_H_

#include <vector>


#include "sattools/sat/Assignment.h"
#include "sattools/sat/Literal.h"

namespace sattools {

struct AssignmentInfo {
    AssignmentInfo() : last_polarity(false), level(0), trail_index(0) {}
    bool   last_polarity : 1;
    uint32 level         : 31;
    uint32 trail_index   : 32;
};


class Trail {
 public:
    Trail();
    ~Trail();

    void enqueue(Literal literal);
    void dequeue();
    void cancelUntil(unsigned int target_level);
    void newDecisionLevel();
    unsigned int currentDecisionLevel() const;

 private:
    Assignment _assignment;
    AssignmentInfo _current_info;
    std::vector<AssignmentInfo> _infos;
    std::vector<Literal> _trail;
};

}  // namespace sattools

#endif  // SATTOOLS_SAT_TRAIL_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
