#pragma once
#include <cfloat>
#include <cmath>
#include <array>

#include "Vector2.h"

namespace dae
{
    /* --- HELPER STRUCTS --- */
    struct Int2
    {
        int x{};
        int y{};
    };

    /* --- CONSTANTS --- */
    constexpr auto PI = 3.14159265358979323846f;
    constexpr auto PI_DIV_2 = 1.57079632679489661923f;
    constexpr auto PI_DIV_4 = 0.785398163397448309616f;
    constexpr auto PI_2 = 6.283185307179586476925f;
    constexpr auto PI_4 = 12.56637061435917295385f;

    constexpr auto TO_DEGREES = (180.0f / PI);
    constexpr auto TO_RADIANS(PI / 180.0f);

    /* --- HELPER FUNCTIONS --- */
    inline float Square(float a)
    {
        return a * a;
    }

    inline float Lerpf(float a, float b, float factor)
    {
        return ((1 - factor) * a) + (factor * b);
    }

    inline bool AreEqual(float a, float b, float epsilon = FLT_EPSILON)
    {
        return abs(a - b) < epsilon;
    }

    inline int Clamp(const int v, int min, int max)
    {
        if (v < min) return min;
        if (v > max) return max;
        return v;
    }

    inline float Clamp(const float v, float min, float max)
    {
        if (v < min) return min;
        if (v > max) return max;
        return v;
    }

    inline float Saturate(const float v)
    {
        if (v < 0.f) return 0.f;
        if (v > 1.f) return 1.f;
        return v;
    }

    inline float Remap(float value, float inputMin, float inputMax, float outputMin, float outputMax)
    {
        if (value <= inputMin) return outputMin;
        if (value >= inputMax) return outputMax;
        return outputMin + (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin);
    }

    /**
     * \brief https://www.youtube.com/watch?v=HYAgJN3x4GA
     * ~2X faster than IsPointInTriangleV2
     * \param point
     * \param A 
     * \param B 
     * \param C 
     * \return 
     */
    inline bool IsPointInTriangleFast(const Vector2& point, const Vector2& A, const Vector2& B, const Vector2& C)
    {
        if (point == A or point == B or point == C) return true;
        
        const float Cy_min_Ay = C.y - A.y;
        const float Cx_min_Ax = C.x - A.x;
        const float By_min_Ay = B.y - A.y;
        
        const float w1{ (A.x * Cy_min_Ay + (point.y - A.y) * Cx_min_Ax - point.x * Cy_min_Ay) / (By_min_Ay * Cx_min_Ax - (B.x - A.x) * Cy_min_Ay) };
        const float w2{ (point.y - A.y - w1 * By_min_Ay) / Cy_min_Ay };
        
        return w1 >= 0 and w2 >= 0 and (w1 + w2) <= 1;
    }

    inline bool IsPointInTriangle(const Vector2& point, const Vector2& v0, const Vector2& v1, const Vector2& v2)
    {
        if (point == v0 or point == v1 or point == v2) return true;
        
        const Vector2 v0v1 = v1 - v0;
        const Vector2 v1v2 = v2 - v1;
        const Vector2 v2v0 = v0 - v2;

        if (Vector2::Cross(v0v1, point - v0) < 0) return false;
        if (Vector2::Cross(v1v2, point - v1) < 0) return false;
        if (Vector2::Cross(v2v0, point - v2) < 0) return false;

        return true;
    }
    
    inline bool IsPointInTriangle(const Vector2& point, const Vector2& v0, const Vector2& v1, const Vector2& v2, std::array<float,3>& weights)
    {
        if (point == v0 or point == v1 or point == v2) return true;
        
        const Vector2 v0v1 = v1 - v0;
        const Vector2 v1v2 = v2 - v1;
        const Vector2 v2v0 = v0 - v2;
        
        weights[0] = Vector2::Cross(v1v2, point - v1);
        weights[1] = Vector2::Cross(v2v0, point - v2);
        weights[2] = Vector2::Cross(v0v1, point - v0);
        
        if (weights[0] < 0) return false;
        if (weights[1] < 0) return false;
        if (weights[2] < 0) return false;

        const float area = 1.0f / Vector2::Cross(v0v1, v1v2);
        weights[0] *= area;
        weights[1] *= area;
        weights[2] *= area;

        return true;
    }
}
