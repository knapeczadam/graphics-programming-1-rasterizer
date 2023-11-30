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
#pragma region Global Variables
    const std::vector<Vertex> triangle_vertices_ndc
    {
        {{0.0f, 0.5f, 1.0f}},
        {{0.5f, -0.5f, 1.0f}},
        {{-0.5f, -0.5f, 1.0f}}
    };

    const std::vector<Vertex> triangle_vertices_world_todo_2
    {
        {{0.0f, 2.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}},
        {{-1.0f, 0.0f, 0.0f}}
    };

    const std::vector<Vertex> triangle_vertices_world_todo_3
    {
        {{0.0f, 4.0f, 2.0f}, {1.0f, 0.0f, 0.0f}},
        {{3.0f, -2.0f, 2.0f}, {0.0f, 1.0f, 0.0f}},
        {{-3.0f, -2.0f, 2.0f}, {0.0f, 0.0f, 1.0f}}
    };

    const std::vector<Vertex> triangle_vertices_world_todo_4
    {
        {{0.0f, 2.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.5f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.5f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.0f, 4.0f, 2.0f}, {1.0f, 0.0f, 0.0f}},
        {{3.0f, -2.0f, 2.0f}, {0.0f, 1.0f, 0.0f}},
        {{-3.0f, -2.0f, 2.0f}, {0.0f, 0.0f, 1.0f}}
    };

    std::vector<Mesh> meshes_world_list =
    {
        Mesh
        {
            {
                Vertex{{-3, 3, -2}},
                Vertex{{0, 3, -2}},
                Vertex{{3, 3, -2}},
                Vertex{{-3, 0, -2}},
                Vertex{{0, 0, -2}},
                Vertex{{3, 0, -2}},
                Vertex{{-3, -3, -2}},
                Vertex{{0, -3, -2}},
                Vertex{{3, -3, -2}}
            },
            {
                3, 0, 1,    1, 4, 3,    4, 1, 2,
                2, 5, 4,    6, 3, 4,    4, 7, 6,
                7, 4, 5,    5, 8, 7
            },
            PrimitiveTopology::TriangleList
        },
    };
    
    std::vector<Mesh> meshes_world_list_transformed;

    std::vector<Mesh> meshes_world_strip
    {
        Mesh
        {
            {
                Vertex{{-3, 3, -2}, {},{0, 0} },
                Vertex{{0, 3, -2}, {}, {0.5f, 0}},
                Vertex{{3, 3, -2}, {}, {1, 0}},
                Vertex{{-3, 0, -2}, {}, {0, 0.5f}},
                Vertex{{0, 0, -2}, {}, {0.5f, 0.5f}},
                Vertex{{3, 0, -2}, {}, {1, 0.5f}},
                Vertex{{-3, -3, -2}, {}, {0, 1}},
                Vertex{{0, -3, -2}, {}, {0.5f, 1}},
                Vertex{{3, -3, -2}, {}, {1, 1}}
            },
            {
                3, 0, 4, 1, 5, 2,
                2, 6,
                6, 3, 7, 4, 8, 5
            },
            PrimitiveTopology::TriangleStrip
        }
    };

    // SS = Screen Space
    std::vector<Vertex> vertices_ss{};
    std::vector<Vertex_Out> vertices_ss_out{};

    std::array<float, 3> weights{};
#pragma endregion

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
        m_DepthBuffer.resize(m_Width * m_Height);

        //Initialize Camera
        InitializeCamera();
        InitializeOutputVertices();
        InitializeTextures();

        // --- ASSERTS ---
        assert(not meshes_world_list.empty() and "Meshes list is empty");
        assert(not meshes_world_strip.empty() and "Meshes strip is empty");
    }

    Renderer::~Renderer()
    {
        //delete[] m_pDepthBufferPixels;
        delete m_pTexture;

        // Vehicle
        delete m_pTextureDiffuse;
        delete m_pTextureGlossiness;
        delete m_pTextureNormal;
        delete m_pTextureSpecular;
    }

    void Renderer::Update(Timer* pTimer)
    {
        m_Camera.Update(pTimer);
        
        // --- WEEK 3 ---
#if W3
#if TODO_4
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
        }
#elif TODO_5
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
        }
#elif TODO_6
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
        }
#endif
#endif

        // --- WEEK 4 ---
#if W4
#if TODO_0
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
        }
#elif TODO_1
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
            meshes_world_list_transformed[0].vertices[idx].normal = rotMatrix.TransformVector(meshes_world_list[0].vertices[idx].normal);
        }
#elif TODO_2
        if (not m_Rotate) return;
        m_AccTime += pTimer->GetElapsed();
        const float yaw{m_RotationAngle * TO_RADIANS * m_RotationSpeed * m_AccTime};
        const auto rotMatrix{Matrix::CreateRotationY(yaw)};
        for (size_t idx{0}; idx < meshes_world_list[0].vertices.size(); ++idx)
        {
            meshes_world_list_transformed[0].vertices[idx].position = rotMatrix.TransformPoint(meshes_world_list[0].vertices[idx].position);
            meshes_world_list_transformed[0].vertices[idx].normal = rotMatrix.TransformVector(meshes_world_list[0].vertices[idx].normal);
            meshes_world_list_transformed[0].vertices[idx].tangent = rotMatrix.TransformVector(meshes_world_list[0].vertices[idx].tangent);
        }
