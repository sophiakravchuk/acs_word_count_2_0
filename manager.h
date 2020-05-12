#ifndef WORDS_COUNT_2_0_MANAGER_H
#define WORDS_COUNT_2_0_MANAGER_H

#include <vector>
#include <map>
#include <thread>
#include "queue.h"

class Manager{
private:
    std::mutex count_m;
    std::mutex path_m;
    int am_of_threads;
    void start_threads();

    std::atomic<int> am_of_working_th_text;

    std::string p_p_text = "";
    Queue<std::string> queue_text = Queue<std::string>(p_p_text);

    std::map<std::string, int> p_p_dict = {};
    Queue<std::map<std::string, int>> queue_dicts = Queue<std::map<std::string, int>>(p_p_dict);

    std::string p_p_path = "";
    std::vector<std::thread> vector_of_threads;

public:
    Manager(int threads_count);
    Queue<std::string> queue_paths = Queue<std::string>(p_p_path);

//    ~Manager();

    std::map<std::string, int> get_res();

    void res_to_file(std::map<std::string, int>& mp, const std::string& out_by_a_file,
                     const std::string& out_by_n_file);


    int largeDict = 0;
    int smallDict = 0;
};

#endif //WORDS_COUNT_2_0_MANAGER_H
