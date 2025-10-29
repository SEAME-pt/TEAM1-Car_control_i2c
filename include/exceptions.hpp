#pragma once

#include <SDL2/SDL.h>
#include <string>

class GenException {

public:
    class InitException : public std::exception {
        public:
			const char* what() const throw() {
                return (SDL_GetError());
            }
    };

};