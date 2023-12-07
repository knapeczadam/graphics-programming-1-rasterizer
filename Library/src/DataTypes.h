#pragma once
#include "Maths.h"
#include "vector"

namespace dae
{
#pragma region GEOMETRY
    struct Vertex
    {
        Vector3  position      {};
        ColorRGB color         {colors::White};
        Vector2  uv            {};              //W2
        Vector3  normal        {};              //W4
        Vector3  tangent       {};              //W4
        Vector3  viewDirection {};              //W4
    };

    struct Vertex_Out
    {
        Vector4  position        {};
        ColorRGB color           {colors::White};
        Vector2  uv              {};
        Vector3  normal          {};
        Vector3  tangent         {};
        Vector3  viewDirection   {};
        bool     isFrustumCulled {false};
    };

    enum class PrimitiveTopology
    {
        TriangleList,
        TriangleStrip
    };

    struct Mesh
    {
        std::vector<Vertex>   vertices {};
        std::vector<uint32_t> indices  {};
        PrimitiveTopology primitiveTopology{PrimitiveTopology::TriangleStrip};

        std::vector<Vertex_Out> vertices_out{};
        Matrix worldMatrix{};
    };
#pragma endregion
    
#pragma region LIGHT
    enum class LightType
    {
        Point,
        Directional
    };

    struct Light
    {
        Vector3   origin    {};
        Vector3   direction {};
        ColorRGB  color     {colors::White};
        LightType type      {LightType::Directional};
        float     intensity {1.0f};
    };
#pragma endregion
}
