
#include "sattools/BreakerInfo.h"

namespace sat {

BreakerInfo::BreakerInfo(const std::unique_ptr<Permutation>&permutation,
                         const Assignment& assignment) :
    _permutation(permutation),
    _assignment(assignment),
    _lookup_index(0) {
}

BreakerInfo::~BreakerInfo() {
}



}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
