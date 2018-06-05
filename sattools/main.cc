// Copyright 2017 Hakan Metin

#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/RangeIterator.h"
#include "sattools/StreamBuffer.h"
#include "sattools/ColoredGraph.h"

using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::Literal;

int main(int argc, char *argv[]) {
    CNFReader reader;
    CNFModel model;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

    std::string cnf_filename(argv[1]);

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    LOG(INFO) << model.numberOfVariables() << " " << model.numberOfClauses();
    for (const std::unique_ptr<Clause>& clause : model.clauses())
        std::cout << clause->debugString() << std::endl;

    return 0;
}
