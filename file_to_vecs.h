#ifndef WORD_C2_FILE_OF_VECS_H
#define WORD_C2_FILE_OF_VECS_H

#include <iostream>
#include <istream>
#include <streambuf>
#include <string>
#include <vector>
struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

std::istringstream chars_to_vecs(char* buffer, int am_of_vecs);
#endif //WORD_C2_FILE_OF_VECS_H
