// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SAUCY1READER_H_
#define SATTOOLS_SAUCY1READER_H_

#include <memory>
#include <string>
#include <utility>

#include "sattools/Group.h"
#include "sattools/Permutation.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/StreamBuffer.h"

// This class read a symmetry file which look likes:
// [
// (3,5)(4,6)(9,11)(10,12),
// (1,3)(2,4)(7,9)(8,10)
// ]
// And fill it in group if it is a valid permutation (not spurious and size > 0)
// The graph node encoding is DoubleLiteralGraphNodeAdaptor
// look LiteralGraphNodeAdaptor.h for more details.

namespace sat {

class Saucy1Reader {
 public:
    Saucy1Reader();
    ~Saucy1Reader();

    bool load(const std::string& symmetry_filename,
              unsigned int num_vars,
              Group *group);

 private:
    std::unique_ptr<LiteralGraphNodeAdaptor> _adaptor;
};


}  // namespace sat

#endif  // SATTOOLS_SAUCY1READER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
