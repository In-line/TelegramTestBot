#include "sql_exception.h"

sql_exception::sql_exception(const std::__cxx11::string &what_arg) : std::runtime_error (what_arg)
{

}

sql_exception::sql_exception(const char *what_arg)  : std::runtime_error (what_arg)
{

}
