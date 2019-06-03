#include "lwc_typedefs.h"
#include "lwc_Parser.h"
#define H_LWC_EVALUATOR
#ifndef H_LWC_EVALUATOR

namespace lwc {
	struct Evaluator {
		long last_eval = 0;

		Evaluator() {};

		const long& evaluate(std::vector<Line>& linevec)
		{

			for (Line& ln : linevec) {
				if (ln.request_last) {
					*(ln.vars)[ln.getN() - 1] = last_eval; //set last element to the result of last operation
				}
				last_eval = ln.func(ln.vars);
				if (ln.linked_lines != nullptr && ln.linked_lines->size() > 0 && last_eval) {

					if (ln.loop) {
						while (ln.func(ln.vars)) {
							evaluate((*ln.linked_lines));
						}
					}
					else {
						evaluate((*ln.linked_lines));
					}
				}
			}
			return last_eval;
		}

		const long& single_eval(Line& ln) {
			if (ln.request_last) {
				*(ln.vars)[ln.getN() - 1] = last_eval; //set last element to the result of last operation
			}
			last_eval = ln.func(ln.vars);
			if (ln.linked_lines != nullptr && ln.linked_lines->size() > 0 && last_eval) {

				if (ln.loop) {
					while (ln.func(ln.vars)) {
						evaluate((*ln.linked_lines));
					}
				}
				else {
					evaluate((*ln.linked_lines));
				}
			}
			return last_eval;
		}
	};
}
#endif