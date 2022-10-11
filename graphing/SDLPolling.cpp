//
// Created by tg2-merchant on 11/10/2022.
//

#include "SDLPolling.h"

static SDLPolling* instance = nullptr;

SDLPolling *SDLPolling::get_instance() {
    if( !instance )
    {
        instance = new SDLPolling;
    }

    return instance;
}

SDLPolling::SDLPolling() {}

void SDLPolling::register_close_evt(Uint32 window, std::function<void()> callback ) {
    std::unique_lock lck( event_mutex );

    close_events.insert_or_assign( window, callback );
}

void SDLPolling::poll() {
    SDL_Event e;
    while( SDL_PollEvent( &e ) )
    {
        handle_event( e );
    }
}

void SDLPolling::block() {
    SDL_Event e;
    SDL_WaitEvent( &e );
    handle_event( e );
}

void SDLPolling::handle_event( SDL_Event e ) {
    switch ( e.type ) {
        case SDL_QUIT:
            delete instance;
            instance = nullptr;
            return;
        case SDL_WINDOWEVENT:
            auto windowId = e.window.windowID;
            if( e.window.event == SDL_WINDOWEVENT_CLOSE )
            {
                std::unique_lock lck( event_mutex );
                if( close_events.contains( windowId ) )
                {
                    close_events[ windowId ]();
                }
            }
            break;
    }
}
