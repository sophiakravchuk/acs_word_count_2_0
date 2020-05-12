#ifndef WORD_C2_CONFIG_READ_H
#define WORD_C2_CONFIG_READ_H

#include <iostream>
#include <istream>
#include <streambuf>
#include <string>
#include <vector>
struct argvs
{
    argvs() = default;

    const char *out_a;
    const char *out_n;
    const char *dir;
    int am_of_threads;
};

bool is_delimiter(char value, const std::string& delims);

std::vector<std::string> reading_from_file(const std::string& file_name);

argvs config_read(int argc, char** argv);

#endif //WORD_C2_CONFIG_READ_H
