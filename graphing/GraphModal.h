//
// Created by tg2-merchant on 11/10/2022.
//

#include <SDL.h>
#include <string>
#include <condition_variable>
#include <shared_mutex>
#include <map>
#include <vector>
#include "nanovg.h"

#pragma once

class GraphModal {
public:
    GraphModal( int width, int height, std::string title );
    ~GraphModal();

    void set_x_label( std::string x_label );
    void set_y_label( std::string y_label );
    void show_legend( bool );
    void add_point( double x, double y, std::string series );
    void series_colour( std::string series,  std::array<uint8_t, 3> colour );

    bool open;
private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::string title;
    int width, height;
    std::condition_variable closed;
    NVGcontext* nvgCtx;
    SDL_GLContext glCtx;

    std::string x_label;
    std::string y_label;
    bool legend_visible = true;

    std::map<std::string, std::array<uint8_t, 3>> colours;
    std::map<std::string, std::vector<std::pair<double, double>>> points;

    void on_close();
    void draw();
};