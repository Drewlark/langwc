#include <memory>
#include "lwc_typedefs.h"
#ifndef LWC_REGISTERSTORE
#define LWC_REGISTERSTORE

namespace lwc {
	
	class RegisterStore {
		variable* registers;
		int pos = 0;
		int size = 0;
	public:
		RegisterStore(const int& _size = 1) : size(_size){
			registers = new variable[_size];
		}

		variable& use_space(BaseVariable * v) {
			*(registers[pos]) = *v;
			++pos;
			return registers[pos];
		}

		void resize(const int& new_size) {
			delete[] registers;
			size = new_size;
			registers = new variable[new_size];
		}

		~RegisterStore() {
			delete[] registers;
		}
	};

	
}
#endif // !LWC_REGISTERSTORE