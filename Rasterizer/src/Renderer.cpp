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
    // -------------------------
    // --- Global Variables ----
    // -------------------------
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

    const std::vector<Mesh> meshes_world_list =
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
    std::vector<Vertex> vertices_ss(9);

    std::array<float, 3> weights{};
    // -------------------------

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
        m_Camera.Initialize(60.f, {.0f, .0f, -10.f});
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        m_Camera.SetAspectRatio(aspectRatio);

        // Texture
        m_pTexture = Texture::LoadFromFile("Resources/uv_grid_2.png");

        // --- ASSERTS ---
        assert(not meshes_world_list.empty() and "Meshes list is empty");
        assert(not meshes_world_strip.empty() and "Meshes strip is empty");
    }

    Renderer::~Renderer()
    {
        //delete[] m_pDepthBufferPixels;
        delete m_pTexture;
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

        // --- WEEK 4 ---
#elif W4
#endif

        //@END
        //Update SDL Surface
        SDL_UnlockSurface(m_pBackBuffer);
        SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::VertexTransformationFromWorldToScreen(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const
    {
        for (size_t i{0}; i < vertices_in.size(); ++i)
        {
            const Vertex& vertex_in = vertices_in[i];
            Vertex& vertex_out = vertices_out[i];

            // WORLD
            const Vector4 v4{vertex_in.position.x, vertex_in.position.y, vertex_in.position.z, 1.0f};
            // VIEW
            const Vector4 v4_ndc = m_Camera.invViewMatrix.TransformPoint(v4);
            // PROJECTION
            vertex_out.position.x = v4_ndc.x / v4_ndc.z;
            vertex_out.position.y = v4_ndc.y / v4_ndc.z;
            // NDC
            vertex_out.position.x = vertex_out.position.x / (m_Camera.GetFOV() * m_Camera.GetAspectRatio());
            vertex_out.position.y = vertex_out.position.y / m_Camera.GetFOV();
            // SCREEN
            vertex_out.position.x = (vertex_out.position.x + 1.0f) * 0.5f * static_cast<float>(m_Width);
            vertex_out.position.y = (1.0f - vertex_out.position.y) * 0.5f * static_cast<float>(m_Height);
            // DEPTH
            assert(v4_ndc.z != 0.0f and "Renderer::VertexTransformationFromWorldToScreen: Division by zero");
            vertex_out.position.z = v4_ndc.z;
            // UV
            vertex_out.uv = vertex_in.uv;
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
        VertexTransformationFromWorldToScreen(triangle_vertices_world_todo_2, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};
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
        VertexTransformationFromWorldToScreen(triangle_vertices_world_todo_3, vertices_ss);

        const Vector2 v0{vertices_ss[0].position.GetXY()};
        const Vector2 v1{vertices_ss[1].position.GetXY()};
        const Vector2 v2{vertices_ss[2].position.GetXY()};

        for (int px{0}; px < m_Width; ++px)
        {
            for (int py{0}; py < m_Height; ++py)
            {
                const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                ColorRGB finalColor{colors::Black};
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

        VertexTransformationFromWorldToScreen(triangle_vertices_world_todo_4, vertices_ss);

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
                    if (IsPointInTriangle(pixel, v0, v1, v2, weights))
                    {
                        // Depth
                        const float depth = vertices_ss[triangleIdx].position.z * weights[0] +
                                        vertices_ss[triangleIdx + 1].position.z * weights[1] +
                                        vertices_ss[triangleIdx + 2].position.z * weights[2];
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

        VertexTransformationFromWorldToScreen(triangle_vertices_world_todo_4, vertices_ss);

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
                    if (IsPointInTriangle(pixel, v0, v1, v2, weights))
                    {
                        // Depth
                        const float depth = vertices_ss[triangleIdx].position.z * weights[0] +
                                        vertices_ss[triangleIdx + 1].position.z * weights[1] +
                                        vertices_ss[triangleIdx + 2].position.z * weights[2];
                        // TODO: simplify
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

    void Renderer::Render_W2_TODO_1()
    {
        std::fill_n(m_DepthBuffer.begin(), m_DepthBuffer.size(), std::numeric_limits<float>::max());

        SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));
        
        VertexTransformationFromWorldToScreen(meshes_world_list[0].vertices, vertices_ss);

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
                    
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};
                        
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

        VertexTransformationFromWorldToScreen(meshes_world_strip[0].vertices, vertices_ss);

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
                    
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};
                        
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

        VertexTransformationFromWorldToScreen(meshes_world_strip[0].vertices, vertices_ss);

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
                    
                    if (IsPointInTriangle(pixel, pos0.GetXY(), pos1.GetXY(), pos2.GetXY(), weights))
                    {
                        // Interpolate depth
                        const float depth {pos0.z * weights[0] + pos1.z * weights[1] + pos2.z * weights[2]};

                        // Interpolate UV
                        const Vector2 uv {v0.uv * weights[0] + v1.uv * weights[1] + v2.uv * weights[2]};
                        
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

        VertexTransformationFromWorldToScreen(meshes_world_strip[0].vertices, vertices_ss);

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

        VertexTransformationFromWorldToScreen(meshes_world_strip[0].vertices, vertices_ss);

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

            // Clamp bounding box to screen + stretch by 1 pixel
            minX = std::max(--minX, 0);
            maxX = std::min(++maxX, m_Width - 1);
            minY = std::max(--minY, 0);
            maxY = std::min(++maxY, m_Height - 1);

            for (int px{minX}; px <= maxX; ++px)
            {
                for (int py{minY}; py <= maxY; ++py)
                {
                    const Vector2 pixel{static_cast<float>(px) + 0.5f, static_cast<float>(py) + 0.5f};

                    ColorRGB finalColor{colors::Black};
                    
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

    bool Renderer::SaveBufferToImage() const
    {
        return SDL_SaveBMP(m_pBackBuffer, "Rasterizer_ColorBuffer.bmp");
    }
}
