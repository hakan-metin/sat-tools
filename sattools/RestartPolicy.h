// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_RESTARTPOLICY_H_
#define SATTOOLS_RESTARTPOLICY_H_

namespace sat {
class RestartPolicy {
 public:
    RestartPolicy() {}
    virtual ~RestartPolicy() {}

    virtual bool shouldRestart() = 0;
    virtual void onConflict() {}
};

}  // namespace sat

#endif  // SATTOOLS_RESTARTPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
