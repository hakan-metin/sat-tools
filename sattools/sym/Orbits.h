// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_ORBITS_H_
#define SATTOOLS_SYM_ORBITS_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "sattools/utils/DisjointSets.h"
#include "sattools/sat/Literal.h"
#include "sattools/sym/Permutation.h"
#include "sattools/sym/Group.h"

namespace sattools {

class Orbits {
 public:
    Orbits() {}
    ~Orbits() {}

    void assign(const std::vector<Permutation*>& permutations);
    void assign(const Group& group);

    const std::vector< std::vector<BooleanVariable> >::const_iterator begin() {
        return _orbits.begin();
    }
    const std::vector< std::vector<BooleanVariable> >::const_iterator end() {
        return _orbits.end();
    }

    int64 numberOfOrbits() const { return _orbits.size(); }

 private:
    std::vector<BooleanVariable> _symmetrics;
    std::vector< std::vector<BooleanVariable> > _orbits;
};

}  // namespace sattools
#endif  // SATTOOLS_SYM_ORBITS_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
