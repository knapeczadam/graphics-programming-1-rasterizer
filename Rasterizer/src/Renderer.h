#pragma once

// Project includes
#include "Camera.h"

// Standard includes
#include <cstdint>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

namespace dae
{
    // Forward Declarations
    struct Mesh;
    struct Vertex;
    
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
        
        enum class LightingMode
        {
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

        bool SaveBufferToImage() const;
        inline Camera& GetCamera() { return m_Camera; }
        void ToggleDepthBufferVisibility();
        void ToggleBoundingBoxVisibility();
        void ToggleNormalVisibility();
        void ToggleRotation();
        void CycleShadingMode();

    private:
        void InitializeCamera();
        void InitializeOutputVertices();
        void InitializeTextures();

        // Vertex Transformation
        void TransformFromWorldToScreenV1(const  std::vector<Vertex>& vertices_in, std::vector<Vertex>&     vertices_out) const;
        void TransformFromWorldToScreenV2(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromWorldToScreenV3(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromWorldToScreenV4(const  std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
        void TransformFromNDCtoScreenSpace(const std::vector<Vertex>& vertices_in, std::vector<Vertex>&     vertices_out) const;

        int GetBufferIndex(int x, int y) const;

        void UpdateColor(ColorRGB& finalColor, int px, int py) const;

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
        SDL_Window*  m_pWindow           {nullptr};
        SDL_Surface* m_pFrontBuffer      {nullptr};
        SDL_Surface* m_pBackBuffer       {nullptr};
        SDL_Renderer* m_pRenderer        {nullptr};
        uint32_t*    m_pBackBufferPixels {nullptr};

        // General texture
        Texture* m_pTexture           {nullptr};

        // Material
        Material* m_pMaterial         {nullptr};

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

        Camera  m_Camera           {};
        float   m_RotationAngleDeg {5.0f};
        float   m_RotationAngleRad {1.0f};
        float   m_RotationSpeed    {10.0f};
        float   m_AccTime          {0.0f};
        Matrix  m_Transform        {};
        Vector3 m_Translation      {0.0f,  0.0f, 50.0f};

        int m_Width{};
        int m_Height{};
        
        LightingMode m_CurrentLightingMode {LightingMode::Combined};

        float m_ambient        {0.025f};
        float m_lightDir[3]    {0.577, -0.577f, 0.577};      
        float m_lightIntensity {7.0f};
        float m_kd             {1.0f}; // Diffuse reflection coefficient
        float m_shininess      {25.0f};
    };
}
