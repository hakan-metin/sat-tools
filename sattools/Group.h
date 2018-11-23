// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_GROUP_H_
#define SATTOOLS_GROUP_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include <string>

#include "sattools/Macros.h"
#include "sattools/Permutation.h"
#include "sattools/Logging.h"
#include "sattools/Watcher.h"

namespace sat {

class Group {
 public:
    Group();
    ~Group();

    bool addPermutation(std::unique_ptr<Permutation>&& permutation);

    Watcher<int, int>::Iterator watch(Literal literal) const;
    Watcher<int, int>::Iterator watch(BooleanVariable var) const;

    const std::vector< std::unique_ptr<Permutation> >& permutations() const {
        return _permutations;
    }
    const std::unique_ptr<Permutation>& permutation(unsigned int index) const {
        return _permutations[index];
    }

    int64 numberOfPermutations() const { return _permutations.size(); }
    int64 numberOfSymmetricVariables() const { return _symmetric.size(); }
    int64 numberOfInverting() const { return _inverting.size(); }

    std::string debugString() const;

 private:
    std::vector< std::unique_ptr<Permutation> > _permutations;
    std::unordered_set<BooleanVariable> _symmetric;
    std::unordered_set<BooleanVariable> _inverting;
    Watcher<int, int> _watchers;

    DISALLOW_COPY_AND_ASSIGN(Group);
};

}  // namespace sat

#endif  // SATTOOLS_GROUP_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