#endif
#endif
    }

    void Renderer::Render()
    {
        //@START
        //Lock BackBuffer
        SDL_LockSurface(m_pBackBuffer);

        // --- WEEK 1 ---
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

        // --- WEEK 2 ---
#elif W2
#if TODO_1
        Render_W2_TODO_1();
#elif TODO_2
        Render_W2_TODO_2();
#elif TODO_3
        Render_W2_TODO_3();
#elif TODO_4
        Render_W2_TODO_4();
#elif TODO_5
        Render_W2_TODO_5();
#endif

        // --- WEEK 3 ---
#elif W3
#if TODO_0
        Render_W3_TODO_0();
#elif TODO_1
        Render_W3_TODO_1();
#elif TODO_2
        Render_W3_TODO_2();
#elif TODO_3
        Render_W3_TODO_3();
#elif TODO_4
        Render_W3_TODO_4();
#elif TODO_5
        Render_W3_TODO_5();
#elif TODO_6
        Render_W3_TODO_6();
#endif

        // --- WEEK 4 ---
#elif W4
#if TODO_0
        Render_W4_TODO_0();
#elif TODO_1
        Render_W4_TODO_1();
#elif TODO_2
        Render_W4_TODO_2();
#elif TODO_3
        Render_W4_TODO_3();
#endif
#endif

        //@END
        //Update SDL Surface
        SDL_UnlockSurface(m_pBackBuffer);
        SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::ToggleDepthBufferVisibility()
    {
        m_VisualizeDepthBuffer = not m_VisualizeDepthBuffer;
    }

    void Renderer::ToggleBoundingBoxVisibility()
    {
        m_VisualizeBoundingBox = not m_VisualizeBoundingBox;
    }

    void Renderer::ToggleNormalVisibility()
    {
        m_VisualizeNormals = not m_VisualizeNormals;
    }

    void Renderer::ToggleRotation()
    {
        m_Rotate = not m_Rotate;
    }

#pragma region Initialization
    void Renderer::InitializeCamera()
    {
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        m_Camera.SetAspectRatio(aspectRatio);
        
        // --- WEEK 1 ---
#if W1
#if TODO_0
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_1
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_2
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_3
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_4
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_5
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#endif

        // --- WEEK 2 ---
#elif W2
#if TODO_1
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_2
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_3
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_4
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_5
#endif

        // --- WEEK 3 ---
#elif W3
#if TODO_0
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f}); 
#elif TODO_1
        m_Camera.Initialize(60.f, {0.0f, 0.0f, -10.0f}); 
#elif TODO_2
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f}); 
#elif TODO_3
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f});
#elif TODO_4
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f});
#elif TODO_5
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f});
#elif TODO_6
        m_Camera.Initialize(60.f, {0.0f, 5.0f, -30.0f});
#endif

        // --- WEEK 4 ---
#elif W4
#if TODO_0
        // Camera [ Position {0.f, 5.f, 64.f}, FOV angle {45}, Near {.1f}, Far {100.f} ]
        m_Camera.Initialize(45.0f, {0.0f, 5.0f, -64.0f}, 0.1f, 100.0f);
#elif TODO_1
        m_Camera.Initialize(45.0f, {0.0f, 5.0f, -64.0f}, 0.1f, 100.0f);
#elif TODO_2
        m_Camera.Initialize(45.0f, {0.0f, 5.0f, -64.0f}, 0.1f, 100.0f);
