#include "dpprofits.hpp"

#include "../ub_dantzig/dantzig.hpp"

#define DBG(x)
//#define DBG(x) x

#define INDEX(i,q) (i)*(ub+1) + (q)

Profit opt_dpprofits(const Instance& instance, Profit ub,
		boost::property_tree::ptree* pt, bool verbose)
{
	// Initialize memory table
	ItemIdx n = instance.item_number();
	Weight  c = instance.capacity();
	if (ub == -1)
		ub = ub_dantzig(instance);
	Weight* values = new Weight[ub+1];

	// Compute optimal value
	values[0] = 0;
	for (Profit q=1; q<=ub; ++q)
		values[q] = c+1;
	DBG(for (Profit q=0; q<=ub; ++q)
		std::cout << values[q] << " ";
	std::cout << std::endl;)
	for (ItemIdx i=1; i<=n; ++i) {
		Profit pi = instance.profit(i);
		Weight wi = instance.weight(i);
		for (Profit q=ub+1; q-->0;) {
			Weight w = (q < pi)? wi: values[q-pi] + wi;
			if (w < values[q])
				values[q] = w;
		}
		DBG(for (Profit q=0; q<=ub; ++q)
			std::cout << values[q] << " ";
		std::cout << std::endl;)
	}

	Profit opt = 0;
	for (opt=0; opt<=ub && values[opt]<=c; ++opt);
	opt--;

	if (pt != NULL)
		pt->put("Solution.OPT", opt);
	if (verbose) {
		std::cout << "OPT: " << opt << std::endl;
		std::cout << "EXP: " << instance.optimum() << std::endl;
	}

	delete[] values; // Free memory
	return opt;
}

Solution sopt_dpprofits_1(const Instance& instance, Profit ub,
		boost::property_tree::ptree* pt, bool verbose)
{
	// Initialize memory table
	ItemIdx n = instance.item_number();
	Weight  c = instance.capacity();
	if (ub == -1)
		ub = ub_dantzig(instance);
	ValIdx values_size = (n+1)*(ub+1);
	Weight* values = new Weight[values_size];

	// Compute optimal value
	values[0] = 0;
	for (Profit q=1; q<=ub; ++q)
		values[INDEX(0,q)] = c+1;
	for (ItemIdx i=1; i<=n; ++i) {
		Profit pi = instance.profit(i);
		Profit wi = instance.weight(i);
		for (Profit q=0; q<=ub; ++q) {
			Weight v0 = values[INDEX(i-1,q)];
			Weight v1 = (q < pi)? wi: values[INDEX(i-1,q-pi)] + wi;
			values[INDEX(i,q)] = (v1 < v0)? v1: v0;
		}
	}

	DBG(std::cout << "ub " << ub << std::endl;
	for (Profit q=0; q<=ub; ++q) {
		for (ItemIdx i=0; i<=instance.item_number(); ++i)
			std::cout << values[INDEX(i,q)] << " " << std::flush;
		std::cout << std::endl;
	})

	Profit opt = 0;
	for (opt=0; opt<=ub && values[INDEX(n,opt)]<=c; ++opt);
	opt--;
	DBG(std::cout << "OPT: " << opt << std::endl;)

	// Retrieve optimal solution
	Solution solution(instance);
	ItemIdx i = n;
	Profit  q = opt;
	ValIdx xopt = n*(ub+1) + opt;
	Weight  v = values[xopt];
	while (v > 0) {
		DBG(std::cout << q << " " << v << " " << i << std::endl;)
		Weight wi = instance.weight(i);
		Profit pi = instance.profit(i);
		Weight v0 = values[INDEX(i-1,q)];
		Weight v1 = (q < pi)? instance.capacity() + 1: values[INDEX(i-1,q-pi)] + wi;
		if (v1 < v0) {
			q -= pi;
			v -= wi;
			solution.set(i, true);
		}
		i--;
	}

	delete[] values; // Free memory

	if (pt != NULL)
		pt->put("Solution.OPT", opt);
	if (verbose) {
		std::cout << "OPT: " << opt << std::endl;
		std::cout << "EXP: " << instance.optimum() << std::endl;
	}

	return solution;
}

#undef DBG
