#include <stdio.h>
#include <winsock.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_AtomicInt running;
} RenderContext;

int RenderingThread(void* data) {
    RenderContext* ctx = (RenderContext*)data;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;

    // VSync is important in a separate thread to prevent 100% CPU usage
    SDL_SetRenderVSync(ctx->renderer, 1);

    while (SDL_GetAtomicInt(&ctx->running)) {
        last = now;
        now = SDL_GetPerformanceCounter();
        double deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());

        SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
        SDL_RenderClear(ctx->renderer);

        

        SDL_RenderPresent(ctx->renderer);
    }
    return 0;
}
int main(int argc, char* argv[]) {
    
    // Init SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;

    // Define the render context
    RenderContext ctx;
    if (!SDL_CreateWindowAndRenderer("CChat", 1920, 1080, SDL_WINDOW_BORDERLESS, &ctx.window, &ctx.renderer)) return 2;

    SDL_SetAtomicInt(&ctx.running, 1);
    SDL_Thread* renderThread = SDL_CreateThread(RenderingThread, "RenderThread", &ctx);

    SDL_Event event;
    while (SDL_GetAtomicInt(&ctx.running)) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) SDL_SetAtomicInt(&ctx.running, 0);
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) SDL_SetAtomicInt(&ctx.running, 0);
                
            }
        }
    }

    SDL_WaitThread(renderThread, NULL);
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
    return 0;
}