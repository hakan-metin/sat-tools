// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Simplifier.h"

namespace sat {

Simplifier::Simplifier(const Group &group, CNFModel *model) : _group(group),
                                                              _model(model) {
    const unsigned int num_vars = model->numberOfVariables();

    _assignment.resize(num_vars);
}

Simplifier::~Simplifier() {
}





}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
