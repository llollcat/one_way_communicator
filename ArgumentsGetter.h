#ifndef ONE_WAY_COMMUNICATOR_ARGUMENTSGETTER_H
#define ONE_WAY_COMMUNICATOR_ARGUMENTSGETTER_H

#include "vector"
#include "string"
#include <algorithm>

class ArgumentsGetter {
private:
    std::vector<std::string> m_tokensVector;


public:
    ArgumentsGetter(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            this->m_tokensVector.emplace_back(argv[i]);
    }


    [[nodiscard]] bool isCmdOptionExists(const std::string &option) const {
        return std::find(this->m_tokensVector.begin(), this->m_tokensVector.end(), option) != this->m_tokensVector.end();
    }

    [[nodiscard]] const std::string &getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->m_tokensVector.begin(), this->m_tokensVector.end(), option);
        if (itr != this->m_tokensVector.end() && ++itr != this->m_tokensVector.end()) {
            //todo fix
            return *itr;
        }
        static const std::string empty_string;
        return empty_string;
    }

    // if option does not exist out error
    [[nodiscard]] const std::string &getCmdOptionSafely(const std::string &option) const {
        if (!isCmdOptionExists(option)) {
            std::cout << option << " - not given. -h for help and example" << std::endl;
            exit(1);

        }
        return getCmdOption(option);

    }


    [[nodiscard]] const std::string &
    getCmdOptionIfGiven(const std::string &option, const std::string &default_value) const {
        if (!isCmdOptionExists(option)) {
            return default_value;

        }
        return getCmdOption(option);

    }
};


#endif
