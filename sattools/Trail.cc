// Copyright 2017 Hakan Metin

#include "sattools/Trail.h"

namespace sat {

void Trail::enqueue(Literal literal) {
    DCHECK(!_assignment.literalIsAssigned(literal));
    _assignment.assignFromTrueLiteral(literal);
    _trail[_current_info.trail_index] = literal;
    _current_info.last_polarity = literal.isPositive();
    _infos[literal.variable().value()] = _current_info;
    _current_info.trail_index++;
}


void Trail::dequeue() {
    int index =  _current_info.trail_index;
    Literal literal = _trail[index];
    _assignment.unassignLiteral(literal);
    _current_info.level = _infos[literal.variable().value()].level;
    _current_info.trail_index--;
}


void Trail::newDecisionLevel() {
    _current_info.level++;
}

unsigned int Trail::currentDecisionLevel() const {
    return _current_info.level;
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
