#pragma once

#include "knapsack/lib/instance.hpp"
#include "knapsack/lib/solution.hpp"

namespace knapsack
{

struct MinknapParams
{
    StateIdx lb_greedy = 0;
    StateIdx lb_greedynlogn = -1;
    StateIdx lb_pairing = -1;
    StateIdx ub_surrogate = -1;
    StateIdx solve_sur = -1;
};

Profit opt_minknap_list(Instance& ins,
        MinknapParams params = MinknapParams(), Info* info = NULL);
Solution sopt_minknap_list_part(Instance& ins,
        MinknapParams params = MinknapParams(), ItemPos k = 64, Info* info = NULL, Profit o = -1);
Solution sopt_minknap_list_part(Instance& ins,
        MinknapParams params = MinknapParams(), Info* info = NULL, Profit o = -1);

}

