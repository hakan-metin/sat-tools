// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKERINFO_H_
#define SATTOOLS_BREAKERINFO_H_

#include <vector>

#include "sattools/Assignment.h"
#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"
#include "sattools/Macros.h"

namespace sat {

class BreakerInfo {
 public:
    BreakerInfo(const std::unique_ptr<Permutation>&permutation,
                const Assignment& assignment);
    ~BreakerInfo();

    void addLookupLiteral(Literal literal) { _lookup.push_back(literal); }
    bool isStable() const { return _lookup_index >= _lookup.size(); }
    bool generateSBP() { return false; }

 private:
    const std::unique_ptr<Permutation>& _permutation;
    const Assignment &_assignment;

    std::vector<Literal> _lookup;
    unsigned int _lookup_index;

    // void update() {
    //     for (; _lookup_index<_lookup.size(); _lookup_index++) {
    //         Literal element = _lookup[_lookup_index];
    //         Literal image = _permutation->imageOf(element);

    //         if (_assignment.hasSameAssignmentValue(element, image))
    //             break;
    //     }
    // }
};

}  // namespace sat
#endif  // SATTOOLS_BREAKERINFO_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
