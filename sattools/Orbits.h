// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_ORBITS_H_
#define SATTOOLS_ORBITS_H_

#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/DisjointSets.h"
#include "sattools/Literal.h"
#include "sattools/Permutation.h"
#include "sattools/Group.h"

namespace sat {

class Orbits {
 public:
    Orbits() {}
    ~Orbits() {}

    void assign(const std::vector<Permutation*>& permutations);
    void assign(const Group& group);

    const std::vector< std::vector<Literal> >::const_iterator begin() {
        return _orbits.begin();
    }
    const std::vector< std::vector<Literal> >::const_iterator end() {
        return _orbits.end();
    }

    int64 numberOfOrbits() const { return _orbits.size(); }

    bool isInSameOrbit(Literal a, Literal b);

    std::string debugString() const;

 private:
    std::vector<Literal> _symmetrics;
    DisjointSets _disjointSets;
    std::vector< std::vector<Literal> > _orbits;
};

}  // namespace sat
#endif  // SATTOOLS_ORBITS_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
