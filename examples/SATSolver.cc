// Copyright 2017 Hakan Metin

// #define STRIP_INTERNAL_FLAG_HELP 1
// #define STRIP_FLAG_HELP 1

#include <iostream>
#include <string>
#include <gflags/gflags.h>

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

DEFINE_string(input, "",
              "Required: input file of the problem to solve in cnf format.");

DEFINE_string(proof_file, "",
              "If non-emty, write the proof of the solution to this file.");


DEFINE_bool(use_preprocessor, false,
            "If true, apply a preprocessor to the original cnf file.");

int main(int argc, char *argv[]) {
    CNFReader<CNFModel> reader;
    Solver solver;

    CNFModel model;

    std::string cnf_file;
    std::string proof_file;

    // DratProofHandler drat_proof_handler(false, proof_filename);
    SatParameters parameters;



#ifdef USE_GFLAGS
    gflags::SetUsageMessage("usage message");
    gflags::SetVersionString("1.0.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (FLAGS_input.empty()) {
        LOG(FATAL) << "Please supply a data file with --input=";
    }

    cnf_file   = FLAGS_input;
    proof_file = FLAGS_proof_file;
#else
    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

    cnf_file = std::string(argv[1]);
    proof_file
#endif


#ifdef USE_GLOG
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
#endif

    solver.setParameters(&parameters);

    // solver.setDratProofHandler(&drat_proof_handler);

    if (!reader.load(cnf_file, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_file;

    solver.assign(&model);

    LOG(INFO) << "Number of clauses initial: " << model.numberOfClauses();

    if (FLAGS_use_preprocessor) {
        solver.simplifyInitialProblem();

        LOG(INFO) << "Number of clauses after initial simplification: "
                  << model.numberOfClauses();
    }

    solver.solve();

    return 0;
}
