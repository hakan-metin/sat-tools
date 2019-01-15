// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_ORDERSCORING_H_
#define SATTOOLS_ORDERSCORING_H_

#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"


namespace sat {

// A comparator class
struct OrderLt {
    const std::vector<int64>& values;

    bool operator() (int i, int j) {
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OrderLt(const std::vector<int64>& v) : values(v) {}
};

// Utility function to store map by value
template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
    return std::pair<B, A>(p.second, p.first);
}
// Utility function to flip map by value
template<typename A, typename B>
std::multimap<B, A> flip_map(const std::map<A, B> &src) {
    std::multimap<B, A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A, B>);
    return dst;
}

struct PermCycleInfo {
 public:
    PermCycleInfo(unsigned int p, unsigned int c) : perm(p), cycle(c) {}
    unsigned int perm;
    unsigned int cycle;

    bool operator<(PermCycleInfo other) const {
        if (other.perm == perm)
            return other.cycle > cycle;
        return other.perm > perm;
    }
};

class OrderScoring {
 public:
    OrderScoring(const CNFModel& model, const Group& group);
    virtual ~OrderScoring();

    void initialize();

    bool hasScore(unsigned int perm, unsigned int cycle) const {
        return _info_to_scores.at(PermCycleInfo(perm, cycle)) < 0;
    }

    Literal minimalOccurence(const PermCycleInfo& info);

    std::multimap<double, PermCycleInfo>::const_iterator begin() const {
        return _scores_to_infos.begin();
    }

    std::multimap<double, PermCycleInfo>::const_iterator end() const {
        return _scores_to_infos.end();
    }

    std::string debugString() const;

 private:
    const CNFModel &_model;
    const Group& _group;

    std::map<PermCycleInfo, double> _info_to_scores;
    std::multimap<double, PermCycleInfo> _scores_to_infos;
    std::map<Literal, int64> _occurences;
};


}  // namespace sat
#endif  // SATTOOLS_ORDERSCORING_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
