//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

// ImGui includes
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"

using namespace dae;

void ShutDown(SDL_Window* windowPtr, SDL_Renderer* SDLRendererPtr)
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(SDLRendererPtr);
    SDL_DestroyWindow(windowPtr);
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //Unreferenced parameters
    (void)argc;
    (void)args;

    //Create window + surfaces
    SDL_Init(SDL_INIT_VIDEO);

    constexpr uint32_t width  = 640;
    constexpr uint32_t height = 480;

    SDL_Window* windowPtr = SDL_CreateWindow(
        "Rasterizer - **Ádám Knapecz (2DAE09)**",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);
    SDL_Renderer* SDLRendererPtr = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (not windowPtr)
    {
        SDL_Log("Error creating SDL_Window!");
        return 1;
    }
    if (not SDLRendererPtr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 1;
    }

    //Initialize "framework"
    const auto timerPtr    = new Timer();
    const auto rendererPtr = new Renderer(windowPtr, SDLRendererPtr);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(windowPtr, SDLRendererPtr);
    ImGui_ImplSDLRenderer2_Init(SDLRendererPtr);

    //Start loop
    timerPtr->Start();

    float printTimer     = 0.f;
    bool  isLooping      = true;
    
    while (isLooping)
    {
        //--------- Get input events ---------
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            // ImGui events
            ImGui_ImplSDL2_ProcessEvent(&e);
            
            switch (e.type)
            {
            case SDL_QUIT:
                isLooping = false;
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_F3:
                    rendererPtr->ToggleBoundingBoxVisibility();
                    break;
                case SDL_SCANCODE_F4:
                case SDL_SCANCODE_Z:
                    rendererPtr->ToggleDepthBufferVisibility();
                    break;
                case SDL_SCANCODE_F5:
                case SDL_SCANCODE_R:
                    rendererPtr->ToggleRotation();
                    break;
                case SDL_SCANCODE_F6:
                case SDL_SCANCODE_N:
                    rendererPtr->ToggleNormalVisibility();
                    break;
                case SDL_SCANCODE_F7:
                    rendererPtr->CycleShadingMode();
                    break;
                case SDL_SCANCODE_F8:
                    timerPtr->StartBenchmark();
                    break;
                case SDL_SCANCODE_E:
                    rendererPtr->GetCamera().IncreaseFOV();
                    break;
                case SDL_SCANCODE_Q:
                    rendererPtr->GetCamera().DecreaseFOV();
                    break;
                case SDL_SCANCODE_X:
                    rendererPtr->TakeScreenshot();
                    break;
                }
                break;
            case SDL_MOUSEWHEEL:
                rendererPtr->GetCamera().Scroll(e.wheel);
                break;
            }
        }

        if (rendererPtr->IsBenchmarking())
        {
            timerPtr->StartBenchmark();
            rendererPtr->StopBenchmark();
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        //--------- Update ---------
        rendererPtr->Update(timerPtr);

        //--------- Render ---------
        rendererPtr->Render();

        //--------- Timer ---------
        timerPtr->Update();
        printTimer += timerPtr->GetElapsed();
        if (printTimer >= 1.f)
        {
            printTimer = 0.f;
            // std::cout << "dFPS: " << timerPtr->GetdFPS() << std::endl;
        }

        //Save screenshot after full render
        if (rendererPtr->IsTakingScreenshot())
        {
            if (!rendererPtr->SaveBufferToImage())
                std::cout << "Screenshot saved!" << std::endl;
            else
                std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
            rendererPtr->StopTakingScreenshot();
        }
    }
    timerPtr->Stop();

    //Shutdown "framework"
    delete rendererPtr;
    delete timerPtr;

    ShutDown(windowPtr, SDLRendererPtr);
    return 0;
}
