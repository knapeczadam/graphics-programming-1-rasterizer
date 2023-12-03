#pragma once

// Project includes
#include "Camera.h"

// Standard includes
#include <cstdint>
#include <vector>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

namespace dae
{
    // Forward Declarations
    struct Mesh;
    struct Vertex;
    struct Vertex_Out;
    
    class Texture;
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
        
        enum class ShadingMode
        {
            BoundingBox = -2,
            DepthBuffer,
            ObservedArea, // Lambert Cosine Law
            Diffuse,
            Specular, 
            Combined // (Diffuse + Specular) * ObservedArea  
        };

    public:
        Renderer(SDL_Window* pWindow);
        Renderer(SDL_Window* pWindow, SDL_Renderer* pRenderer);
        ~Renderer();

        Renderer(const Renderer&)                = delete;
        Renderer(Renderer&&) noexcept            = delete;
        Renderer& operator=(const Renderer&)     = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        void Update(Timer* pTimer);
        void Render();

        // Render helpers
        void SwapBuffers() const;
        void UpdateWindow() const;
        void UpdateRenderer() const;
        void CreateUI();
    
        bool SaveBufferToImage() const;
        inline Camera& GetCamera() { return m_Camera; }

        // Setters
        void ToggleDepthBufferVisibility();
        void ToggleBoundingBoxVisibility();
        void ToggleNormalVisibility();
        void ToggleRotation();
        void CycleShadingMode();

    private:
        // Initialization
        void InitializeCamera();
        void InitializeOutputVertices();
        void InitializeTextures();

        // Vertex Transformation
        void TransformFromWorldToScreenV1(const  std::vector<Vertex>& vertices_in, std::vector<Vertex>&     vertices_out) const;
        void TransformFromWorldToScreenV2(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromWorldToScreenV3(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromWorldToScreenV4(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromNDCtoScreenSpace(const std::vector<Vertex>& vertices_in, std::vector<Vertex>&     vertices_out) const;

        // Helper functions
        int GetBufferIndex(int x, int y) const;
        void UpdateColor(ColorRGB& finalColor, int px, int py) const;
        void UpdateCurrentShadingModeText();

        // Shading
        void ShadePixelV0(const Vertex_Out& vertex, ColorRGB& finalColor) const;
        void ShadePixelV1(const Vertex_Out& vertex, ColorRGB& finalColor, const  ColorRGB& diffuseColor = colors::White) const;
        void ShadePixelV2(const Vertex_Out& vertex, ColorRGB& finalColor, const  ColorRGB& diffuseColor = colors::White, const  ColorRGB& specularColor = colors::White, float glossiness = 0.0f) const;
        void ShadePixelV3(const Vertex_Out& vertex, ColorRGB& finalColor, const  ColorRGB& diffuseColor = colors::White, const  ColorRGB& specularColor = colors::White, float glossiness = 0.0f) const;
        
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
        void Render_W4_TODO_4();
        void Render_W4_TODO_5();
        void Render_W4_TODO_6();

    private:
        SDL_Window*   m_WindowPtr           {nullptr};
        SDL_Surface*  m_FrontBufferPtr      {nullptr};
        SDL_Surface*  m_BackBufferPtr       {nullptr};
        SDL_Renderer* m_RendererPtr         {nullptr};
        uint32_t*     m_BackBufferPixelsPtr {nullptr};

        // General texture
        Texture* m_TexturePtr           {nullptr};

        // Vehicle
        Texture* m_DiffuseTexturePtr    {nullptr};
        Texture* m_GlossinessTexturePtr {nullptr};
        Texture* m_NormalTexturePtr     {nullptr};
        Texture* m_SpecularTexturePtr   {nullptr};

        // float* m_pDepthBufferPixels{};
        std::vector<float> m_DepthBuffer {};

        // Debug
        bool m_UseNormalMap         {false};
        bool m_Rotate               {false};

        Camera  m_Camera           {};
        float   m_RotationAngleDeg {5.0f};
        float   m_RotationAngleRad {1.0f};
        float   m_RotationSpeed    {10.0f};
        float   m_AccTime          {0.0f};
        Matrix  m_Transform        {};
        Vector3 m_Translation      {0.0f,  0.0f, 50.0f};

        int m_Width{};
        int m_Height{};

        ShadingMode m_PreviousShadingMode      {ShadingMode::Combined};
        ShadingMode m_CurrentShadingMode       {ShadingMode::Combined};
        std::string m_CurrentShadingModeAsText {"COMBINED"};

        float m_Ambient           {0.025f};
        float m_LightDirection[3] {0.577f,  -0.577f, 0.577f}; 
        float m_LightIntensity    {7.0f};
        float m_KD                {1.0f};   // Diffuse  reflection coefficient
        float m_Shininess         {25.0f};
    };
}
