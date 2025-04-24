#include <string>
#include <vector>
#include <sstream>

std::string Trim(std::string str, std::string whitespace = " \t\r");
std::vector<std::string> Split(std::string str, char delim);
bool IsStrAlpha(std::string str);
