// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYMMETRYFINDER_H_
#define SATTOOLS_SYMMETRYFINDER_H_

#include <memory>
#include <string>
#include <vector>
#include <limits>

#include "sattools/CNFModel.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/Group.h"

namespace sat {

template<typename Graph, typename Adaptor>
class SymmetryFinder : private Graph {
    using Graph::addNode;
    using Graph::findAutomorphisms;

 public:
    SymmetryFinder() {}
    ~SymmetryFinder() {}

    void findAutomorphisms(const CNFModel& model, Group *group);

 private:
    unsigned int _num_vars;
    std::unique_ptr<Graph> _graph;
    std::unique_ptr<LiteralGraphNodeAdaptor> _adaptor;

    void buildGraph(const CNFModel& model);
};

template<typename Graph, typename Adaptor>
inline void SymmetryFinder<Graph, Adaptor>::buildGraph(const CNFModel& model) {
    const bool verbose = false;
    const unsigned int num_vars = model.numberOfVariables();
    const unsigned int num_nodes = num_vars * 2 + model.numberOfClauses() -
        model.numberOfBinaryClauses();
    unsigned int clause_node = num_vars * 2;
    unsigned int x, y;

    _num_vars = num_vars;
    _graph = std::make_unique<Graph>(num_nodes);
    _adaptor = std::make_unique<Adaptor>(num_vars);

    std::vector<bool> seen(num_vars, false);

    // Boolean consistency
    for (BooleanVariable var(0); var < num_vars; ++var) {
        Literal l = Literal(var, false);
        x = _adaptor->literalToNode(l) - 1;
        y = _adaptor->literalToNode(l.negated()) - 1;

        if (verbose)
            LOG(INFO) << x << " " << y;
        _graph->addEdge(x, y);
    }

    for (Clause *clause : model.clauses()) {
        if (clause->size() == 2) {
            const Literal first = clause->literals()[0];
            const Literal second = clause->literals()[1];

            x = _adaptor->literalToNode(first) - 1;
            y = _adaptor->literalToNode(second) - 1;
            if (verbose)
                LOG(INFO) << x << " " << y;
            _graph->addEdge(x, y);

            x = first.variable().value();
            y = second.variable().value();

            seen[x] = true;
            seen[y] = true;
        } else {
            for (const Literal& literal : *clause) {
                x =  _adaptor->literalToNode(literal) - 1;

                if (verbose)
                    LOG(INFO) << x << " " << clause_node;
                _graph->addEdge(x, clause_node);

                x = literal.variable().value();
                seen[x] = true;
            }
            clause_node++;
        }
    }

    CHECK_EQ(clause_node, num_nodes);

    // Change color of clauses
    const unsigned int kClauseColor = 1;
    for (clause_node = num_vars * 2; clause_node < num_nodes; clause_node++)
        _graph->setColor(clause_node, kClauseColor);

    // Change color unused nodes
    int64 color = kClauseColor + 1;
    for (unsigned int i = 0; i < seen.size(); i++) {
        if (seen[i])
            continue;

        Literal literal(BooleanVariable(i), true);
        x =  _adaptor->literalToNode(literal) - 1;
        _graph->setColor(x, color++);
    }
    CHECK_LT(color, std::numeric_limits<int32>::max());
}

template<typename Graph, typename Adaptor>
void SymmetryFinder<Graph, Adaptor>::findAutomorphisms(const CNFModel& model,
                                                       Group *group) {
    buildGraph(model);
    _graph->findAutomorphisms(_num_vars, _adaptor, group);
}

}  // namespace sat

#endif  // SATTOOLS_SYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
