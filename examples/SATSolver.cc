// Copyright 2017 Hakan Metin

#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/CNFWriter.h"
#include "sattools/Group.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"
#include "sattools/Trail.h"
#include "sattools/Saucy1Writer.h"
#include "sattools/Propagator.h"
#include "sattools/Solver.h"
#include "sattools/DratProofHandler.h"
#include "sattools/SatParameters.pb.h"

using sat::Assignment;
using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::CNFWriter;
using sat::Literal;
using sat::Saucy1Writer;
using sat::Trail;
using sat::Propagator;
using sat::Solver;
using sat::DratProofHandler;

int main(int argc, char *argv[]) {
    CNFReader<CNFModel> reader;
    Solver solver;

    CNFModel model;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

#ifdef USE_GLOG
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
#endif

    std::string cnf_filename(argv[1]);
    std::string proof_filename = cnf_filename + ".proof";
    DratProofHandler drat_proof_handler(false, proof_filename);
    SatParameters parameters;

    // solver.setParameters(parameters);
    solver.setDratProofHandler(&drat_proof_handler);

    // std::string symmetry_filename = cnf_filename + ".txt";

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    solver.assign(&model);

    LOG(INFO) << "Number of clauses initial: " << model.numberOfClauses();

    // solver.simplifyInitialProblem();

    LOG(INFO) << "Number of clauses after initial simplification: "
              << model.numberOfClauses();

    solver.solve();

    return 0;
}
