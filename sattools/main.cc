// Copyright 2017 Hakan Metin

#include <iostream>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/RangeIterator.h"
#include "sattools/StreamBuffer.h"

using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::Literal;

int main() {
    CNFReader reader;
    CNFModel model;

    reader.load("tests/resources/teast.cnf", &model);
    LOG(INFO) << model.numberOfVariables() << " " << model.numberOfClauses();

    for (const std::unique_ptr<Clause>& clause : model.clauses()) {
        for (const Literal& l : *clause) {
            std::cout << l.signedValue() << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}