#elif TODO_3
#endif
#endif
    }

    void Renderer::InitializeOutputVertices()
    {
        // --- OBJECTS ---
        // Tuktuk
        // Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_strip[0].vertices, meshes_world_strip[0].indices);
        
        // Vehicle
        // Utils::ParseOBJ("Resources/vehicle.obj", meshes_world_strip[0].vertices, meshes_world_strip[0].indices);

        // --- WEEK 1 ---
#if W1
#if TODO_0
#elif TODO_1
        vertices_ss.resize(triangle_vertices_ndc.size());
#elif TODO_2
        vertices_ss.resize(triangle_vertices_world_todo_2.size());
#elif TODO_3
        vertices_ss.resize(triangle_vertices_world_todo_3.size());
#elif TODO_4
        vertices_ss.resize(triangle_vertices_world_todo_4.size());
#elif TODO_5
        vertices_ss.resize(triangle_vertices_world_todo_4.size());
#endif

        // --- WEEK 2 ---
#elif W2
#if TODO_1
        vertices_ss.resize(meshes_world_list[0].vertices.size());
#elif TODO_2
        vertices_ss.resize(meshes_world_strip[0].vertices.size());
#elif TODO_3
        vertices_ss.resize(meshes_world_strip[0].vertices.size());
#elif TODO_4
        vertices_ss.resize(meshes_world_strip[0].vertices.size());
#elif TODO_5
        vertices_ss.resize(meshes_world_strip[0].vertices.size());
#endif

        // --- WEEK 3 ---
#elif W3
#if TODO_0
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        vertices_ss.resize(meshes_world_list[0].vertices.size());
#elif TODO_1
        vertices_ss_out.resize(meshes_world_strip[0].vertices.size());
#elif TODO_2
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_3
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_4
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_5
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_6
        Utils::ParseOBJ("Resources/tuktuk.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#endif

        // --- WEEK 4 ---
#elif W4
#if TODO_0
        Utils::ParseOBJ("Resources/vehicle.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_1
        Utils::ParseOBJ("Resources/vehicle.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_2
        Utils::ParseOBJ("Resources/vehicle.obj", meshes_world_list[0].vertices, meshes_world_list[0].indices);
        meshes_world_list_transformed = meshes_world_list;
        vertices_ss_out.resize(meshes_world_list[0].vertices.size());
#elif TODO_3
#endif
#endif
    }

    void Renderer::InitializeTextures()
    {
        // --- TEXTURES ---
        // UV
        // m_pTexture = Texture::LoadFromFile("Resources/uv_grid.png");
        // m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
        
        // Tuktuk
        // m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");

       // Vehicle
        // m_pTextureDiffuse = Texture::LoadFromFile("Resources/vehicle_diffuse.png");
        // m_pTextureGlossiness = Texture::LoadFromFile("Resources/vehicle_gloss.png");
        // m_pTextureNormal = Texture::LoadFromFile("Resources/vehicle_normal.png");
        // m_pTextureSpecular = Texture::LoadFromFile("Resources/vehicle_specular.png");
        
        // --- WEEK 1 ---
#if W1
#if TODO_0
#elif TODO_1
#elif TODO_2
#elif TODO_3
#elif TODO_4
#elif TODO_5
#endif

        // --- WEEK 2 ---
#elif W2
#if TODO_1
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#elif TODO_2
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#elif TODO_3
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#elif TODO_4
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#elif TODO_5
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#endif

        // --- WEEK 3 ---
#elif W3
#if TODO_0
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#elif TODO_1
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");
#elif TODO_2
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#elif TODO_3
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#elif TODO_4
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#elif TODO_5
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#elif TODO_6
        m_pTexture = Texture::LoadFromFile("Resources/tuktuk.png");
#endif

        // --- WEEK 4 ---
#elif W4
#if TODO_0
        m_pTextureDiffuse = Texture::LoadFromFile("Resources/vehicle_diffuse.png");
#elif TODO_1
#elif TODO_2
        m_pTextureNormal = Texture::LoadFromFile("Resources/vehicle_normal.png");
#elif TODO_3
#endif
#endif
    }
#pragma endregion

#pragma region Transformations
    /**
     * \brief Without Projection Matrix
     * \param vertices_in 
     * \param vertices_out 
     */
    void Renderer::VertexTransformationFromWorldToScreenV1(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex& vertex_out = vertices_out[i];

            // MODEL/OBJECT
            const Vector4 v4{vertex_in.position.x, vertex_in.position.y, vertex_in.position.z, 1.0f};
            // WORLD -> VIEW
            const Vector4 v4_view = m_Camera.invViewMatrix.TransformPoint(v4);
            // PROJECTION
            vertex_out.position.x = v4_view.x / v4_view.z;
            vertex_out.position.y = v4_view.y / v4_view.z;
            // NDC
            vertex_out.position.x = vertex_out.position.x / (m_Camera.GetFOV() * m_Camera.GetAspectRatio());
            vertex_out.position.y = vertex_out.position.y / m_Camera.GetFOV();
            // SCREEN
            vertex_out.position.x = (vertex_out.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_out.position.y) * 0.5f * static_cast<float>(m_Height);
            // DEPTH
            assert(v4_view.z != 0.0f and "Renderer::VertexTransformationFromWorldToScreen: Division by zero");
            vertex_out.position.z = v4_view.z;
            // UV
            vertex_out.uv = vertex_in.uv;
        }
    }

    /**
     * \brief With Projection Matrix
     * \param vertices_in 
     * \param vertices_out 
     */
    void Renderer::VertexTransformationFromWorldToScreenV2(const std::vector<Vertex>& vertices_in,
                                                           std::vector<Vertex_Out>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex_Out& vertex_out = vertices_out[i];

            // MODEL/OBJECT
            const Vector4 v4{vertex_in.position.x, vertex_in.position.y, vertex_in.position.z, 1.0f};
            // WORLD -> VIEW - PROJECTION
            const Vector4 v4_proj = (m_Camera.invViewMatrix * m_Camera.projectionMatrix).TransformPoint(v4);
            // NDC
            vertex_out.position.x = v4_proj.x / v4_proj.w;
            vertex_out.position.y = v4_proj.y / v4_proj.w;
            vertex_out.position.z = v4_proj.z / v4_proj.w;
            // SCREEN
            vertex_out.position.x = (vertex_out.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_out.position.y) * 0.5f * static_cast<float>(m_Height);
            // DEPTH
            assert(v4_proj.w != 0.0f and "Renderer::VertexTransformationFromWorldToScreenV2: Division by zero");
            vertex_out.position.w = v4_proj.w;
            // UV
            vertex_out.uv = vertex_in.uv;
        }
    }

    /**
     * \brief Optimized version
     * \param vertices_in 
     * \param vertices_out 
     */
    void Renderer::VertexTransformationFromWorldToScreenV3(const std::vector<Vertex>& vertices_in,
                                                           std::vector<Vertex_Out>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex_Out& vertex_out = vertices_out[i];

            // MODEL/OBJECT
            const Vector4 positionIn{vertex_in.position.x, vertex_in.position.y, vertex_in.position.z, 1.0f};
            // WORLD -> VIEW - PROJECTION
            const Vector4 projectedPos = (m_Camera.invViewMatrix * m_Camera.projectionMatrix).TransformPoint(positionIn);
            // DEPTH
            assert(projectedPos.w != 0.0f and "Renderer::VertexTransformationFromWorldToScreenV3: Division by zero");
            vertex_out.position.w = 1.0f / projectedPos.w;
            // NDC
            vertex_out.position.x = projectedPos.x * vertex_out.position.w;
            vertex_out.position.y = projectedPos.y * vertex_out.position.w;
            vertex_out.position.z = projectedPos.z * vertex_out.position.w;
            vertex_out.position.z = 1.0f / vertex_out.position.z;
            // SCREEN
            vertex_out.position.x = (vertex_out.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_out.position.y) * 0.5f * static_cast<float>(m_Height);
            // UV
            vertex_out.uv = vertex_in.uv;
        }
    }

    /**
     * \brief Optimized version with normal calculation
     * \param vertices_in 
     * \param vertices_out 
     */
    void Renderer::VertexTransformationFromWorldToScreenV4(const std::vector<Vertex>& vertices_in,
                                                           std::vector<Vertex_Out>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex_Out& vertex_out = vertices_out[i];

            // MODEL/OBJECT
            const Vector4 positionIn{vertex_in.position.x, vertex_in.position.y, vertex_in.position.z, 1.0f};
            // WORLD -> VIEW -> PROJECTION
            const Vector4 projectedPos = (m_Camera.invViewMatrix * m_Camera.projectionMatrix).TransformPoint(positionIn);
            // DEPTH
            assert(projectedPos.w != 0.0f and "Renderer::VertexTransformationFromWorldToScreenV4: Division by zero");
            vertex_out.position.w = 1.0f / projectedPos.w;
            // NDC
            vertex_out.position.x = projectedPos.x * vertex_out.position.w;
            vertex_out.position.y = projectedPos.y * vertex_out.position.w;
            vertex_out.position.z = projectedPos.z * vertex_out.position.w;
            vertex_out.position.z = 1.0f / vertex_out.position.z;
            // SCREEN
            vertex_out.position.x = (vertex_out.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_out.position.y) * 0.5f * static_cast<float>(m_Height);
            // UV
            vertex_out.uv = vertex_in.uv;
            // WORLD NORMAL
            vertex_out.normal = vertex_in.normal;
            // WORLD TANGENT
            vertex_out.tangent = vertex_in.tangent;
        }
    }

    void Renderer::VertexTransformationFromNDCtoScreenSpace(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex& vertex_out = vertices_out[i];

            // From NDC to Screen Space
            vertex_out.position.x = (vertex_in.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_in.position.y) * 0.5f * static_cast<float>(m_Height);
        }
    }
