// Copyright 2017 Hakan Metin

#include "sattools/ColoredGraphSymmetryFinder.h"

namespace sat {

#ifdef USE_BLISS

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

#endif  // USE_BLISS



#ifdef USE_SAUCY

static int
on_saucy_automorphism(int n, const int *aut, int k ATTRIBUTE_UNUSED,
                      int *support ATTRIBUTE_UNUSED, void *arg) {
    SymmetryFinderInfo *info = static_cast<SymmetryFinderInfo*>(arg);
    unsigned int num_vars = info->num_vars;
    LiteralGraphNodeAdaptor *adaptor = info->adaptor;
    Group *group = info->group;
    std::unique_ptr<Permutation> permutation =
        std::make_unique<Permutation>(num_vars);
    std::vector<bool> seen(n);
    LiteralIndex index;

    for (int i = 0; i < n; ++i) {
        if (i == aut[i] || seen[i])
            continue;

        index = adaptor->nodeToLiteral(i + 1);
        if (index != kNoLiteralIndex)
            permutation->addToCurrentCycle(Literal(index));

        seen[i] = true;

        for (int j = aut[i]; j != i; j = aut[j]) {
            seen[j] = true;
            index = adaptor->nodeToLiteral(j + 1);
            if (index != kNoLiteralIndex)
                permutation->addToCurrentCycle(Literal(index));

            // index = LiteralIndex(node2Literal(j, num_vars));
            // if (index != kNoLiteralIndex)
            //     permutation->addToCurrentCycle(Literal(index));
        }
        permutation->closeCurrentCycle();
    }

    group->addPermutation(std::move(permutation));
    return 1;  // Always continue to search
}

SaucyColoredGraphSymmetryFinder::SaucyColoredGraphSymmetryFinder(
             unsigned int num_nodes) : ColoredGraphSymmetryFinder(num_nodes) {
    _colors.resize(_num_nodes);
    _graph.resize(_num_nodes);
}


void SaucyColoredGraphSymmetryFinder::addNode(NodeIndex node) {
    if (node > _num_nodes) {
        _num_nodes = node + 1;
        _colors.resize(_num_nodes);
        _graph.resize(_num_nodes);
    }
}

void SaucyColoredGraphSymmetryFinder::addEdge(NodeIndex a, NodeIndex b) {
    CHECK_EQ(_graph.size(), _num_nodes);
    CHECK_LT(a, _num_nodes);
    CHECK_LT(b, _num_nodes);
    CHECK_GE(a, 0);
    CHECK_GE(b, 0);

    _graph[a].push_back(b);
    _graph[b].push_back(a);

    _num_edges++;
}

void SaucyColoredGraphSymmetryFinder::setColor(NodeIndex node,
                                               unsigned int color) {
    _colors[node] = color;
}


void
SaucyColoredGraphSymmetryFinder::findAutomorphisms(unsigned int num_vars,
                        const std::unique_ptr<LiteralGraphNodeAdaptor>& adaptor,
                        Group *group) {
    SymmetryFinderInfo info(num_vars, adaptor.get(), group);

    int n = _num_nodes;
    int e = _num_edges;
    std::vector<int> adj;
    std::vector<int> edg;

    adj.push_back(0);
    unsigned int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += _graph[i].size();
        adj.push_back(sum);
    }

    for (int i = 0; i < n; i++)
        for (const unsigned int& x : _graph[i])
            edg.push_back(x);

    // Initialize saucy structure
    struct saucy *s = reinterpret_cast<struct saucy*>(saucy_alloc(n));
    struct saucy_graph  *g = reinterpret_cast<struct saucy_graph*>
        (malloc(sizeof(struct saucy_graph)));

    g->n = n;
    g->e = e;
    g->adj = adj.data();
    g->edg = edg.data();

    struct saucy_stats stats;
    saucy_search(s, g, 0, _colors.data(), on_saucy_automorphism,
                 static_cast<void*>(&info), &stats);
    free(g);
    saucy_free(s);
}

#endif  // USE_SAUCY


}  // namespace sat
