#include "file_to_vecs.h"
#include <sstream>

std::istringstream chars_to_vecs(char* buffer, int am_of_vecs)
{
    std::istringstream in((std::string(buffer)));
//    std::string word;
//    std::vector<std::vector<std::string>> all_vec;
//    for (int i = 0; i < am_of_vecs; i ++) {
//        std::vector<std::string> new_vec;
//        all_vec.push_back(new_vec);
//    }
//
//    int numb = 0;
//    while (in) {
//        in >> word;
//        std::string str(word);
//        all_vec[numb % am_of_vecs].push_back(word);
//        numb++;
//    }
//    all_vec[(numb-1) % am_of_vecs].pop_back();
    return in;

}
