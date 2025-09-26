#ifndef AFFICHAGE_HPP
#define AFFICHAGE_HPP
#include <iostream>
#include "matrice.hpp"
#include "C:/Users/charl/Desktop/C++/Installation/SDL2-2.30.9/x86_64-w64-mingw32/include/SDL2/SDL.h"

class Sdl{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool en_cours;

public:
    Sdl(const char* title, int width, int height);
    ~Sdl();
    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void present();
    static int affichage_final(const std::vector<double>& C, const std::vector<double>& C_s, int WINDOW_WIDTH, int WINDOW_HEIGHT);
};

#endif