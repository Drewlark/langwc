#include <memory>
#include <vector>

#ifndef LWC_TYPEDEF
#define LWC_TYPEDEF

typedef std::shared_ptr<long> variable;
typedef std::vector<variable> varset;
using builtin_func = long(*)(const varset&);
#endif