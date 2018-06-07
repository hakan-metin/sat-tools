// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_GROUP_H_
#define SATTOOLS_GROUP_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include <string>

#include "sattools/Macros.h"
#include "sattools/Permutation.h"

namespace sat {

class Group {
 public:
    Group();
    ~Group();

    bool addPermutation(std::unique_ptr<Permutation>&& permutation);
    struct Iterator;
    Iterator watch(BooleanVariable var) const;

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
    void debugPrint() const;

 private:
    std::vector< std::unique_ptr<Permutation> > _permutations;
    std::unordered_set<BooleanVariable> _symmetric;
    std::unordered_set<BooleanVariable> _inverting;
    std::vector< std::unordered_set<int> > _watchers;
};

struct Group::Iterator {
    typedef int value_type;
    typedef std::unordered_set<int>::const_iterator const_iterator;

    Iterator() {}
    Iterator(const std::unordered_set<int>::const_iterator& b,
             const std::unordered_set<int>::const_iterator& e) :
        _begin(b), _end(e) {}

    std::unordered_set<int>::const_iterator begin() const { return _begin; }
    std::unordered_set<int>::const_iterator end() const { return _end; }
    const std::unordered_set<int>::const_iterator _begin;
    const std::unordered_set<int>::const_iterator _end;

    int size() const { return std::distance(_begin, _end); }
};

}  // namespace sat

#endif  // SATTOOLS_GROUP_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
