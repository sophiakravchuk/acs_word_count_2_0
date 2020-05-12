#include <fstream>
#include <algorithm>
#include <iomanip>
#include "manager.h"
#include "queue.h"
#include "functions.h"

Manager::Manager(int threads_count){
    am_of_threads = threads_count;
    am_of_working_th_text = am_of_threads;
    start_threads();
}

void Manager::start_threads(){
    vector_of_threads.reserve(am_of_threads+2);

    for (int i = 0; i < 1; i++) {
        vector_of_threads.emplace_back(paths_to_vecs,
                                       std::ref(queue_paths), std::ref(queue_text), 1);
    }

    for (int i = 0; i < am_of_threads; i++) {
        vector_of_threads.emplace_back(count_part_words,
                std::ref(queue_text), std::ref(queue_dicts), std::ref(am_of_working_th_text));
    }

    for (int i = 0; i < 1; i++) {
        vector_of_threads.emplace_back(merge_for_one_thread,
                                       std::ref(queue_dicts), std::ref(largeDict), std::ref(smallDict));
    }
}

bool sort_by_val (const std::pair<std::string, int>& a,
                const std::pair<std::string, int>& b)
{
    if(a.second != b.second) {
        return a.second > b.second;
    }
    return  a.first < b.first;
}


std::map<std::string, int> Manager::get_res() {
    for (auto &t: vector_of_threads) {
        t.join();
    }

    return queue_dicts.pop_back();
}

void Manager::res_to_file(std::map<std::string, int>& mp,
                          const std::string& out_by_a_file, const std::string& out_by_n_file) {
    std::ofstream outfile_a;
    outfile_a.open(out_by_a_file);
    for (auto &it : mp) {
        outfile_a << it.first << std::setw(5) << it.second << std::endl;
    }
    outfile_a.close();
    std::ofstream outfile_n;
    outfile_n.open(out_by_n_file);
    std::vector<std::pair<std::string, int>> vec;
    std::map<std::string, int>::iterator it2;
    for (auto it2 = mp.cbegin(); it2 != mp.cend(); ++it2) {
        vec.emplace_back(*it2);
    }

    std::sort(vec.begin(), vec.end(), sort_by_val);
    for (auto &it : vec) {
        outfile_n << it.first << std::setw(5) << it.second << std::endl;
    }
    outfile_n.close();
}
