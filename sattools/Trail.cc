// Copyright 2017 Hakan Metin

#include "sattools/Trail.h"

namespace sat {



void Trail::resize(unsigned int num_vars) {
    _assignment.resize(num_vars << 2);
    _infos.resize(num_vars);
    _trail.resize(num_vars);
}

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


std::string Trail::debugString() const {
    std::string result;
    for (int i = 0; i < _current_info.trail_index; ++i) {
        if (!result.empty())
            result += " ";
        result += _trail[i].debugString();
    }
return result;
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
