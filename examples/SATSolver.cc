// Copyright 2017 Hakan Metin

// #define STRIP_INTERNAL_FLAG_HELP 1
// #define STRIP_FLAG_HELP 1

#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/CNFWriter.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"
#include "sattools/Trail.h"
#include "sattools/Propagator.h"
#include "sattools/Solver.h"
#include "sattools/DratProofHandler.h"

using sat::Assignment;
using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::CNFWriter;
using sat::Literal;
using sat::Trail;
using sat::Propagator;
using sat::Solver;
using sat::DratProofHandler;


int main(int argc, char *argv[]) {
    CNFReader<CNFModel> reader;
    Solver solver;

    CNFModel model;

    std::string cnf_file;
    std::string proof_file;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

    cnf_file = std::string(argv[1]);

    // solver.setDratProofHandler(&drat_proof_handler);

    if (!reader.load(cnf_file, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_file;

    solver.assign(&model);

    LOG(INFO) << "Number of clauses initial: " << model.numberOfClauses();

    solver.solve();

    return 0;
}
