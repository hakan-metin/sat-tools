// Copyright 2017 Hakan Metin - LIP6

#include "sattools/SymmetryFinder.h"

namespace sat {


SymmetryFinder::SymmetryFinder(const CNFModel& model) : _model(model) {
}

SymmetryFinder::~SymmetryFinder() {
}


bool SymmetryFinder::computeAutomorphism(Tool tool, Group *group) {
    switch (tool) {
    case BLISS:

        return false;
    }

    return false;
}


bool SymmetryFinder::loadAutomorphism(FileEncoding encoding,
                                      const std::string& symmetry_filename,
                                      Group *group) {
    const unsigned int num_vars = _model.numberOfVariables();
    switch (encoding) {
    case SAUCY1:
        Saucy1Reader saucy1Reader;
        return saucy1Reader.load(symmetry_filename, num_vars, group);
    }

    return false;
}


}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
