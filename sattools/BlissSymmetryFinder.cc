// Copyright 2017 Hakan Metin - LIP6

#include "sattools/BlissSymmetryFinder.h"

namespace sat {

BlissSymmetryFinder::BlissSymmetryFinder() : _adaptor(nullptr) {
}

BlissSymmetryFinder::~BlissSymmetryFinder() {
}


bool BlissSymmetryFinder::findAutomorphism(const CNFModel& model, Group *group) {
    unsigned int x, y;
    unsigned int num_vars = model.numberOfVariables();
    _adaptor = std::unique_ptr<LiteralGraphNodeAdaptor>
        (new DoubleLiteralGraphNodeAdaptor(num_vars));

    const unsigned int num_nodes = model.numberOfVariables() * 2 +
        model.numberOfUnaryClauses() +
        model.numberOfTernaryClauses() +
        model.numberOfLargeClauses();

    std::unique_ptr<bliss::Graph> graph(new bliss::Graph(num_nodes));
    bliss::Stats stats;

    // Boolean Consistency edges
    for (BooleanVariable var(0); var < n; ++var) {
        x = _adaptor->literalToNode(Literal(var, true));
        y = _adaptor->literalToNode(Literal(var, true));

        graph->add_edge(x, y);
    }



    return true;
}

}  // namespace sat
