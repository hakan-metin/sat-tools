// Copyright 2017 Hakan Metin

#include "sattools/ColoredGraphSymmetryFinder.h"

namespace sat {

static void
on_bliss_automorphim(void* arg, const unsigned int n, const unsigned int* aut) {
    SymmetryFinderInfo *info = static_cast<SymmetryFinderInfo*>(arg);
    unsigned int num_vars = info->num_vars;
    LiteralGraphNodeAdaptor *adaptor = info->adaptor;
    Group *group = info->group;
    std::unique_ptr<Permutation> permutation =
        std::make_unique<Permutation>(num_vars);
    std::vector<bool> seen(n);
    LiteralIndex index;

    for (unsigned int i = 0; i < n; ++i) {
        if (i == aut[i] || seen[i])
            continue;

        index = adaptor->nodeToLiteral(i + 1);
        if (index != kNoLiteralIndex)
            permutation->addToCurrentCycle(Literal(index));

        seen[i] = true;

        for (unsigned int j = aut[i]; j != i; j = aut[j]) {
            seen[j] = true;
            index = adaptor->nodeToLiteral(j + 1);
            if (index != kNoLiteralIndex)
                permutation->addToCurrentCycle(Literal(index));
        }
        permutation->closeCurrentCycle();
    }
    group->addPermutation(std::move(permutation));
}

void BlissColoredGraphSymmetryFinder::addNode(NodeIndex node) {
    unsigned int n;
    while (_num_nodes <= node) {
        n = _graph->add_vertex();
        CHECK_EQ(n, _num_nodes);
        _num_nodes = node + 1;
    }
}

void BlissColoredGraphSymmetryFinder::addEdge(NodeIndex a, NodeIndex b) {
    _graph->add_edge(a, b);
    _num_edges++;
}

void BlissColoredGraphSymmetryFinder::setColor(NodeIndex node,
                                               unsigned int color) {
    CHECK_LT(node, _num_nodes);
    _graph->change_color(node, color);
}


void
BlissColoredGraphSymmetryFinder::findAutomorphisms(unsigned int num_vars,
                        const std::unique_ptr<LiteralGraphNodeAdaptor>& adaptor,
                        Group *group) {
        bliss::Stats stats;
        SymmetryFinderInfo info(num_vars, adaptor.get(), group);

        _graph->find_automorphisms(stats, &on_bliss_automorphim, &info);
        _graph = nullptr;
}

}  // namespace sat
