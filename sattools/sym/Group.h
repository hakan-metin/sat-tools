// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_GROUP_H_
#define SATTOOLS_SYM_GROUP_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include <string>

#include "sattools/base/Macros.h"
#include "sattools/sym/Permutation.h"
#include "sattools/base/Logging.h"

namespace sattools {

class Group {
 public:
    Group();
    ~Group();

    bool addPermutation(std::unique_ptr<Permutation>&& permutation);
    struct Iterator;
    Iterator watch(BooleanVariable var) const;
    Iterator watch(Literal literal) const;

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
    std::vector< std::vector<int> > _watchers;

    DISALLOW_COPY_AND_ASSIGN(Group);
};

struct Group::Iterator {
    typedef int value_type;
    typedef std::vector<int>::const_iterator const_iterator;

    Iterator() {}
    Iterator(const std::vector<int>::const_iterator& b,
             const std::vector<int>::const_iterator& e) :
        _begin(b), _end(e) {}

    std::vector<int>::const_iterator begin() const { return _begin; }
    std::vector<int>::const_iterator end() const { return _end; }
    const std::vector<int>::const_iterator _begin;
    const std::vector<int>::const_iterator _end;

    int size() const { return std::distance(_begin, _end); }
};

}  // namespace sattools

#endif  // SATTOOLS_SYM_GROUP_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
