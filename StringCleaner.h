//Written by Emma Seba Sharp

#ifndef STRINGCLEANER_H
#define STRINGCLEANER_H
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

class StringCleaner{
private:
public:
    std::vector<std::string> clean(std::string);
    bool binarySearch(const std::vector<std::string> &, const std::string &);
};
#endif