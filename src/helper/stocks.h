#ifndef STOCKS_H
#define STOCKS_H
#include <utility>
#include <string>
#include <stdint.h>
int random_int(int start, int end, bool generateNewSeed = false);

bool is_string_empty(const std::string &str);

std::string trim(const std::__cxx11::string &str, const short type = 0);

std::pair<std::string, std::string> string_tok(const std::string &str,
																							 const std::string &delimiter,
																							 bool toTrim = 1);

void msleep(uint delay);
#endif // STOCKS_H

