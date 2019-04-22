#include "lwc_Line.h"
#ifndef LWC_H_EVALUATOR
#define LWC_H_EVALUATOR
class Evaluator
{
	std::vector<Line> linevec;
	int last_eval = 0;

	Evaluator(std::vector<Line> _linevec) {
		linevec = _linevec;
	}

	int Evaluate();

};
#endif