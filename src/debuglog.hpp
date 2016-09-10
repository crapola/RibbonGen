#ifndef DEBUGLOG_HPP_INCLUDED
#define DEBUGLOG_HPP_INCLUDED
// Shorthands for debug cout
#ifndef NDEBUG
#include <iostream>
#define DOUT(x) std::cout<<x<<'\n';
#define DERR(x) std::cerr<<"@ "<<__FILE__<<", line "<<__LINE__<<"\n\t"<<__PRETTY_FUNCTION__<<":\n\t"<<"Error: "<<x<<'\n';
#else
#define DOUT(x)
#define DERR(x)
#endif // NDEBUG
#endif // DEBUGLOG_HPP_INCLUDED
