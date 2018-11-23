// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderScoring.h"

namespace sat {

OrderScoring::OrderScoring(const CNFModel& model, const Group& group)
    : _model(model), _group(group) {
}

OrderScoring::~OrderScoring() {
}

void OrderScoring::initialize() {
    std::vector<PermCycleInfo> targets;
    std::unordered_set<unsigned int> touched;

    for (Clause *clause : _model.clauses()) {
        if (clause->size() != 2)
            continue;

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

    // Compute occurences for touched permutations
    for (unsigned int idx : touched) {
        const std::unique_ptr<Permutation>& perm = _group.permutation(idx);
        for (const Literal& literal : perm->support()) {
            _occurences[literal]++;
        }
    }

    std::vector<PermCycleInfo> infos;
    for (const auto& pair : _info_to_scores)
        infos.push_back(pair.first);

    for (const PermCycleInfo info : infos) {
        int sum = 0;
        unsigned int idx = info.perm;
        unsigned int c = info.cycle;

        const std::unique_ptr<Permutation>& perm = _group.permutation(idx);
        for (Literal literal : perm->cycle(c))
            sum += _occurences[literal];

        _info_to_scores[info] += sum / 1000.0;
    }

    _scores_to_infos = flip_map(_info_to_scores);
}

Literal OrderScoring::minimalOccurence(const PermCycleInfo& info) {
    unsigned int perm = info.perm;
    unsigned int cycle = info.cycle;

    const std::unique_ptr<Permutation>& p = _group.permutation(perm);
    Literal literal = p->lastElementInCycle(cycle);

    for (Literal image : p->cycle(cycle))
        if (_occurences[image] < _occurences[literal])
            literal = image;

    return literal;
}

std::string OrderScoring::debugString() const {
    std::stringstream output;

    output << std::endl << "== SCORES" << std::endl;
    for (const auto& p : _scores_to_infos) {
        double score = p.first;
        const PermCycleInfo& info = p.second;

        output << score << " : " << info.perm << "  " << info.cycle << " ( ";
        // Display also cycle
        unsigned int index = info.perm;
        const std::unique_ptr<Permutation>& perm = _group.permutation(index);
        for (Literal l : perm->cycle(info.cycle))
            output << l.debugString() << " ";
        output << ")" << std::endl;
    }

    output << std::endl << "== OCCURENCES" << std::endl;

    // Occurences
    for (const auto& pair : _occurences) {
        Literal l = pair.first;
        if (l.isNegative())
            continue;
        output << l.debugString() << " : " << pair.second << std::endl;
    }

    return output.str();
}

}  // namespace sat