#include "lwc_typedefs.h"
namespace lwc {
	variable * Scope::handle_generic_token(const ParseToken& pt) {
		if (pt.keywords.count(Keywords::ELAST)) {
			return handle_name<LASTVariable>(pt.val);
		}
		else {
			return handle_name(pt.val);
		}
	}
}