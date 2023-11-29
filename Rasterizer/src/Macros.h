#pragma once

// 0/1 vs true/false:
// https://stackoverflow.com/questions/135069/ifdef-vs-if-which-is-better-safer-as-a-method-for-enabling-disabling-compila#:~:text=In%20idiomatic%20use%2C%20%23ifdef%20is,defined(B).&text=This%20means%2C%20that%20%2DDA%20is,case%20of%20%23if%20A%20usage.
namespace dae
{
    #define W1 0
    #define W2 0
    #define W3 0
    #define W4 1

    #define TODO_0 0
    #define TODO_1 0
    #define TODO_2 1
    #define TODO_3 0
    #define TODO_4 0
    #define TODO_5 0    
    #define TODO_6 0    
}

// --- WEEK 1 ---
/*
 * TODO_0: GrayScale
 * TODO_1: Big-white triangle (from NDC to screen space)
 * TODO_2: Small-white triangle (from world space to screen space)
 * TODO_3: Interpolated color
 * TODO_4: 2 triangles with Z-test
 * TODO_5: Bounding box
 */

// --- WEEK 2 ---
/*
 * TODO_1: White square - triangle list
 * TODO_2: White square - triangle strip
 * TODO_3: Textured square
 * TODO_4: Textured square with interpolated UVs
 */

// --- WEEK 3 ---
/*
 * TODO_0: Load tuktuk.obj as a sample model to test Utils::ParseOBJ function
 * TODO_1: Textured square - Perspective projection with Frustum culling
 * TODO_2: Tuktuk
 * TODO_3: Tuktuk - Visualize Depth Buffer
 * TODO_4: Rotate Tuktuk - Modified bounding box
 * TODO_5: Tuktuk - Visualize Bounding Box
 * TODO_6: Tuktuk - Optimized Transformation function + interpolation
 */

// --- WEEK 4 ---
/*
 * TODO_0: Test vehicle.obj
 * TODO_1: Vehicle - observed area calculation based on .obj file's normals
 * TODO_2: Vehicle - observed area calculation based on normal map
 */
