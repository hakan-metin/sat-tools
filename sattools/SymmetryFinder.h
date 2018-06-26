// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYMMETRYFINDER_H_
#define SATTOOLS_SYMMETRYFINDER_H_

#include <string>

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/Saucy1Reader.h"

namespace sat {

class SymmetryFinder {
 public:
    enum Tool {
        BLISS,
        SAUCY3,
    };
    enum FileEncoding {
        // Look like:
        // [
        // (3,5)(4,6)(9,11)(10,12),
        // (1,3)(2,4)(7,9)(8,10)
        // ]
        SAUCY1
    };

    explicit SymmetryFinder(const CNFModel& model);
    ~SymmetryFinder();

    bool computeAutomorphism(Tool tool, Group *group);
    bool loadAutomorphism(FileEncoding encoding,
                          const std::string& symmetry_filename,
                          Group *group);

 private:
    const CNFModel& _model;
};

}  // namespace sat

#endif  // SATTOOLS_SYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
