// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_TRAIL_H_
#define SATTOOLS_TRAIL_H_

#include <vector>


#include "sattools/Assignment.h"
#include "sattools/Literal.h"

namespace sat {

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

    unsigned int index() const { return _current_info.trail_index; }
    const Literal operator[](int index) const { return _trail[index]; }

    const Assignment& assignment() const { return _assignment; }

 private:
    Assignment _assignment;
    AssignmentInfo _current_info;
    std::vector<AssignmentInfo> _infos;
    std::vector<Literal> _trail;
};

}  // namespace sat

#endif  // SATTOOLS_TRAIL_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
