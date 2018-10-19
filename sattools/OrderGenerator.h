// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_ORDERGENERATOR_H_
#define SATTOOLS_ORDERGENERATOR_H_

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
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}
// Utility function to flip map by value
template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}

struct PermScoreInfo {
 public:
    PermScoreInfo(unsigned int p, unsigned int c) : num_perm(p), num_cycle(c) {}
    unsigned int num_perm;
    unsigned int num_cycle;

    bool operator<(PermScoreInfo other) const {
        if (other.num_perm == num_perm)
            return other.num_cycle > num_cycle;
        return other.num_perm > num_perm;
    }
};

class OrderGenerator {
 public:
    OrderGenerator(const CNFModel& model, const Group& group);
    virtual ~OrderGenerator();

    void createOrder();

    std::string debugString() const;

 private:
    const CNFModel &_model;
    const Group& _group;

    std::unique_ptr<Order> _order;

    std::multimap<double, PermScoreInfo> _scores;
    std::map<Literal, int64> _occurences;


    void compute_scoring();
};


}  // namespace sat
#endif  // SATTOOLS_ORDERGENERATOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
