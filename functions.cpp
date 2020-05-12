#include "functions.h"
#include "queue.h"
#include <vector>
#include <map>
#include <algorithm>
#include <zip.h>
#include <cstring>
#include <sstream>
#include <boost/locale.hpp>
#include "file_to_vecs.h"
void merge_for_one_thread(Queue<std::map<std::string, int>> &q, int &largeDict, int &smallDict) {
    while (true) {
        std::map<std::string, int> dict1 = q.pop_back();
        if (dict1 == q.get_poison_pill()) {
            break;
        }
        std::map<std::string, int> dict2 = q.pop_back();
        if (dict2 == q.get_poison_pill()) {
            q.push_front(dict1);
            break;
        }

        if (dict1.size() > dict2.size()) {
            largeDict = dict1.size();
            smallDict = dict2.size();
            for (const auto &x : dict2) {
                dict1[x.first] += x.second;
            }
            dict2.erase(dict2.begin(), dict2.end());

            q.push_back(dict1, false);
        } else {
            largeDict = dict2.size();
            smallDict = dict1.size();
            for (const auto &x : dict1) {
                dict2[x.first] += x.second;
            }
            dict1.erase(dict1.begin(), dict1.end());

            q.push_back(dict2, false);
        }

//        std::map<std::string,int>::iterator it;
        //for (it=dict1.begin(); it!=dict1.end(); ++it)

    }
    std::cout << "Finish listing merge" << std::endl << std::flush;
}

void count_part_words(Queue<std::string> &q_from,
                      Queue<std::map<std::string, int>> &q_to, std::atomic<int>& am) {
    while(true){
        std::string words_str = q_from.pop_back();

        if (words_str == q_from.get_poison_pill()){
            am--;
            if (am == 0){
                q_to.will_be_push = false;
                std::cout << "Finish listing count_part_words" << std::endl << std::flush;
            }
            q_from.cv_is_empty_m.notify_all();
            q_to.cv_is_empty_m.notify_all();
            break;
        }

        std::istringstream words(words_str);
        std::map<std::string, int> result = {};

        boost::locale::boundary::ssegment_index
           map(boost::locale::boundary::word, words_str.begin(), words_str.end());

        map.rule(boost::locale::boundary::word_any);

//        for(boost::locale::boundary::ssegment_index::iterator it=map.begin(),e=map.end();it!=e;++it) {
//            auto it1 = boost::locale::normalize((*it));
//            auto it2 = boost::locale::fold_case(it1);
//            result[it2]++;
//        }

        std::string el;
        while (words >> el) {
            auto el1 = boost::locale::normalize(el);
            el.erase();
            auto el2 = boost::locale::fold_case(el1);
            el1.erase();

            el2.erase(std::remove_if(el2.begin(), el2.end(), [](auto c) { return std::ispunct(c) || std::isdigit(c); }),
                      el2.end());
            if (!el2.empty()) {
                ++result[el2];
            }
            el2.erase();

        }
        words_str.erase (words_str.begin(), words_str.end());
        words.clear();
        q_to.push_front(result);
    }
}

void paths_to_vecs(Queue<std::string> &q_from,
                   Queue<std::string> &q_to, std::atomic<int> am) {
    while(true) {
        std::string file = q_from.pop_back();

        if (file == q_from.get_poison_pill()){
            {
                am--;
//                q_from.push_front(file);
            }
            if (am == 0){
                q_to.will_be_push = false;
                std::cout << "Finish listing path_to_vecs " << std::endl << std::flush;
            }
            q_from.cv_is_empty_m.notify_all();
            q_to.cv_is_empty_m.notify_all();
            break;
        }

        const char *file_n = file.c_str();

        struct zip *zip_file;
        struct zip_stat file_info;
        struct tm file_time;
        int err;
        int files_total;
        int i;

        zip_file = zip_open(file_n, 0, &err);
        if (!zip_file) {
            fprintf(stderr, "Error: can't open file %s\n", file_n);
            continue;
        }

        files_total = zip_get_num_files(zip_file);

        for (i = 0; i < files_total; i++) {
            zip_stat_index(zip_file, i, 0, &file_info);
            std::string fn = file_info.name;
            std::string val = fn.substr(fn.size() - 4);
            const char *value = val.c_str();
            if (strcmp(".txt", value) == 0 && file_info.size < 10000000) {
                err = 0;
                zip *z = zip_open(file_n, 0, &err);

                //Search for the file of given name
                struct zip_stat st;
                zip_stat_init(&st);
                zip_stat(z, fn.c_str(), 0, &st);

                //Alloc memory for its uncompressed contents
                char *contents = new char[st.size];

                //Read the compressed file

                zip_file_t *f = zip_fopen(z, fn.c_str(), 0);
                zip_fread(f, contents, st.size);
                zip_fclose(f);

                //And close the archive
                zip_close(z);
                auto string1 = std::string(contents);
                delete contents;
                q_to.push_front(string1);

            }
        }
        zip_close(zip_file);
        file.erase (file.begin(), file.end());
    }
}
