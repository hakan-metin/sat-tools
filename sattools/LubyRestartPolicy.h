// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_LUBYRESTARTPOLICY_H_
#define SATTOOLS_LUBYRESTARTPOLICY_H_

#include "sattools/Bitset.h"
#include "sattools/RestartPolicy.h"

namespace sat {

// Returns the ith element of the strategy S^univ proposed by M. Luby et al. in
// Optimal Speedup of Las Vegas Algorithms, Information Processing Letters 1993.
// This is used to decide the number of conflicts allowed before the next
// restart. This method, used by most SAT solvers, is usually referenced as
// Luby.
// Returns 2^{k-1} when i == 2^k - 1
//    and  SUniv(i - 2^{k-1} + 1) when 2^{k-1} <= i < 2^k - 1.
// The sequence is defined for i > 0 and starts with:
//   {1, 1, 2, 1, 1, 2, 4, 1, 1, 2, 1, 1, 2, 4, 8, ...}
inline int SUniv(int i) {
    DCHECK_GT(i, 0);
    while (i > 2) {
        const int most_significant_bit_position =
            MostSignificantBitPosition64(i + 1);
        if ((1 << most_significant_bit_position) == i + 1) {
            return 1 << (most_significant_bit_position - 1);
        }
        i -= (1 << most_significant_bit_position) - 1;
    }
    return 1;
}

constexpr int RESTART_PERIOD = 100;

class LubyRestartPolicy : public RestartPolicy {
 public:
    LubyRestartPolicy() : RestartPolicy(),
                          _luby_count(0),
                          _conflicts_until_next_restart(RESTART_PERIOD) {}

    virtual ~LubyRestartPolicy() {}

    bool shouldRestart() override {
        bool should_restart = false;

        if (_conflicts_until_next_restart == 0) {
            should_restart = true;
            _luby_count++;
        }

        if (should_restart) {
            _conflicts_until_next_restart  = RESTART_PERIOD;
            _conflicts_until_next_restart *= SUniv(_luby_count + 1);
        }

        return should_restart;
    }

    void onConflict() override {
        if (_conflicts_until_next_restart > 0)
            _conflicts_until_next_restart--;
    }

 private:
    int _luby_count;
    int _conflicts_until_next_restart;
};



}  // namespace sat


#endif  // SATTOOLS_LUBYRESTARTPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
