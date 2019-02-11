
// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CLAUSE_H_
#define SATTOOLS_CLAUSE_H_

#include <string>
#include <vector>

#include "sattools/Literal.h"
#include "sattools/Logging.h"

namespace sat {

class Clause {
 public:
    static Clause* create(const std::vector<Literal>& literals,
                          bool is_redundant);

    // Non-sized delete because this is a tail-padded class.
    void operator delete(void* p) {
        ::operator delete(p);  // non-sized delete
    }

    // Allows for range based iteration: for (Literal literal : clause) {}.
    const Literal* begin() const { return &(_literals[0]); }
    const Literal* end() const { return &(_literals[_size]); }
    Literal* literals() { return &(_literals[0]); }

    //
    void removeLiteral(Literal *to_remove);

    // Getters
    int  size()         const { return _size;        }
    bool is_redundant() const { return _is_redundant; }

    // Clause is no more usable after.
    void lazyDetach() { _size = 0; }

    std::string debugString() const;

 private:
    // Packed data, use 32 bits
    bool _is_redundant : 2;  // true => leanrt, false => problem clause
    unsigned int _size : 30;

    // This class store the literals inline, and _literals mark the starts of
    // the variable length portion.
    Literal _literals[0];

    DISALLOW_COPY_AND_ASSIGN(Clause);
};

}  // namespace sat

#endif  // SATTOOLS_CLAUSE_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
