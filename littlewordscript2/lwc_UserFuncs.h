#include "lwc_typedefs.h"
#include "lwc_Parser.h"
#include <tuple>
#include <map>
#ifndef H_LWC_USERFUNCS
#define H_LWC_USERFUNCS
namespace lwc {

	struct UserFunctionTemplate { 
		Scope scope;
		std::vector<TokenQueue> pre_block;
		UserFunctionTemplate(std::vector<TokenQueue> _preprocessed_block, Scope & parent_scope, std::vector<ParseToken> params) : pre_block(_preprocessed_block) {
			for (const auto &pt : params) {
				scope.handle_generic_token(pt);
			}
			scope.parent = &parent_scope;
		}

		variable simple_call() {
			auto entry = threadsafe_parse_from_tq(pre_block, scope);
			return evaluate_lines(entry);
		}
	};


	class UserFunctionVariable : public BaseVariable { // wraps UserFunction class for get() availability
		static const RegisterType const* typei;
		UserFunctionTemplate uft;
	public:
		UserFunctionVariable(UserFunctionTemplate _uft) : uft(_uft){}
		long get() { return uft.simple_call()->get(); };
		void* get_vp(void*& reg) { return uft.simple_call(); }
		RegisterType const* const get_typei();
	};

}
#endif // !H_LWC_USERFUNCS