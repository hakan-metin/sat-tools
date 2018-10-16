// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

Breaker::Breaker() {
    _order = std::make_unique<Order>();
}

Breaker::~Breaker() {
}


struct OrderLt {
    const std::vector<int64>& values;

    bool operator() (int i, int j) {
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OrderLt(const std::vector<int64>& v) : values(v) {}
};

void Breaker::addUnits(CNFModel& model, const Group& group) {
    // Score is calculated with number of binary clauses included
    std::vector<int64> perm_score(group.numberOfPermutations(), 0);

    for (const std::unique_ptr<Clause>& clause : model.binaryClauses()) {
        Literal first = clause->literals()[0];
        Literal second = clause->literals()[1];

        // Register binary clauses for resolution
        _resolutions[first].insert(second);
        _resolutions[second].insert(first);


        for (int idx : group.watch(first)) {
            const std::unique_ptr<Permutation>& perm = group.permutation(idx);

            // Detect if second literal is in same cycle
            Literal literal = perm->imageOf(first);
            bool isInSameCycle = false;
            while (literal != first) {
                if (literal == second) {
                    isInSameCycle = true;
                    break;
                }
                literal = perm->imageOf(literal);
            }

            if (isInSameCycle) {
                perm_score[idx]++;
            }
        }
    }

    // Sort scores
    std::vector<int64> indexes(perm_score.size());
    for (unsigned int i=0; i<perm_score.size(); i++)
        indexes[i] = i;

    std::sort(indexes.begin(), indexes.end(), OrderLt(perm_score));

    std::cout << "," << perm_score[indexes[0]] << std::endl;

    // for (int idx : indexes)
    //     LOG(INFO) << idx;


}

}  // namespace sat
