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
        void ToggleDepthBufferVisibility();
        void ToggleBoundingBoxVisibility();
        void ToggleNormalVisibility();
        void ToggleRotation();

    private:
        void InitCamera();
        void InitializeOutVertices();
        void InitializeTextures();
        
        void VertexTransformationFromWorldToScreenV1(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const;
        void VertexTransformationFromWorldToScreenV2(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void VertexTransformationFromWorldToScreenV3(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void VertexTransformationFromWorldToScreenV4(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void VertexTransformationFromNDCtoScreenSpace(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const;

        int GetBufferIndex(int x, int y) const;

        void UpdateColor(ColorRGB& finalColor, int px, int py) const;
        void PixelShadingV1(const Vertex_Out& vertex, ColorRGB& finalColor, const ColorRGB& diffuseColor = colors::White) const;
        
        // --- Week 1 ---
        void Render_W1_TODO_0() const;
        void Render_W1_TODO_1() const;
        void Render_W1_TODO_2() const;
        void Render_W1_TODO_3() const;
        void Render_W1_TODO_4();
        void Render_W1_TODO_5();

        // --- Week 2 ---
        void Render_W2_TODO_1();
        void Render_W2_TODO_2();
        void Render_W2_TODO_3();
        void Render_W2_TODO_4();
        void Render_W2_TODO_5();

        // --- Week 3 ---
        void Render_W3_TODO_0();
        void Render_W3_TODO_1();
        void Render_W3_TODO_2();
        void Render_W3_TODO_3();
        void Render_W3_TODO_4();
        void Render_W3_TODO_5();
        void Render_W3_TODO_6();

        // --- Week 4 ---
        void Render_W4_TODO_0();
        void Render_W4_TODO_1();
        void Render_W4_TODO_2();
        void Render_W4_TODO_3();

    private:
        SDL_Window* m_pWindow{};

        SDL_Surface* m_pFrontBuffer{nullptr};
        SDL_Surface* m_pBackBuffer{nullptr};
        uint32_t* m_pBackBufferPixels{nullptr};

        // General texture
        Texture* m_pTexture           {nullptr};

        // Vehicle
        Texture* m_pTextureDiffuse    {nullptr};
        Texture* m_pTextureGlossiness {nullptr};
        Texture* m_pTextureNormal     {nullptr};
        Texture* m_pTextureSpecular   {nullptr};
        

        // float* m_pDepthBufferPixels{};
        std::vector<float> m_DepthBuffer {};
        bool m_VisualizeDepthBuffer {false};
        bool m_VisualizeBoundingBox {false};
        bool m_VisualizeNormals     {false};
        bool m_Rotate               {false};

        Camera m_Camera        {};
        float  m_RotationAngle {5.0f};
        float  m_RotationSpeed {10.0f};
        float m_AccTime        {0.0f};

        int m_Width{};
        int m_Height{};
    };
}
