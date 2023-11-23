//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //Unreferenced parameters
    (void)argc;
    (void)args;

    //Create window + surfaces
    SDL_Init(SDL_INIT_VIDEO);

    const uint32_t width = 640;
    const uint32_t height = 480;

    SDL_Window* pWindow = SDL_CreateWindow(
        "Rasterizer - **Ádám Knapecz (2DAE09)**",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);

    if (!pWindow)
        return 1;

    //Initialize "framework"
    const auto pTimer = new Timer();
    const auto pRenderer = new Renderer(pWindow);

    //Start loop
    pTimer->Start();

    // Start Benchmark
    // TODO pTimer->StartBenchmark();

    float printTimer = 0.f;
    bool isLooping = true;
    bool takeScreenshot = false;
    while (isLooping)
    {
        //--------- Get input events ---------
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                isLooping = false;
                break;
            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_X)
                    takeScreenshot = true;
                if (e.key.keysym.scancode == SDL_SCANCODE_F3)
                    pRenderer->ToggleBoundingBoxVisibility();
                if (e.key.keysym.scancode == SDL_SCANCODE_F6)
                    pTimer->StartBenchmark();
                if (e.key.keysym.scancode == SDL_SCANCODE_E)
                    pRenderer->GetCamera().IncreaseFOV();
                if (e.key.keysym.scancode == SDL_SCANCODE_Q)
                    pRenderer->GetCamera().DecreaseFOV();
                if (e.key.keysym.scancode == SDL_SCANCODE_R)
                    pRenderer->ToggleRotation();
                if (e.key.keysym.scancode == SDL_SCANCODE_N)
                    pRenderer->ToggleNormalVisibility();
                if (e.key.keysym.scancode == SDL_SCANCODE_Z)
                    pRenderer->ToggleDepthBufferVisibility();
                break;
            case SDL_MOUSEWHEEL:
                pRenderer->GetCamera().Scroll(e.wheel);
                break;
            }
        }

        //--------- Update ---------
        pRenderer->Update(pTimer);

        //--------- Render ---------
        pRenderer->Render();

        //--------- Timer ---------
        pTimer->Update();
        printTimer += pTimer->GetElapsed();
        if (printTimer >= 1.f)
        {
            printTimer = 0.f;
            std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
        }

        //Save screenshot after full render
        if (takeScreenshot)
        {
            if (!pRenderer->SaveBufferToImage())
                std::cout << "Screenshot saved!" << std::endl;
            else
                std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
            takeScreenshot = false;
        }
    }
    pTimer->Stop();

    //Shutdown "framework"
    delete pRenderer;
    delete pTimer;

    ShutDown(pWindow);
    return 0;
}
