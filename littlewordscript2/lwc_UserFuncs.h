#include "lwc_typedefs.h"
#include "lwc_Parser.h"
#include <tuple>
#include <map>
#ifndef H_LWC_USERFUNCS
#define H_LWC_USERFUNCS
namespace lwc {

	struct UserFunctionTemplate { 
		Scope s;
		std::vector<TokenQueue> pre_block;
		UserFunctionTemplate(std::vector<TokenQueue> _pre_block, Scope &_s) : pre_block(_pre_block) {
			s.parent = &_s;
		}

		variable simple_call() {
			auto entry = parse_from_tq(pre_block);
			return evaluate_lines(entry);
		}
	};


	class UserFunctionVariable : public BaseVariable { // wraps UserFunction class for get() availability
		static const RegisterType const* typei;
		UserFunctionTemplate uft;
	public:
		UserFunctionVariable(UserFunctionTemplate _uft) : uft(_uft){}
		long get() { return uft.simple_call()->get(); };
		void* get_vp(void*& reg) { *(long*)reg = this->get();  return reg; }
		RegisterType const* const get_typei();
	};

}
#endif // !H_LWC_USERFUNCS