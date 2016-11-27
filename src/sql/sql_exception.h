#ifndef SQL_EXCEPTION_H
#define SQL_EXCEPTION_H
#include <stdexcept>

class sql_exception :  public std::runtime_error
{
public:
	explicit sql_exception( const std::string& what_arg );
	explicit sql_exception( const char* what_arg );
};

#endif // SQL_EXCEPTION_H
