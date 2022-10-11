//
// Created by tg2-merchant on 11/10/2022.
//

#include "GraphModal.h"
#include "SDLPolling.h"

GraphModal::GraphModal (int width, int height, std::string title ) : open( true ) {
    SDL_CreateWindowAndRenderer( width, height, 0, &window, &renderer );
    SDL_SetWindowTitle( window, title.c_str() );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDLPolling::get_instance()->register_close_evt(SDL_GetWindowID(window), [this]() { on_close(); });
}

GraphModal::~GraphModal() {
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
}

void GraphModal::on_close() {
    open = false;
}


