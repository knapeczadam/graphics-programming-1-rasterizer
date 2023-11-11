//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"

#include <iostream>

#include "Maths.h"
#include "Texture.h"
#include "Utils.h"

#include "Macros.h"

namespace dae
{
    Renderer::Renderer(SDL_Window* pWindow) :
        m_pWindow(pWindow)
    {
        //Initialize
        SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

        //Create Buffers
        m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
        m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
        m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;

        //m_pDepthBufferPixels = new float[m_Width * m_Height];

        //Initialize Camera
        m_Camera.Initialize(60.f, {.0f, .0f, -10.f});
    }

    Renderer::~Renderer()
    {
        //delete[] m_pDepthBufferPixels;
    }

    void Renderer::Update(Timer* pTimer)
    {
        m_Camera.Update(pTimer);
    }

    void Renderer::Render()
    {
        //@START
        //Lock BackBuffer
        SDL_LockSurface(m_pBackBuffer);

#if W1
#if TODO_0
        Render_W1_TODO_0();
#elif TODO_1
        Render_W1_TODO_1();
#elif TODO_2
        Render_W1_TODO_2();
#elif TODO_3
        Render_W1_TODO_3();
#elif TODO_4
        Render_W1_TODO_4();
#elif TODO_5
        Render_W1_TODO_5();
#endif
        
#elif W2
#elfi W3
#elif W4
#endif
        

        //@END
        //Update SDL Surface
        SDL_UnlockSurface(m_pBackBuffer);
        SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::VertexTransformationFunction(const std::vector<Vertex>& vertices_in,
                                                std::vector<Vertex>& vertices_out) const
    {
        //Todo > W1 Projection Stage
    }

    void Renderer::UpdateColor(ColorRGB& finalColor, int px, int py) const
    {
                //Update Color in Buffer
                finalColor.MaxToOne();

                m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
                                                                      static_cast<uint8_t>(finalColor.r * 255),
                                                                      static_cast<uint8_t>(finalColor.g * 255),
                                                                      static_cast<uint8_t>(finalColor.b * 255));
    }

    void Renderer::Render_W1_TODO_0()
    {
        //RENDER LOGIC
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                float gradient = px / static_cast<float>(m_Width);
                gradient += py / static_cast<float>(m_Width);
                gradient /= 2.0f;

                ColorRGB finalColor{gradient, gradient, gradient};

                UpdateColor(finalColor, px, py);
            }
        }
    }

    void Renderer::Render_W1_TODO_1()
    {
    }

    void Renderer::Render_W1_TODO_2()
    {
    }

    void Renderer::Render_W1_TODO_3()
    {
    }

    void Renderer::Render_W1_TODO_4()
    {
    }

    void Renderer::Render_W1_TODO_5()
    {
    }

    bool Renderer::SaveBufferToImage() const
    {
        return SDL_SaveBMP(m_pBackBuffer, "Rasterizer_ColorBuffer.bmp");
    }
}