#pragma endregion

#pragma region Helper Functions
    int Renderer::GetBufferIndex(int x, int y) const
    {
        return x + (y * m_Width);
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

    void Renderer::PixelShadingV0(const Vertex_Out& vertex, ColorRGB& finalColor) const
    {
        // Normalized light direction
        const Vector3 lighDirection{0.577f, -0.577f, 0.577f};

        // Observed area
        const float observedArea{Vector3::Dot(vertex.normal, -lighDirection)};

        if (observedArea < 0) return;
        
        finalColor = ColorRGB{observedArea, observedArea, observedArea};
    }

    void Renderer::PixelShadingV1(const Vertex_Out& vertex, ColorRGB& finalColor, const ColorRGB& diffuseColor) const
    {
        // Normalized light direction
        const Vector3 lighDirection{0.577f, -0.577f, 0.577f};

        // Observed area
        const float observedArea{Vector3::Dot(vertex.normal, -lighDirection)};

        if (observedArea < 0) return;

        // Lambert
        constexpr float kd{7.0f}; // Light intensity
        finalColor = diffuseColor * kd / PI;

        // Combined
        finalColor *= observedArea;
    }

    bool Renderer::SaveBufferToImage() const
    {
        return SDL_SaveBMP(m_pBackBuffer, "Rasterizer_ColorBuffer.bmp");
    }
#pragma endregion

#pragma region Week 1
    void Renderer::Render_W1_TODO_0() const
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
    void Renderer::Render_W1_TODO_1() const
    {
        VertexTransformationFromNDCtoScreenSpace(triangle_vertices_ndc, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};

                // Point - Triangle test
                if (IsPointInTriangleFast(pixel, v0, v1, v2))
                {
                    finalColor = colors::White;
                }
                UpdateColor(finalColor, px, py);
            }
        }
    }

    /**
     * \brief First Project the given triangle (now, the vertices
              are defined in WORLD space). Rasterization stays
              the same as the previous step.\n\n
              Create a separate function that transforms a
              vector of WORLD space vertices to a vector of
              NDC space vertices (or directly to SCREEN
              space).
     */
    void Renderer::Render_W1_TODO_2() const
    {
        VertexTransformationFromWorldToScreenV1(triangle_vertices_world_todo_2, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};

                // Point - Triangle test
                if (IsPointInTriangle(pixel, v0, v1, v2))
                {
                    finalColor = colors::White;
                }
                UpdateColor(finalColor, px, py);
            }
        }
    }

    void Renderer::Render_W1_TODO_3() const
    {
        VertexTransformationFromWorldToScreenV1(triangle_vertices_world_todo_3, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};

                // Point - Triangle test
                if (IsPointInTriangle(pixel, v0, v1, v2, weights))
                {
                    finalColor = triangle_vertices_world_todo_3[0].color * weights[0] +
                                 triangle_vertices_world_todo_3[1].color * weights[1] +
                                 triangle_vertices_world_todo_3[2].color * weights[2];
                }
                UpdateColor(finalColor, px, py);
            }
        }
    }

    void Renderer::Render_W1_TODO_4()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(triangle_vertices_world_todo_4, vertices_ss);

        for (size_t triangleIdx{0}; triangleIdx < vertices_ss.size(); triangleIdx += 3)
        {
            const Vector2 v0{vertices_ss[triangleIdx].position.GetXY()};
            const Vector2 v1{vertices_ss[triangleIdx + 1].position.GetXY()};
            const Vector2 v2{vertices_ss[triangleIdx + 2].position.GetXY()};

            for (int px{0}; px < m_Width; ++px)
            {
                for (int py{0}; py < m_Height; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, v0, v1, v2, weights))
                    {
                        // Depth
                        const float depth = vertices_ss[triangleIdx].position.z * weights[0] +
                                        vertices_ss[triangleIdx + 1].position.z * weights[1] +
                                        vertices_ss[triangleIdx + 2].position.z * weights[2];
                        
                        // Z-test
                        if (depth < m_DepthBuffer[px + (py * m_Width)])
                        {
                            m_DepthBuffer[px + (py * m_Width)] = depth;

                            // Color
                            finalColor = triangle_vertices_world_todo_4[triangleIdx].color * weights[0] +
                                     triangle_vertices_world_todo_4[triangleIdx + 1].color * weights[1] +
                                     triangle_vertices_world_todo_4[triangleIdx + 2].color * weights[2];
                            UpdateColor(finalColor, px, py);
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W1_TODO_5()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(triangle_vertices_world_todo_4, vertices_ss);

        for (size_t triangleIdx{0}; triangleIdx < vertices_ss.size(); triangleIdx += 3)
        {
            const Vector2 v0{vertices_ss[triangleIdx].position.GetXY()};
            const Vector2 v1{vertices_ss[triangleIdx + 1].position.GetXY()};
            const Vector2 v2{vertices_ss[triangleIdx + 2].position.GetXY()};

            // Create bounding box
            int minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
            int maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
            int minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));
            int maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));

            // Clamp bounding box to screen
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, v0, v1, v2, weights))
                    {
                        // Depth
                        const float depth = vertices_ss[triangleIdx].position.z * weights[0] +
                                        vertices_ss[triangleIdx + 1].position.z * weights[1] +
                                        vertices_ss[triangleIdx + 2].position.z * weights[2];

                        // Z-test
                        if (depth < m_DepthBuffer[px + (py * m_Width)])
                        {
                            m_DepthBuffer[px + (py * m_Width)] = depth;

                            // Color
                            finalColor = triangle_vertices_world_todo_4[triangleIdx].color * weights[0] +
                                     triangle_vertices_world_todo_4[triangleIdx + 1].color * weights[1] +
                                     triangle_vertices_world_todo_4[triangleIdx + 2].color * weights[2];
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }
#pragma endregion

#pragma region Week 2
    void Renderer::Render_W2_TODO_1()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));
        
        VertexTransformationFromWorldToScreenV1(meshes_world_list[0].vertices, vertices_ss);

        for (size_t idx{0}; idx < meshes_world_list[0].indices.size(); idx += 3)
        {
            // Indices triplets
            const uint32_t idx0{meshes_world_list[0].indices[idx]};
            const uint32_t idx1{meshes_world_list[0].indices[idx + 1]};
            const uint32_t idx2{meshes_world_list[0].indices[idx + 2]};

            // Triangle's vertices
            const Vertex& v0{vertices_ss[idx0]};
            const Vertex& v1{vertices_ss[idx1]};
            const Vertex& v2{vertices_ss[idx2]};

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};
            
            // Create bounding box
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x)))};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x)))};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y)))};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y)))};

            // Clamp bounding box to screen
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);
            
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Color
                            finalColor = colors::White;
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W2_TODO_2()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(meshes_world_strip[0].vertices, vertices_ss);

        const std::vector<uint32_t>& indices{meshes_world_strip[0].indices};
        for (size_t idx{0}; idx < indices.size() - 2; ++idx)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex v0;
            Vertex v1;
            Vertex v2;

            // Check for degenarate triangles
            if (idx0 == idx1 or idx1 == idx2 or idx2 == idx0) continue;
                
            v0 = vertices_ss[idx0];

            // idx is odd
            if (idx & 1)
            {
                v1 = vertices_ss[idx2];
                v2 = vertices_ss[idx1];
            }
            else
            {
                v1 = vertices_ss[idx1];
                v2 = vertices_ss[idx2];
            }

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};

            // Create bounding box
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x)))};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x)))};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y)))};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y)))};

            // Clamp bounding box to screen
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Color
                            finalColor = colors::White;
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W2_TODO_3()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(meshes_world_strip[0].vertices, vertices_ss);

        const std::vector<uint32_t>& indices{meshes_world_strip[0].indices};
        for (size_t idx{0}; idx < indices.size() - 2; ++idx)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex v0;
            Vertex v1;
            Vertex v2;

            // Check for degenarate triangles
            if (idx0 == idx1 or idx1 == idx2 or idx2 == idx0) continue;
                
            v0 = vertices_ss[idx0];

            // idx is odd
            if (idx & 1)
            {
                v1 = vertices_ss[idx2];
                v2 = vertices_ss[idx1];
            }
            else
            {
                v1 = vertices_ss[idx1];
                v2 = vertices_ss[idx2];
            }

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};

            // Create bounding box
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x)))};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x)))};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y)))};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y)))};

            // Clamp bounding box to screen
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};

                        // Interpolate UV
                        const Vector2 uv {v0.uv * weights[0] + v1.uv * weights[1] + v2.uv * weights[2]};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Color
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W2_TODO_4()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(meshes_world_strip[0].vertices, vertices_ss);

        const std::vector<uint32_t>& indices{meshes_world_strip[0].indices};
        for (size_t idx{0}; idx < indices.size() - 2; ++idx)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex v0;
            Vertex v1;
            Vertex v2;

            // Check for degenarate triangles
            if (idx0 == idx1 or idx1 == idx2 or idx2 == idx0) continue;
                
            v0 = vertices_ss[idx0];

            // idx is odd
            if (idx & 1)
            {
                v1 = vertices_ss[idx2];
                v2 = vertices_ss[idx1];
            }
            else
            {
                v1 = vertices_ss[idx1];
                v2 = vertices_ss[idx2];
            }

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};

            // Create bounding box
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x)))};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x)))};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y)))};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y)))};

            // Clamp bounding box to screen
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate depth
                        const float weightedV0Depth{1.0f / pos0.z * weights[0]};
                        const float weightedV1Depth{1.0f / pos1.z * weights[1]};
                        const float weightedV2Depth{1.0f / pos2.z * weights[2]};
                        const float depth{1.0f / (weightedV0Depth + weightedV1Depth + weightedV2Depth)};

                        // Interpolate UV
                        const Vector2 weightedV0UV{v0.uv / pos0.z * weights[0]};
                        const Vector2 weightedV1UV{v1.uv / pos1.z * weights[1]};
                        const Vector2 weightedV2UV{v2.uv / pos2.z * weights[2]};
                        const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * depth};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Color
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W2_TODO_5()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(meshes_world_strip[0].vertices, vertices_ss);

        const std::vector<uint32_t>& indices{meshes_world_strip[0].indices};
        for (size_t idx{0}; idx < indices.size() - 2; ++idx)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex v0;
            Vertex v1;
            Vertex v2;

            // Check for degenarate triangles
            if (idx0 == idx1 or idx1 == idx2 or idx2 == idx0) continue;
                
            v0 = vertices_ss[idx0];

            // idx is odd
            if (idx & 1)
            {
                v1 = vertices_ss[idx2];
                v2 = vertices_ss[idx1];
            }
            else
            {
                v1 = vertices_ss[idx1];
                v2 = vertices_ss[idx2];
            }

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box to screen + stretch by 1 pixel
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate depth
                        const float weightedV0Depth{1.0f / pos0.z * weights[0]};
                        const float weightedV1Depth{1.0f / pos1.z * weights[1]};
                        const float weightedV2Depth{1.0f / pos2.z * weights[2]};
                        const float depth{1.0f / (weightedV0Depth + weightedV1Depth + weightedV2Depth)};

                        // Interpolate UV
                        const Vector2 weightedV0UV{v0.uv / pos0.z * weights[0]};
                        const Vector2 weightedV1UV{v1.uv / pos1.z * weights[1]};
                        const Vector2 weightedV2UV{v2.uv / pos2.z * weights[2]};
                        const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * depth};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Color
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }
#pragma endregion

