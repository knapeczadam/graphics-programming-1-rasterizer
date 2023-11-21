#pragma once

#include <cstdint>
#include <vector>

#include "Camera.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
    class Texture;
    struct Mesh;
    struct Vertex;
    class Timer;
    class Scene;

    class Renderer final
    {
    private:
        enum class PrimitiveTopology
        {
            TriangleList,
            TriangleStrip
        };

    public:
        Renderer(SDL_Window* pWindow);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        void Update(Timer* pTimer);
        void Render();

        bool SaveBufferToImage() const;

        inline Camera& GetCamera() { return m_Camera; }

    private:
        void VertexTransformationFromWorldToScreen(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const;
        void VertexTransformationFromWorldToScreenV2(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void VertexTransformationFromNDCtoScreenSpace(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const;

        int GetBufferIndex(int x, int y) const;

        void UpdateColor(ColorRGB& finalColor, int px, int py) const;
        void Render_W1_TODO_0();
        void Render_W1_TODO_1() const;
        void Render_W1_TODO_2() const;
        void Render_W1_TODO_3() const;
        void Render_W1_TODO_4();
        void Render_W1_TODO_5();

        void Render_W2_TODO_1();
        void Render_W2_TODO_2();
        void Render_W2_TODO_3();
        void Render_W2_TODO_4();
        void Render_W2_TODO_5();

        void Render_W3_TODO_0();
        void Render_W3_TODO_1();
        void Render_W3_TODO_2();
        void Render_W3_TODO_3();

    private:
        SDL_Window* m_pWindow{};

        SDL_Surface* m_pFrontBuffer{nullptr};
        SDL_Surface* m_pBackBuffer{nullptr};
        uint32_t* m_pBackBufferPixels{nullptr};

        // General texture
        Texture* m_pTexture           {nullptr};

        // float* m_pDepthBufferPixels{};
        std::vector<float> m_DepthBuffer{};

        Camera m_Camera{};

        int m_Width{};
        int m_Height{};
    };
}
