#include "lwc_Evaluator.h"
int Evaluator::Evaluate()
{
	for (Line ln : linevec) {
		if (!ln.request_last)
			ln.func(ln.vars);
		else
		{
			vector<variable> newvars = ln.vars;
			newvars.push_back(variable(new int(last_eval)));
			ln.func(newvars);
		}
	}
	return 0;
}
