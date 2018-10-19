// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderGenerator.h"

namespace sat {

OrderGenerator::OrderGenerator(const CNFModel& model, const Group& group)
    : _model(model), _group(group) {
    _order = std::make_unique<Order>();
}

OrderGenerator::~OrderGenerator() {
}




void OrderGenerator::score2() {
    unsigned int n = _group.numberOfPermutations();
    std::unordered_set<unsigned int> touched;

    for (const std::unique_ptr<Clause>& clause : _model.binaryClauses()) {
        Literal first = clause->literals()[0];
        Literal second = clause->literals()[1];

        for (int idx : _group.watch(first)) {
            if (touched.find(idx) != touched.end())
                continue;

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

            if (isInSameCycle)
                touched.insert(idx);

            if (touched.size() == n)
                break;
        }
    }

    std::map<Literal, int64> occurence_generators;

    for (unsigned int i=0; i<n; i++) {
        if (touched.find(i) == touched.end())
            continue;
        const std::unique_ptr<Permutation>& perm = _group.permutation(i);
        LOG(INFO) << "[" << i << "]"  << perm->debugString();
    }

    for (unsigned int client : touched) {
        const std::unique_ptr<Permutation>& perm = _group.permutation(client);
        for (const Literal& literal : perm->support()) {
            if (literal.isNegative())
                continue;
            occurence_generators[literal]++;
        }
    }

    for (auto p : occurence_generators) {
        LOG(INFO) << p.first.signedValue() << " " << p.second;
    }

}

template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

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
            return other.num_cycle < num_cycle;
        return other.num_perm < num_perm;
    }

};

void OrderGenerator::score_per_cycle() {
    unsigned int n = _group.numberOfPermutations();

    std::map<PermScoreInfo, double> scores;
    // PermScoreInfo i(0, 0);
    // PermScoreInfo j(1, 1);

    // s[i] = -32.0;
    // s[j] = -322.0;

    // std::multimap<double, PermScoreInfo> dst = flip_map(s);

    // for (auto d : dst)
    //     LOG(INFO) << d.first << " : "
    //               << d.second.num_perm << "  " << d.second.num_cycle;

    // exit(0);

    // // Allocate Score Matrix
    // std::vector< std::vector<double> > scores(n);
    // for (unsigned int i=0; i<n; i++) {
    //     unsigned int num_cycles = _group.permutation(i)->numberOfCycles();
    //     scores.at(i).resize(num_cycles, 0.0);
    // }

    std::vector<PermScoreInfo> targets;

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
            }
        }

        // Give score
        const double score = 1.0 / targets.size();
        for (PermScoreInfo info : targets) {
            scores[info] -= score;
        }
    }

    std::multimap<double, PermScoreInfo> r_scores = flip_map(scores);

    for (auto p : r_scores) {
        double score = p.first;
        PermScoreInfo& info = p.second;

        LOG(INFO) << score << " : " << info.num_perm << "  " << info.num_cycle;
    }
}



void OrderGenerator::score() {
    unsigned int n = _group.numberOfPermutations();

    // Score is calculated with number of binary clauses included
    std::vector<int64> perm_score(n, 0);

    std::vector<unsigned int> clients;
    std::vector<double> clients_score(n, 0);

    for (const std::unique_ptr<Clause>& clause : _model.binaryClauses()) {
        Literal first = clause->literals()[0];
        Literal second = clause->literals()[1];

        clients.clear();

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
                clients.push_back(idx);
                perm_score[idx]++;
            }
        }

        // Give score
        const double score = 1.0 / clients.size();
        for (unsigned int client : clients) {
            clients_score[client] += score;
            if (score == 1.0)
                LOG(INFO) << "Only " <<  client << " unary " <<
                    " for the clause " << clause->debugString();
        }
    }

    for (unsigned int i=0; i<perm_score.size(); i++)
        if (perm_score[i] > 0)
            LOG(INFO) << "[" << i << "] " << perm_score[i] << "  "
                      << clients_score[i];

    return;

    // Sort scores
    std::vector<int64> indexes(perm_score.size());
    for (unsigned int i=0; i<perm_score.size(); i++)
        indexes[i] = i;

    std::sort(indexes.begin(), indexes.end(), OrderLt(perm_score));

    std::cout << "," << perm_score[indexes[0]] << std::endl;

    for (int idx : indexes) {
        LOG(INFO) << _group.permutation(idx)->debugString();
        LOG(INFO) << idx << " : " << perm_score[indexes[idx]];
    }


}

}  // namespace sat
