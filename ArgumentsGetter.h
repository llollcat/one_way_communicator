#ifndef ONE_WAY_COMMUNICATOR_ARGUMENTSGETTER_H
#define ONE_WAY_COMMUNICATOR_ARGUMENTSGETTER_H

#include "vector"
#include "string"
#include <algorithm>

class ArgumentsGetter {
private:
    std::vector<std::string> tokens;


public:
    ArgumentsGetter(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.emplace_back(argv[i]);
    }


    [[nodiscard]] bool isCmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }

    [[nodiscard]] const std::string &getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const std::string empty_string;
        return empty_string;
    }

    // if doen not exist out error
    const std::string &getCmdOptionSafely(const std::string &option) {
        if (!isCmdOptionExists(option)) {
            std::cout << option << " - not given. -h for help and example" << std::endl;
            exit(-1);

        }
        return getCmdOption(option);

    }
};


#endif
