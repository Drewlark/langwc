#ifndef LWC_VARIABLE
#define LWC_VARIABLE
struct Variable { //Currently unused
	long val;
	bool is_ptr = false;
	
	Variable(long _val) {
		val = _val;
	}

	Variable(long _val, bool _is_ptr) {
		val = _val;
		_is_ptr = is_ptr;
	}

};
#endif //!LWC_VARIABLE