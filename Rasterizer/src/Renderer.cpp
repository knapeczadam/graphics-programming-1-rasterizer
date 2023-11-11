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
        vertices_out.clear();
        vertices_out.resize(vertices_in.size());

        for (size_t i{}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex& vertex_out = vertices_out[i];

            // From NDC to Screen Space
            vertex_out.position.x = (vertex_in.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_in.position.y) * 0.5f * static_cast<float>(m_Height);
        }
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

    /**
     * \brief Rasterize the given triangle (each vertex is already defined in NDC space)
     * You should convert to each pixel to Screen Space (pixel coordinates)
     * Loop over all pixels, and then check if triangle covers the given pixel
     * Vertex vector can be defined in the render function itself
     */
    void Renderer::Render_W1_TODO_1()
    {
        // Define triangle - vertices in NDC space
        std::vector<Vertex> vertices_ndc
        {
            {{0.0f, 0.5f, 1.0f}},
            {{0.5f, -0.5f, 1.0f}},
            {{-0.5f, -0.5f, 1.0f}}
        };

        std::vector<Vertex> vertices_ss;
        VertexTransformationFunction(vertices_ndc, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};
                if (IsPointInTriangleV1(pixel, v0, v1, v2))
                {
                    finalColor = colors::White;
                }
                UpdateColor(finalColor, px, py);
            }
        }
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
