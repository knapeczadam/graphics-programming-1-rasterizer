#include "Camera.h"

#include <iostream>

#include "ImGui/imgui.h"

namespace dae
{
    Camera::Camera(const Vector3& _origin, float _fovAngle)
        : origin{_origin}
          , fovAngle{_fovAngle}
    {
    }

    void Camera::Initialize(float _fovAngle, Vector3 _origin, float _nearPlane, float _farPlane)
    {
        fovAngle = _fovAngle;
        fov = CalculateFOV(fovAngle);

        origin = _origin;
        nearPlane = _nearPlane;
        farPlane = _farPlane;
    }

    void Camera::Update(Timer* pTimer)
    {
        const float deltaTime = pTimer->GetElapsed();

        //Camera Update Logic

        //Keyboard Input
        const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
        MoveCamera(pKeyboardState, deltaTime);
        RotateCamera(deltaTime);

        //Update Matrices
        CalculateViewMatrix();
        CalculateProjectionMatrix();
        //Try to optimize this - should only be called once or when fov/aspectRatio changes
    }

    float Camera::GetFOV() const
    {
        return CalculateFOV(fovAngle);
    }

    void Camera::Scroll(SDL_MouseWheelEvent wheel)
    {
        if (wheel.y > 0) // scroll up
        {
            origin += forward * m_ScrollSpeed;
        }
        else if (wheel.y < 0) // scroll down
        {
            origin -= forward * m_ScrollSpeed;
        }
    }

    void Camera::IncreaseFOV()
    {
        ++fovAngle;
    }

    void Camera::DecreaseFOV()
    {
        --fovAngle;
    }

    void Camera::SetTotalPitch(float pitch)
    {
        totalPitch = pitch;
    }

    void Camera::SetTotalYaw(float yaw)
    {
        totalYaw = yaw;
    }

    float Camera::CalculateFOV(float angle) const
    {
        const float halfAlpha{(angle * 0.5f) * TO_RADIANS};
        return std::tanf(halfAlpha);
    }

    void Camera::MoveCamera(const uint8_t* pKeyboardState, float deltaTime)
    {
        if (pKeyboardState[SDL_SCANCODE_A])
        {
            origin -= right * deltaTime * speed;
        }
        else if (pKeyboardState[SDL_SCANCODE_D])
        {
            origin += right * deltaTime * speed;
        }
        if (pKeyboardState[SDL_SCANCODE_W])
        {
            origin += forward * deltaTime * speed;
        }
        else if (pKeyboardState[SDL_SCANCODE_S])
        {
            origin -= forward * deltaTime * speed;
        }
    }

    void Camera::RotateCamera(float deltaTime)
    {
        // Check whether the mouse is over the ImGui window
        if (ImGui::GetIO().WantCaptureMouse)
        {
            return;
        }
        
        int mouseX{}, mouseY{};
        const int threshold{1};
        const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
        mouseX = mouseX > threshold ? 1 : mouseX < -threshold ? -1 : 0;
        mouseY = mouseY > threshold ? 1 : mouseY < -threshold ? -1 : 0;
        const bool leftMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
        const bool rightMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
        if (leftMouseButtonDown and rightMouseButtonDown)
        {
            origin += up * static_cast<float>(mouseY * -1) * deltaTime * speed;
        }
        else if (leftMouseButtonDown)
        {
            origin += forward * static_cast<float>(mouseY * -1) * deltaTime * speed;
            totalYaw += static_cast<float>(mouseX) * rotationSpeed * deltaTime;
        }
        else if (rightMouseButtonDown)
        {
            totalYaw += static_cast<float>(mouseX) * rotationSpeed * deltaTime;
            totalPitch += static_cast<float>(mouseY * -1) * rotationSpeed * deltaTime;
        }
        if (mouseX or mouseY)
        {
            forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.0f).TransformVector(
                Vector3::UnitZ);
            // TODO: verify if this is needed
            //forward.Normalize();
        }
    }

    void Camera::CalculateViewMatrix()
    {
        //ONB => invViewMatrix
        //Inverse(ONB) => ViewMatrix
        
        viewMatrix = Matrix::CreateLookAtLH(origin, forward, up, right);
        invViewMatrix = Matrix::Inverse(viewMatrix);

        //ViewMatrix => Matrix::CreateLookAtLH(...) [not implemented yet]
        //DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
    }

    void Camera::CalculateProjectionMatrix()
    {
        //ProjectionMatrix => Matrix::CreatePerspectiveFovLH(...) [not implemented yet]
        //DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
        projectionMatrix = Matrix::CreatePerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
    }

}
