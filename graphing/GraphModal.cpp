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
    window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );
    SDL_SetWindowTitle( window, title.c_str() );

    SDLPolling::get_instance()->register_close_evt(SDL_GetWindowID(window), [this]() { on_close(); });

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    glCtx = SDL_GL_CreateContext( window );
    SDL_GL_MakeCurrent( window, glCtx );
    glewInit();
    glGetError();

    int fbWidth, fbHeight;
    SDL_GL_GetDrawableSize( window, &fbWidth, &fbHeight );
    device_px_ratio = (float)fbWidth / (float)width;
    glViewport( 0, 0, fbWidth, fbHeight );
    glDisable( GL_CULL_FACE );

    nvgCtx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
    nvgCreateFont( nvgCtx, "sans", "Roboto-Regular.ttf" );
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

void GraphModal::series_colour( std::string series, colour colour ) {
    colours.insert_or_assign( series, colour );
    draw();
}

#define TO_NVG(x) ((NVGcolor){x[0] / 255.0f, x[1] / 255.0f, x[2] / 255.0f, 1.0f})

void GraphModal::draw() {
    SDL_GL_MakeCurrent( window, glCtx );

    glClearColor(0,0,0,0);
    glClear( GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0,0,0,1);
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    nvgBeginFrame( nvgCtx, width, height, device_px_ratio );
        drawAxes();
        drawLegend();
        drawSeries();
    nvgEndFrame( nvgCtx );

    SDL_GL_SwapWindow( window );
}

void GraphModal::drawAxes() {
    nvgBeginPath( nvgCtx );
    nvgMoveTo( nvgCtx, axes_inset_px, 0 );
    nvgLineTo( nvgCtx, axes_inset_px, height - axes_inset_px );
    nvgLineTo( nvgCtx, width, height - axes_inset_px );
    nvgStrokeColor( nvgCtx, TO_NVG( axes_colour ) );
    nvgStrokeWidth( nvgCtx, axes_line_weight_px );
    nvgStroke( nvgCtx );
    nvgClosePath( nvgCtx );
}

void GraphModal::drawLegend() {
    nvgFontSize( nvgCtx, font_size_pt );
    nvgFontFace( nvgCtx, "sans" );
    nvgTextAlign( nvgCtx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );

    auto legTextY = axes_inset_px;
    auto legTextX = axes_inset_px;

    for ( auto series : points )
    {
        float bounds[ 4 ];
        nvgTextBounds( nvgCtx, 0, 0, series.first.c_str(), series.first.c_str() + series.first.length(), bounds );
        legTextX = std::max( legTextX, (int)bounds[ 2 ] + axes_inset_px );
    }

    legTextX = width - legTextX;

    for ( auto series : points ) {
        auto series_colour = colours.contains( series.first ) ? colours[ series.first ] : colour({255,255,255});
        nvgFillColor( nvgCtx, TO_NVG( series_colour ) );
        float bounds[ 4 ];
        nvgTextBounds( nvgCtx, legTextX, legTextY, series.first.c_str(), series.first.c_str() + series.first.length(), bounds );
        nvgText( nvgCtx, legTextX, legTextY, series.first.c_str(), series.first.c_str() + series.first.length() );
        legTextY = bounds[ 3 ] + 5;
    }
}

void GraphModal::drawSeries() {
    double maxX = -INFINITY;
    double maxY = -INFINITY;

    for ( auto series : points ) {
        for ( auto point : series.second ) {
            maxX = std::max( maxX, point.first );
            maxY = std::max( maxY, point.second );
        }
    }

    double xStep = ( width - axes_inset_px ) / ( maxX - origin[0] );
    double yStep = ( height - axes_inset_px ) / ( maxY - origin[1] );

    for ( auto series : points ) {
        auto series_colour = colours.contains( series.first ) ? colours[ series.first ] : colour({255,255,255});
        nvgStrokeColor( nvgCtx, TO_NVG( series_colour ) );

        nvgBeginPath( nvgCtx );
        bool first = true;
        for ( auto point : series.second ) {
            auto xFromOrigin = point.first - origin[0];
            auto yFromOrigin = point.second - origin[1];
            if( first ){
                nvgMoveTo( nvgCtx, axes_inset_px + xFromOrigin * xStep, height - (axes_inset_px + yFromOrigin * yStep) );
                first = false;
                continue;
            }
            nvgLineTo( nvgCtx, axes_inset_px + xFromOrigin * xStep, height - (axes_inset_px + yFromOrigin * yStep) );
        }
        nvgStroke( nvgCtx );
        nvgClosePath( nvgCtx );
    }
}

void GraphModal::set_origin( std::array<double, 2> pt ) {
    origin = pt;
}


