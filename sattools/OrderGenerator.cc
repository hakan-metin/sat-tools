// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderGenerator.h"

namespace sat {

OrderGenerator::OrderGenerator(const CNFModel& model, const Group& group)
    : _model(model), _group(group), _order(nullptr) {
    compute_scoring();
}

OrderGenerator::~OrderGenerator() {
}

void OrderGenerator::compute_scoring() {

    std::map<PermScoreInfo, double> scores;
    std::vector<PermScoreInfo> targets;
    std::unordered_set<unsigned int> touched;

    for (const std::unique_ptr<Clause>& clause : _model.binaryClauses()) {
        Literal first = clause->literals()[0];
        Literal second = clause->literals()[1];

        targets.clear();
        for (int idx : _group.watch(first)) {
            const std::unique_ptr<Permutation>& perm = _group.permutation(idx);

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
                unsigned int num_cycle = perm->literalInCycle(first);
                targets.push_back(PermScoreInfo(idx, num_cycle));
                touched.insert(idx);
            }
        }

        // Give score
        const double score = 1.0 / targets.size();
        for (PermScoreInfo info : targets) {
            scores[info] -= score;  // score is negative to sort greater
        }
    }
    _scores = flip_map(scores);

    // Compute occurences for touched permutations
    for (unsigned int idx : touched) {
        const std::unique_ptr<Permutation>& perm = _group.permutation(idx);
        for (const Literal& literal : perm->support()) {
            if (literal.isNegative())
                continue;
            _occurences[literal]++;
        }
    }
}

void OrderGenerator::createOrder() {

    _order = std::make_unique<Order>();

    std::unordered_set<unsigned int> used;

    for (auto p : _scores) {
        PermScoreInfo info = p.second;
        unsigned int num_perm = info.num_perm;
        unsigned int num_cycle = info.num_cycle;

        if (!used.insert(num_perm).second)
            continue;

        for (Literal l : _group.permutation(num_perm)->cycle(num_cycle)) {
            if (l.isNegative())
                l = l.negated();

            _order->add(l);
        }
        LOG(INFO) << _order->debugString();
    }
    LOG(INFO) << _order->debugString();
}


std::string OrderGenerator::debugString() const {
    std::stringstream output;

    for (const auto& p : _scores) {
        double score = p.first;
        const PermScoreInfo& info = p.second;

        output << score << " : " << info.num_perm << "  " << info.num_cycle
               << std::endl;
    }

    return output.str();
}


}  // namespace sat
