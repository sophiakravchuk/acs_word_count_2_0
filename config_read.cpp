#include <fstream>
#include "config_read.h"


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


std::vector<std::string> reading_from_file(const std::string& file_name){
    std::string line;
    std::ifstream myFile;
    myFile.open(file_name);

    std::vector<std::string> vecOfLines;
    std::string currLine;
    if (myFile.is_open()) {
        while (std::getline(myFile, currLine)) {
            // Line contains string of length > 0 then save it in vector
            if (currLine.size() > 0)
                vecOfLines.push_back(currLine);
        }
        myFile.close();
        return vecOfLines;
    }
    else {
        throw MyParsingError("No such file was found");
    }
}

argvs config_read(int argc, char** argv) {
    argvs config_param = argvs();

    try {
        std::string config_file;
        if (argc > 1) {
            config_file = argv[1];
        }
        else { config_file = "../config.txt";}

        //std::cout << config_file << std::endl;
        std::vector<std::string> ret = reading_from_file(config_file);

        if (ret.size() < 4 ) {
            throw MyParsingError("Not enough arguments in the input file");
        }
        else if (ret.size() > 4 ) {
            throw MyParsingError("Too many arguments in the input file");
        }

        for (auto el: ret) {
            std::string delimiter = "=";
            std::string key = el.substr(0, el.find(delimiter));
            std::string value = el.substr(el.find(delimiter)+1);

                if (value[0] == '"') {
                    value.erase(value.begin() + 0, value.begin() + 1);
                    value.erase(value.end() - 1, value.end());
                }
                if (key == "out_by_a"){
                    value = "../" + value;
                    std::string *value1 = new std::string(value);
                    config_param.out_a = value1->c_str();
                }
                else if (key == "out_by_n"){
                    value = "../" + value;
                    std::string *value1 = new std::string(value);
                    config_param.out_n = value1->c_str();
                }
                else if (key == "dir"){
                    std::string *value1 = new std::string(value);
                    config_param.dir = value1->c_str();
                }
                else if (key == "threads"){
                    std::string *value1 = new std::string(value);
                    config_param.am_of_threads = std::stoi((*value1));

                }
        }

    }
    catch (std::invalid_argument&) {
        throw MyParsingError("Arguments in the file are not numeric");
    }
    catch (std::logic_error&) {
        throw MyParsingError("Not enough arguments");
    }

    return config_param;
}
