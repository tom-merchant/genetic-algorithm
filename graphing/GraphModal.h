//
// Created by tg2-merchant on 11/10/2022.
//

#include <SDL.h>
#include <string>
#include <condition_variable>
#include <shared_mutex>
#include <map>
#include <vector>
#include <cmath>
#include "nanovg.h"

#pragma once

typedef std::array<uint8_t, 3> colour;

class GraphModal {
public:
    GraphModal( int width, int height, std::string title );
    ~GraphModal();

    void set_x_label( std::string x_label );
    void set_y_label( std::string y_label );
    void show_legend( bool );
    void add_point( double x, double y, std::string series );
    void series_colour( std::string series,  colour );
    void set_origin( std::array<double, 2> );

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
    int font_size_pt = 14;
    int x_gradations_px = 70;
    int y_gradations_px = 40;
    int axes_inset_px = 30;
    int axes_line_weight_px = 2;
    float device_px_ratio;

    double maxX = -INFINITY;
    double maxY = -INFINITY;

    std::array<double, 2> origin = {0,0};

    colour axes_colour = {255, 255, 255};

    std::map<std::string, colour> colours;
    std::map<std::string, std::vector<std::pair<double, double>>> points;

    void on_close();
    void draw();

    void drawAxes();

    void drawLegend();

    void drawSeries();

    void drawAxisTicks();

    void setFont();
};