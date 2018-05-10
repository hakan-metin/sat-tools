/* Original Licence */
// Copyright 2010-2017 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef SATTOOLS_ASSIGNMENT_H_
#define SATTOOLS_ASSIGNMENT_H_

#include "sattools/Bitset.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"

namespace sat {

class Assignment {
 public:
    Assignment() {}
    explicit Assignment(unsigned int num_variables) {
        resize(num_variables);
    }
    ~Assignment() {}

    void resize(unsigned int num_variables);
    void assignFromTrueLiteral(Literal literal);
    void unassignLiteral(Literal literal);

    bool literalIsTrue(Literal literal) const;
    bool literalIsFalse(Literal literal) const;
    bool literalIsAssigned(Literal literal) const;
    bool variableIsAssigned(BooleanVariable variable) const;

    Literal getTrueLiteralForAssignedVariable(BooleanVariable var) const;
    Literal getFalseLiteralForAssignedVariable(BooleanVariable var) const;

    unsigned int numberOfVariables() const;

 private:
    sat::Bitset64<LiteralIndex> _assignment;

    DISALLOW_COPY_AND_ASSIGN(Assignment);
};

inline void Assignment::resize(unsigned int num_variables) {
    _assignment.Resize(LiteralIndex(num_variables << 1));
}

inline void Assignment::assignFromTrueLiteral(Literal literal) {
    DCHECK(!variableIsAssigned(literal.variable()));
    _assignment.Set(literal.index());
}

inline void Assignment::unassignLiteral(Literal literal) {
    DCHECK(variableIsAssigned(literal.variable()));
    _assignment.ClearTwoBits(literal.index());
}

inline bool Assignment::literalIsTrue(Literal literal) const {
    return _assignment.IsSet(literal.index());
}

inline bool Assignment::literalIsFalse(Literal literal) const {
    return _assignment.IsSet(literal.negatedIndex());
}

inline bool Assignment::literalIsAssigned(Literal literal) const {
    return _assignment.AreOneOfTwoBitsSet(literal.index());
}

inline bool Assignment::variableIsAssigned(BooleanVariable var) const {
    return _assignment.AreOneOfTwoBitsSet(LiteralIndex(var.value() << 1));
}

inline Literal
Assignment::getTrueLiteralForAssignedVariable(BooleanVariable var) const {
    DCHECK(variableIsAssigned(var));
    return Literal(var, _assignment.IsSet(LiteralIndex(var.value() << 1)));
}

inline Literal
Assignment::getFalseLiteralForAssignedVariable(BooleanVariable var) const {
    DCHECK(variableIsAssigned(var));
    return Literal(var, !_assignment.IsSet(LiteralIndex(var.value() << 1)));
}

inline unsigned int Assignment::numberOfVariables() const {
    return _assignment.size().value() / 2;
}

}  // namespace sat

#endif  // SATTOOLS_ASSIGNMENT_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
