#include "core.h"

std::string Trim(std::string str, std::string whitespace) {
    int strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos) {
        return "";
    }

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::vector<std::string> Split(std::string str, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(Trim(item));
    }
    if (result.size() == 0 && Trim(str).size() != 0) {
        result.push_back(Trim(str));
    }

    return result;
}

bool IsStrAlpha(std::string str) {
    bool strAlpha = false;

    for (int i = 0; i < str.length(); i++) {
        strAlpha = isalpha(str[i]);

        if (strAlpha) {
            return strAlpha;
        }
    }
    return strAlpha;
}
