#include "lwc_typedefs.h"
#include "lwc_Parser.h"
#include <tuple>
#include <map>
#ifndef H_LWC_USERFUNCS
#define H_LWC_USERFUNCS
namespace lwc {
	// Function class must be threadsafe in so far as calling the same user function contemporaneously behaves as expected
	
	//Two tasks are necessary for thread-safe functions
	//First, the scope must be duplicated
	// 

	typedef std::tuple<unsigned int, unsigned int> VarPos;

	class UserFunctionTemplate { 
		/* "template" userfunction. 
		all Userfunction Envs will copy it's scope and rebuild the vector<LAST>
		using newly allocated addresses for any local variables.
		hmm...
		functions could be compiled differently?
		*/
		CodeBlock templ;
		std::vector<std::vector<VarPos>> lvar_positions;
		UserFunctionTemplate(CodeBlock& bf, std::vector<void*> func_args) : templ(bf){
			for (unsigned int lidx  = 0; lidx < bf.size(); ++lidx) {
				LAST& last = bf[lidx];
				std::map<variable*, std::vector<VarPos>> lvar_map;

				for (unsigned int idx = 0; idx < last.breadthwise.size(); ++idx) {
					auto lnp = last.breadthwise[idx];
					if (lnp->lvar) {
						lvar_map[lnp->lvar].push_back({lidx, idx});
					}
				}
			}
		}

		variable* basic_call() {
			CodeBlock newcb = templ;
			std::vector<void*> garbage;
			for (auto vpos_vec : lvar_positions) {
				auto lookup_lambda = [&vpos_vec, this](const VarPos& vp) {return this->templ[std::get<0>(vp)].breadthwise[std::get<1>(vp)];};
				auto lk = lookup_lambda(vpos_vec[0]);
				variable value = *lk->lvar;
				garbage.push_back(lk);
				garbage.emplace_back(lk->rt->allocate());
				variable newspace = (variable)garbage.back();
				memcpy(newspace, value, lk->rt->size());
				for (auto vpos : vpos_vec) {
					*lookup_lambda(vpos)->lvar = newspace;
				}
			}
			evaluate_lines(newcb);
			for (auto pp : garbage) {
				delete pp;
			}
		}
	};



	// Every userfunction will have to essentially copy all the LASTS and change local variables in the last to point to new locations
	struct UserFunctionEnv { 
		Scope s;
		std::vector<TokenQueue> pre_block;
		UserFunctionEnv(std::vector<TokenQueue> _pre_block, Scope &_s) : pre_block(_pre_block) {
			s.parent = &_s;
		}

		variable simple_call() {
			auto entry = parse_from_tq(pre_block);
			evaluate_lines(entry);
		}
	};


	class UserFunctionVariable : public BaseVariable { // wraps UserFunction class for get() availability
		static const RegisterType const* typei;
	public:
		UserFunctionVariable(){}
		long get();
		void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
		RegisterType const* const get_typei();
	};

}
#endif // !H_LWC_USERFUNCS