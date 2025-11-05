#pragma once

#include <exception>
#include <string>

class GenException {

public:
    class InitException : public std::exception {
        private:
            std::string message;
            
        public:
            InitException(const std::string& msg) : message(msg) {}
            
            const char* what() const throw() {
                return message.c_str();
            }
    };
};
