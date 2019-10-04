// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_NORESTARTPOLICY_H_
#define SATTOOLS_NORESTARTPOLICY_H_

#include "sattools/RestartPolicy.h"

namespace sat {

class NoRestartPolicy : public RestartPolicy {
 public:
    NoRestartPolicy() : RestartPolicy() {}

    virtual ~NoRestartPolicy() {}

    bool shouldRestart() override {
        return false;
    }
};

}  // namespace sat

#endif  // SATTOOLS_NORESTARTPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
