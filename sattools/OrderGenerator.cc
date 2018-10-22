// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderGenerator.h"

namespace sat {

OrderGenerator::OrderGenerator(const CNFModel& model, const Group& group)
    : _model(model), _group(group) {
    compute_scoring();
}

OrderGenerator::~OrderGenerator() {
}

void OrderGenerator::compute_scoring() {
    std::vector<PermCycleInfo> targets;
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
                unsigned int cycle = perm->literalInCycle(first);
                targets.push_back(PermCycleInfo(idx, cycle));
                touched.insert(idx);
            }
        }

        // Give score
        const double score = 1.0 / targets.size();
        for (PermCycleInfo info : targets) {
            _info_to_scores[info] -= score;  // score is negative (sort greater)
        }
    }
    _scores_to_infos = flip_map(_info_to_scores);

    // Compute occurences for touched permutations
    for (unsigned int idx : touched) {
        const std::unique_ptr<Permutation>& perm = _group.permutation(idx);
        for (const Literal& literal : perm->support()) {
            _occurences[literal]++;
        }
    }
}

std::string OrderGenerator::debugString() const {
    std::stringstream output;

    for (const auto& p : _scores_to_infos) {
        double score = p.first;
        const PermCycleInfo& info = p.second;

        output << score << " : " << info.perm << "  " << info.cycle
               << std::endl;
    }

    return output.str();
}


}  // namespace sat
