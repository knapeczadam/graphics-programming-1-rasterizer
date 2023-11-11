#pragma once
#include <cassert>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Maths.h"
#include "Timer.h"

namespace dae
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const Vector3& _origin, float _fovAngle);
        
        void Initialize(float _fovAngle = 90.f, Vector3 _origin = {0.f, 0.f, 0.f});
        void Update(Timer* pTimer);

        
        Matrix CalculateCameraToWorld();
        float GetFOV() const;
        void Scroll(SDL_MouseWheelEvent wheel);
        void IncreaseFOV();
        void DecreaseFOV();
        void SetTotalPitch(float pitch);
        void SetTotalYaw(float yaw);
        
        void CalculateViewMatrix();
        void CalculateProjectionMatrix();
        Matrix GetViewProjection() const;
        
    private:
        float CalculateFOV(float angle) const;
        void MoveCamera(const uint8_t* pKeyboardState, float deltaTime);
        void RotateCamera(float deltaTime);

    private:
        Vector3 origin   {};
        float   fovAngle {90.f};
        float   fov      {tanf((fovAngle * TO_RADIANS) / 2.f)};

        Vector3 forward {Vector3::UnitZ};
        Vector3 up      {Vector3::UnitY};
        Vector3 right   {Vector3::UnitX};

        float  totalPitch    {0.0f};
        float  totalYaw      {0.0f};
        Matrix cameraToWorld {};
        float  speed         {10.0f};
        float  rotationSpeed {100.0f};
        float  m_ScrollSpeed {0.5f};

        Matrix invViewMatrix {};
        Matrix viewMatrix    {};
    };
}
