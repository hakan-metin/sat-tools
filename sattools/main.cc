// Copyright 2017 Hakan Metin
// Create Satistics for a CNF problem (Satistics is not a mistake)
#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFStats.h"
#include "sattools/CNFReader.h"
#include "sattools/CNFWriter.h"
#include "sattools/Group.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"
#include "sattools/Trail.h"
#include "sattools/Saucy1Writer.h"

using sat::Assignment;
using sat::Clause;
using sat::CNFStats;
using sat::CNFReader;
using sat::CNFWriter;
using sat::Literal;
using sat::Saucy1Writer;

int main(int argc, char *argv[]) {
    CNFReader<CNFStats> reader;

    CNFStats model;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

// #ifdef USE_GLOG
//     google::InitGoogleLogging(argv[0]);
//     FLAGS_logtostderr = 1;
// #endif

    std::string cnf_filename(argv[1]);

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    LOG(INFO) << "Unary clauses:   " << model.numberOfUnaryClauses();
    LOG(INFO) << "Binary clauses:  " << model.numberOfBinaryClauses();
    LOG(INFO) << "Ternary clauses: " << model.numberOfTernaryClauses();
    LOG(INFO) << "Large clauses:   " << model.numberOfLargeClauses();


    model.printStats();

    return 0;
}
