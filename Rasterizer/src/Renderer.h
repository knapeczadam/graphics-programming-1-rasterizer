#pragma once

// Project includes
#include "Camera.h"
#include "SceneSelector.h"

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
            Combined, // (Diffuse + Specular) * ObservedArea

            COUNT = 4
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
        
        inline Camera& GetCamera()             { return m_Camera; }
        inline bool IsBenchmarking()     const { return m_StartBenchmark; }
        inline bool IsTakingScreenshot() const { return m_TakeScreenshot; }

        // Setters
        void ToggleDepthBufferVisibility();
        void ToggleBoundingBoxVisibility();
        void ToggleNormalVisibility();
        void ToggleRotation();
        void CycleShadingMode();
        
        inline void StartBenchmark()       { m_StartBenchmark = true;  }
        inline void StopBenchmark()        { m_StartBenchmark = false; }
        inline void TakeScreenshot()       { m_TakeScreenshot = true;  }
        inline void StopTakingScreenshot() { m_TakeScreenshot = false; }

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
        
        // Render helpers
        inline void SwapBuffers()    const;
        inline void UpdateWindow()   const;
        inline void UpdateRenderer() const;
        inline void CreateUI();

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
        inline void Render_W4_TODO_7();

    private:
        SDL_Window*   m_WindowPtr           {nullptr};
        SDL_Surface*  m_FrontBufferPtr      {nullptr};
        SDL_Surface*  m_BackBufferPtr       {nullptr};
        SDL_Renderer* m_RendererPtr         {nullptr};
        uint32_t*     m_BackBufferPixelsPtr {nullptr};

        // General texture
        Texture* m_TexturePtr {nullptr};

        // Vehicle
        Texture* m_DiffuseTexturePtr    {nullptr};
        Texture* m_GlossinessTexturePtr {nullptr};
        Texture* m_NormalTexturePtr     {nullptr};
        Texture* m_SpecularTexturePtr   {nullptr};

        // Path
#if CUSTOM_PATH
        const std::string m_ResourcesPath         {"../_Resources/"};
#else
        const std::string m_ResourcesPath         {"Resources/"};
#endif
        const std::string m_DiffuseTexturePath    {m_ResourcesPath + "vehicle_diffuse.png"};
        const std::string m_GlossinessTexturePath {m_ResourcesPath + "vehicle_gloss.png"};
        const std::string m_NormalTexturePath     {m_ResourcesPath + "vehicle_normal.png"};
        const std::string m_SpecularTexturePath   {m_ResourcesPath + "vehicle_specular.png"};
        const std::string m_TuktukTexturePath     {m_ResourcesPath + "tuktuk.png"};
        const std::string m_UVGridTexturePath     {m_ResourcesPath + "uv_grid.png"};
        const std::string m_UVGrid2TexturePath    {m_ResourcesPath + "uv_grid_2.png"};
        const std::string m_VehiclePath           {m_ResourcesPath + "vehicle.obj"};
        const std::string m_TuktukPath            {m_ResourcesPath + "tuktuk.obj"};

        // float* m_pDepthBufferPixels{};
        std::vector<float> m_DepthBuffer {};

        // Debug
        bool m_UseNormalMap         {true};
        bool m_Rotate               {true};
        bool m_TakeScreenshot       {false};
        bool m_StartBenchmark       {false};

        Camera  m_Camera           {};
        float   m_RotationAngleDeg {5.0f};
        float   m_RotationAngleRad {1.0f};
        float   m_RotationSpeed    {10.0f};
        float   m_AccTime          {0.0f};
        Matrix  m_Transform        {};
        Vector3 m_Translation      {0.0f,  0.0f, 0.0f};

        int   m_Width      {0};
        int   m_Height     {0};
        float m_HalfWidth  {0.0f};
        float m_HalfHeight {0.0f};

        ShadingMode m_PreviousShadingMode      {ShadingMode::Combined};
        ShadingMode m_CurrentShadingMode       {ShadingMode::Combined};
        std::string m_CurrentShadingModeAsText {"COMBINED"};

        float m_Ambient[3]        {0.03f, 0.03f, 0.03f}; // 8, 8, 8
        float m_LightDirection[3] {0.577f,  -0.577f, 0.577f}; 
        float m_LightIntensity    {1.0f};
        float m_KD                {7.0f}; // Diffuse  reflection coefficient
        float m_Shininess         {25.0f};

        float m_BackgroundColor[3] {0.3921f, 0.3921f, 0.3921f}; // 100, 100, 100
    };
}
