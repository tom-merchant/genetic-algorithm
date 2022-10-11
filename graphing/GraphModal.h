//
// Created by tg2-merchant on 11/10/2022.
//

#include <SDL.h>
#include <string>
#include <condition_variable>
#include <shared_mutex>

#pragma once

class GraphModal {
public:
    GraphModal( int width, int height, std::string title );
    ~GraphModal();
    bool open;
private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::string title;
    std::condition_variable closed;
    void on_close();
};