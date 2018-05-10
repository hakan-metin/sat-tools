#include <iostream>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"

using namespace sat;

int main() {

    CNFReader reader;
    CNFModel model;

    reader.load("test.cnf", &model);

    LOG(INFO) << model.numberOfVariables() << " " << model.numberOfClauses();
    return 0;
}
