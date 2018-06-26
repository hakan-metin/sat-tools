// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Saucy1Reader.h"

namespace sat {

Saucy1Reader::Saucy1Reader() : _adaptor(nullptr) {
}

Saucy1Reader::~Saucy1Reader() {
}


bool Saucy1Reader::load(const std::string& symmetry_filename,
          unsigned int num_vars,
          Group *group) {
    std::unique_ptr<Permutation> generator;
    StreamBuffer in(symmetry_filename);
    int parsed;
    LiteralIndex lit_index;
    Literal lit;

    _adaptor = std::unique_ptr<LiteralGraphNodeAdaptor>
        (new DoubleLiteralGraphNodeAdaptor(num_vars));

    CHECK_EQ(*in, '[');
    ++in;
    CHECK_EQ(*in, '\n');
    ++in;

    if (*in == ']')  // No generator exists
        return true;

    while (*in != '\n') {
        CHECK_EQ(*in, '(');
        generator = std::unique_ptr<Permutation>(new Permutation(num_vars));

        while (*in == '(') {
            ++in;
            do {
                parsed = in.readInt();
                lit_index = _adaptor->nodeToLiteral(parsed);
                if (lit_index != kNoLiteralIndex) {
                    lit = Literal(lit_index);
                    generator->addToCurrentCycle(lit);
                }
                if (*in != ')') {
                    CHECK_EQ(*in, ',');
                    ++in;
                }
            } while (*in != ')');
            CHECK_EQ(*in, ')');
            generator->closeCurrentCycle();
            ++in;
        }
        group->addPermutation(std::move(generator));
        /* End of Generator line */
        if (*in == ',') {
            ++in;
            CHECK_EQ(*in, '\n');
            ++in;
        }
    }
    ++in;
    CHECK_EQ(*in, ']');

    return true;
}

}  // namespace sat
