#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace utils {
    void ClearScreen();
    char GetKey();
    std::string LoadUI(const std::string& filename);
}

#endif // UTILITIES_H 