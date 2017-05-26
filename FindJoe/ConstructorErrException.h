#ifndef CONSTRUCTOR_ERR_EXCEPTION_H
#define CONSTRUCTOR_ERR_EXCEPTION_H

#include <exception>

struct ConstructorErrException : public std::exception {
   const char * what () const throw () {
      return "Constructor Error Exception";
   }
};

#endif
