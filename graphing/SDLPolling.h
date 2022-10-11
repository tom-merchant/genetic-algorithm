//
// Created by tg2-merchant on 11/10/2022.
//

#pragma once

#include <thread>
#include <mutex>
#include <SDL.h>
#include <map>
#include <shared_mutex>

class SDLPolling {
public:
    void register_close_evt(Uint32 window, std::function<void()> callback );
    static SDLPolling* get_instance();
    void poll();
    void block();

private:
    SDLPolling();
    std::map<Uint32, std::function<void()>> close_events;
    std::mutex event_mutex;

    void handle_event(SDL_Event event);
};