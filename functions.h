#ifndef WORDS_COUNT_2_0_FUNCTIONS_H
#define WORDS_COUNT_2_0_FUNCTIONS_H

#include <vector>
#include <map>
#include "queue.h"

void merge_for_one_thread(Queue<std::map<std::string, int>> &q, int &largeDict, int &smallDict);

void count_part_words(Queue<std::string> &q_from,
                      Queue<std::map<std::string, int>> &q_to, std::atomic<int> &am);

void paths_to_vecs(Queue<std::string> &q_from,
                   Queue<std::string> &q_to, std::atomic<int> am);

#endif //WORDS_COUNT_2_0_FUNCTIONS_H