#pragma region Week 3
    void Renderer::Render_W3_TODO_0()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV1(meshes_world_list[0].vertices, vertices_ss);

        const std::vector<uint32_t>& indices{meshes_world_list[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex& v0{vertices_ss[idx0]};
            Vertex& v1{vertices_ss[idx1]};
            Vertex& v2{vertices_ss[idx2]};

            // Triangle's vertices' positions
            const Vector3& pos0{v0.position};
            const Vector3& pos1{v1.position};
            const Vector3& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box to screen + stretch by 1 pixel
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate depth
                        const float weightedV0Depth{1.0f / pos0.z * weights[0]};
                        const float weightedV1Depth{1.0f / pos1.z * weights[1]};
                        const float weightedV2Depth{1.0f / pos2.z * weights[2]};
                        const float depth{1.0f / (weightedV0Depth + weightedV1Depth + weightedV2Depth)};

                        // Interpolate UV
                        const Vector2 weightedV0UV{v0.uv / pos0.z * weights[0]};
                        const Vector2 weightedV1UV{v1.uv / pos1.z * weights[1]};
                        const Vector2 weightedV2UV{v2.uv / pos2.z * weights[2]};
                        const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * depth};

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (depth < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = depth;

                            // Diffuse
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_1()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV2(meshes_world_strip[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_strip[0].indices};
        for (size_t idx{0}; idx < indices.size() - 2; ++idx)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out v0;
            Vertex_Out v1;
            Vertex_Out v2;

            // Check for degenarate triangles
            if (idx0 == idx1 or idx1 == idx2 or idx2 == idx0) continue;
                
            v0 = vertices_ss_out[idx0];

            // idx is odd
            if (idx & 1)
            {
                v1 = vertices_ss_out[idx2];
                v2 = vertices_ss_out[idx1];
            }
            else
            {
                v1 = vertices_ss_out[idx1];
                v2 = vertices_ss_out[idx2];
            }

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box to screen + stretch by 1 pixel
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer
                        const float weightedZBufferV0{1.0f / pos0.z * weights[0]};
                        const float weightedZBufferV1{1.0f / pos1.z * weights[1]};
                        const float weightedZBufferV2{1.0f / pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth
                            const float weightedViewSpaceDepthV0{1.0f / pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{1.0f / pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{1.0f / pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV
                            const Vector2 weightedV0UV{v0.uv / pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv / pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv / pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_2()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV2(meshes_world_list[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer
                        const float weightedZBufferV0{1.0f / pos0.z * weights[0]};
                        const float weightedZBufferV1{1.0f / pos1.z * weights[1]};
                        const float weightedZBufferV2{1.0f / pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth
                            const float weightedViewSpaceDepthV0{1.0f / pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{1.0f / pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{1.0f / pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV
                            const Vector2 weightedV0UV{v0.uv / pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv / pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv / pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            finalColor = m_pTexture->Sample(uv);
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_3()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV2(meshes_world_list[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            minX = std::max(minX, 0);
            maxX = std::min(maxX, m_Width - 1);
            minY = std::max(minY, 0);
            maxY = std::min(maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer
                        const float weightedZBufferV0{1.0f / pos0.z * weights[0]};
                        const float weightedZBufferV1{1.0f / pos1.z * weights[1]};
                        const float weightedZBufferV2{1.0f / pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;
                            
                            // Interpolate View Space depth
                            const float weightedViewSpaceDepthV0{1.0f / pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{1.0f / pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{1.0f / pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV
                            const Vector2 weightedV0UV{v0.uv / pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv / pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv / pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};

                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                finalColor = m_pTexture->Sample(uv);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_4()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV2(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer
                        const float weightedZBufferV0{1.0f / pos0.z * weights[0]};
                        const float weightedZBufferV1{1.0f / pos1.z * weights[1]};
                        const float weightedZBufferV2{1.0f / pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth
                            const float weightedViewSpaceDepthV0{1.0f / pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{1.0f / pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{1.0f / pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV
                            const Vector2 weightedV0UV{v0.uv / pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv / pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv / pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                finalColor = m_pTexture->Sample(uv);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_5()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV2(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            ColorRGB finalColor{colors::Black};
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    if (m_VisualizeBoundingBox)
                    {
                        finalColor = colors::White;
                        UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        continue;
                    }
                    
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer
                        const float weightedZBufferV0{1.0f / pos0.z * weights[0]};
                        const float weightedZBufferV1{1.0f / pos1.z * weights[1]};
                        const float weightedZBufferV2{1.0f / pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth
                            const float weightedViewSpaceDepthV0{1.0f / pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{1.0f / pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{1.0f / pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV
                            const Vector2 weightedV0UV{v0.uv / pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv / pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv / pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                finalColor = m_pTexture->Sample(uv);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W3_TODO_6()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV3(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            ColorRGB finalColor{colors::Black};
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    if (m_VisualizeBoundingBox)
                    {
                        finalColor = colors::White;
                        UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        continue;
                    }
                    
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer - optimized
                        const float weightedZBufferV0{pos0.z * weights[0]};
                        const float weightedZBufferV1{pos1.z * weights[1]};
                        const float weightedZBufferV2{pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth - optimized
                            const float weightedViewSpaceDepthV0{pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV - optimized
                            const Vector2 weightedV0UV{v0.uv * pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv * pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv * pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                finalColor = m_pTexture->Sample(uv);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }
#pragma endregion

#pragma region Week 4
    void Renderer::Render_W4_TODO_0()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV3(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            ColorRGB finalColor{colors::Black};
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    if (m_VisualizeBoundingBox)
                    {
                        finalColor = colors::White;
                        UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        continue;
                    }
                    
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer - optimized
                        const float weightedZBufferV0{pos0.z * weights[0]};
                        const float weightedZBufferV1{pos1.z * weights[1]};
                        const float weightedZBufferV2{pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth - optimized
                            const float weightedViewSpaceDepthV0{pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV - optimized
                            const Vector2 weightedV0UV{v0.uv * pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv * pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv * pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                finalColor = m_pTextureDiffuse->Sample(uv);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }
    
    void Renderer::Render_W4_TODO_1()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV4(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            ColorRGB finalColor{colors::Black};
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    if (m_VisualizeBoundingBox)
                    {
                        finalColor = colors::White;
                        UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        continue;
                    }
                    
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer - optimized
                        const float weightedZBufferV0{pos0.z * weights[0]};
                        const float weightedZBufferV1{pos1.z * weights[1]};
                        const float weightedZBufferV2{pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth - optimized
                            const float weightedViewSpaceDepthV0{pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV - optimized
                            const Vector2 weightedV0UV{v0.uv * pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv * pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv * pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};

                            // Interpolate Normal + Normalization
                            const Vector3 weightedV0Normal{v0.normal * weights[0]};
                            const Vector3 weightedV1Normal{v1.normal * weights[1]};
                            const Vector3 weightedV2Normal{v2.normal * weights[2]};
                            const Vector3 normal{(weightedV0Normal + weightedV1Normal + weightedV2Normal).Normalized()};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                // Pixel vertex
                                Vertex_Out pixelVertex;
                                pixelVertex.normal = normal;
                                
                                // Final shading
                                PixelShadingV0(pixelVertex, finalColor);
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W4_TODO_2()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

        VertexTransformationFromWorldToScreenV4(meshes_world_list_transformed[0].vertices, vertices_ss_out);

        const std::vector<uint32_t>& indices{meshes_world_list_transformed[0].indices};
        for (size_t idx{0}; idx < indices.size(); idx+=3)
        {
            // Triangle's indices
            const uint32_t idx0{indices[idx]};
            const uint32_t idx1{indices[idx + 1]};
            const uint32_t idx2{indices[idx + 2]};

            // Triangle's vertices
            Vertex_Out& v0{vertices_ss_out[idx0]};
            Vertex_Out& v1{vertices_ss_out[idx1]};
            Vertex_Out& v2{vertices_ss_out[idx2]};

            // Triangle's vertices' positions
            const Vector4& pos0{v0.position};
            const Vector4& pos1{v1.position};
            const Vector4& pos2{v2.position};

            // Create bounding box + stretch by 1 pixel
            constexpr int offset{1};
            const int minX {static_cast<int>(std::min(pos0.x, std::min(pos1.x, pos2.x))) - offset};
            const int maxX {static_cast<int>(std::max(pos0.x, std::max(pos1.x, pos2.x))) + offset};
            const int minY {static_cast<int>(std::min(pos0.y, std::min(pos1.y, pos2.y))) - offset};
            const int maxY {static_cast<int>(std::max(pos0.y, std::max(pos1.y, pos2.y))) + offset};

            // Clamp bounding box
            if (minX < 0)         continue;
            if (maxX >= m_Width)  continue;
            if (minY < 0)         continue;
            if (maxY >= m_Height) continue;

            ColorRGB finalColor{colors::Black};
            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    if (m_VisualizeBoundingBox)
                    {
                        finalColor = colors::White;
                        UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        continue;
                    }
                    
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    // Point - Triangle test
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate Z-Buffer - optimized
                        const float weightedZBufferV0{pos0.z * weights[0]};
                        const float weightedZBufferV1{pos1.z * weights[1]};
                        const float weightedZBufferV2{pos2.z * weights[2]};
                        const float interpolatedZBuffer{1.0f / (weightedZBufferV0 + weightedZBufferV1 + weightedZBufferV2)};

                        // Frustum culling
                        if (interpolatedZBuffer < 0.0f or interpolatedZBuffer > 1.0f) continue;

                        // Z-test
                        const int bufferIdx {GetBufferIndex(px, py)};
                        if (interpolatedZBuffer < m_DepthBuffer[bufferIdx])
                        {
                            m_DepthBuffer[bufferIdx] = interpolatedZBuffer;

                            // Interpolate View Space depth - optimized
                            const float weightedViewSpaceDepthV0{pos0.w * weights[0]};
                            const float weightedViewSpaceDepthV1{pos1.w * weights[1]};
                            const float weightedViewSpaceDepthV2{pos2.w * weights[2]};
                            const float interpolatedViewSpaceDepth{1.0f / (weightedViewSpaceDepthV0 + weightedViewSpaceDepthV1 + weightedViewSpaceDepthV2)};

                            // Interpolate UV - optimized
                            const Vector2 weightedV0UV{v0.uv * pos0.w * weights[0]};
                            const Vector2 weightedV1UV{v1.uv * pos1.w * weights[1]};
                            const Vector2 weightedV2UV{v2.uv * pos2.w * weights[2]};
                            const Vector2 uv{(weightedV0UV + weightedV1UV + weightedV2UV) * interpolatedViewSpaceDepth};

                            // Interpolate Normal + Normalization
                            const Vector3 weightedV0Normal{v0.normal * weights[0]};
                            const Vector3 weightedV1Normal{v1.normal * weights[1]};
                            const Vector3 weightedV2Normal{v2.normal * weights[2]};
                            const Vector3 normal{(weightedV0Normal + weightedV1Normal + weightedV2Normal).Normalized()};

                            // Interpolate Tangent + Normalization
                            const Vector3 weightedV0Tangent{v0.tangent * weights[0]};
                            const Vector3 weightedV1Tangent{v1.tangent * weights[1]};
                            const Vector3 weightedV2Tangent{v2.tangent * weights[2]};
                            const Vector3 tangent{(weightedV0Tangent + weightedV1Tangent + weightedV2Tangent).Normalized()};

                            // Binormal
                            const Vector3 binormal{Vector3::Cross(normal, tangent)};

                            // Tangent-space transformation matrix
                            const Matrix tangentSpaceAxis{tangent, binormal, normal, Vector3::Zero};
                            
                            // Color
                            if (m_VisualizeDepthBuffer)
                            {
                                const float remappedZBuffer {Remap(interpolatedZBuffer, 0.9f, 1.0f, 0.0f, 1.0f)};
                                finalColor = ColorRGB{remappedZBuffer, remappedZBuffer, remappedZBuffer};
                            }
                            else
                            {
                                // Normal map
                                ColorRGB normalMapColor{m_pTextureNormal->Sample(uv)};
                                // Remap from [0, 1] to [-1, 1]
                                normalMapColor = 2.0f * normalMapColor - colors::White;
                                
                                // Transform to tangent space, where normal and tangent of the vertex are defined in the world space
                                const Vector3 normalMap{tangentSpaceAxis.TransformVector({normalMapColor.r, normalMapColor.g, normalMapColor.b}).Normalized()};

                                // Pixel vertex
                                Vertex_Out pixelVertex;
                                pixelVertex.normal = normalMap;
                                
                                // Final shading
                                PixelShadingV0(pixelVertex, finalColor);
                                
                            }
                            UpdateColor(finalColor, static_cast<int>(px), static_cast<int>(py));
                        }
                    }
                }
            }
        }
    }

    void Renderer::Render_W4_TODO_3()
    {
    }
#pragma endregion
}
