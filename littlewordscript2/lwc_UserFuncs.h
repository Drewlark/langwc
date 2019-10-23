#include "lwc_typedefs.h"
#ifndef H_LWC_USERFUNCS
#define H_LWC_USERFUNCS
namespace lwc {
	// Function class must be threadsafe in so far as calling the same user function contemporaneously behaves as expected
	
	//Two tasks are necessary for thread-safe functions
	//First, the scope must be duplicated
	// 

	class UserFunctionTemplate { 
		/* "template" userfunction. 
		all Userfunction Envs will copy it's scope and rebuild the vector<LAST>
		using newly allocated addresses for any local variables.
		hmm...
		functions could be compiled differently?
		*/
		block_func templ;
		std::vector<std::vector<unsigned int>> vec_of_refs_to_all_lvars; // uint refers to the position of a line node containing [some lvar, the same as all others in its vector] in the breadthwise
		UserFunctionTemplate(block_func& bf) : templ(bf){
			for (LAST last : templ) {
				for (auto sec : vec_of_refs_to_all_lvars) {
					auto val = last.breadthwise.sec[0];
					for (auto item : sec) {

					}
				}
			}
		}
	};



	// Every userfunction will have to essentially copy all the LASTS and change local variables in the last to point to new locations
	class UserFunctionEnv { 
		Scope s;
		block_func bf;
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