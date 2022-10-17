//
// Created by tg2-merchant on 11/10/2022.
//

#include "GraphModal.h"
#include "SDLPolling.h"
#include "nanovg.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

GraphModal::GraphModal ( int width, int height, std::string title )
    : open( true ), width( width ), height( height ) {
    SDL_CreateWindowAndRenderer( width, height, SDL_WINDOW_OPENGL, &window, &renderer );
    SDL_SetWindowTitle( window, title.c_str() );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDLPolling::get_instance()->register_close_evt(SDL_GetWindowID(window), [this]() { on_close(); });

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    glCtx = SDL_GL_CreateContext( window );
    SDL_GL_MakeCurrent( window, glCtx );
    glewInit();
    nvgCtx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES );
}

GraphModal::~GraphModal() {
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    SDL_GL_DeleteContext( glCtx );
    nvgDeleteInternal(nvgCtx );
}

void GraphModal::on_close() {
    open = false;
}

void GraphModal::set_x_label( std::string x_label ) {
    this->x_label = x_label;
    draw();
}

void GraphModal::set_y_label(std::string y_label) {
    this->y_label = y_label;
    draw();
}

void GraphModal::show_legend( bool show ) {
    legend_visible = show;
    draw();
}

void GraphModal::add_point(double x, double y, std::string series) {
    if( !points.contains( series ) )
    {
        points.insert( { series, std::vector<std::pair<double, double>>() } );
    }

    points[ series ].push_back( { x, y } );
    draw();
}

void GraphModal::series_colour( std::string series, std::array<uint8_t, 3> colour ) {
    colours.insert_or_assign( series, colour );
    draw();
}

void GraphModal::draw() {
    SDL_GL_MakeCurrent( window, glCtx );
    nvgBeginFrame( nvgCtx, width, height, 1 );
    nvgEndFrame(nvgCtx );
}


