#include <iostream>
#include <map>
#include "manager.h"
#include "file_to_vecs.h"
#include "config_read.h"
#include "queue.h"
#include <filesystem>
#include <iterator>
#include <chrono>
#include <atomic>
#include <fstream>
#include <boost/locale.hpp>
#include "functions.h"

std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}


class MyParsingError : public std::exception {
public:
    explicit MyParsingError(const std::string& msg) : msg_(msg) {
        std::cout << msg << std::endl;
    }

    const char * what () const throw ()
    {
        return msg_.c_str();
    }

    std::string getMessage() const {return(msg_);}
private:
    std::string msg_;

};

bool is_delimiter(char value, const std::string& delims)
{
    for (auto d : delims) {
        if (d == value) return true;
    }
    return false;
}
std::vector<std::string> splitLine2(std::string str, std::string delims)
{
    std::vector<std::string> output;
    while (str.size() > 0) {
        if (is_delimiter(str[0], delims)) {
            output.push_back("");
            str = str.substr(1);
        } else {
            int i = 1;
            while (i < str.size() &&
                   !is_delimiter(str[i], delims))  {
                i++;
            }
            output.emplace_back(str.begin(), str.begin() + i);
            if (i + 1 < str.size()) {
                str =  str.substr(i + 1);
            } else {
                str = "";
            }
        }
    }
    return output;
}



int main(int argc, char** argv) {

    boost::locale::generator gen;
    std::locale::global(gen("en_us.UTF-8"));
    argvs config_par = config_read(argc, argv);
    Manager m = Manager(config_par.am_of_threads);
    auto start_time = get_current_time_fenced();
    auto files=0;
    auto timeIteration = start_time;
    for(const auto& dirEntry : std::filesystem::recursive_directory_iterator(config_par.dir)){
        if(std::filesystem::is_regular_file(dirEntry)) {
            std::string path_str = dirEntry.path().string();
            std::vector<std::string> splitedLine = splitLine2(path_str, ".");
            if(splitedLine[splitedLine.size()-1] == "zip" || splitedLine[splitedLine.size()-1] == "ZIP"){
                m.queue_paths.push_front(path_str);
                files++;
                if(files % 200 == 0) {
                    auto timeIterationDurationSec = to_us(get_current_time_fenced() - timeIteration)/1000000;
                    auto timeAllDurationSec = to_us(get_current_time_fenced() - start_time)/1000000;
                    std::cout<<"\rfiles:"<<files<<" largeDict:"<<m.largeDict<<" smallDict:"<<m.smallDict
                    <<" IterSec:"<<timeIterationDurationSec<<" AllSec:"<<timeAllDurationSec<<" "<<std::endl;
                    timeIteration = get_current_time_fenced();
                }
             }
        }
    }
    m.queue_paths.will_be_push = false;
    std::cout << "Finish listing pathes " << std::endl << std::flush;
    //std::cout << "Finish listing pathes " << std::endl;
    auto res = m.get_res();
    //std::cout << "Finish " << std::endl;
    auto finish_time = get_current_time_fenced();
    m.res_to_file(res, config_par.out_a, config_par.out_n);
    std::cout << "Total: " << to_us(finish_time - start_time)/1000000 << "seconds" << std::endl;
    return 0;
}

