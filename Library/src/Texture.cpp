#include "Texture.h"
#include "Vector2.h"
#include <SDL_image.h>

#include <iostream>

namespace dae
{
    Texture::Texture(SDL_Surface* pSurface) :
        m_SurfacePtr{pSurface},
        m_SurfacePixelsPtr{(uint32_t*)pSurface->pixels}
    {
    }

    Texture::~Texture()
    {
        if (m_SurfacePtr)
        {
            SDL_FreeSurface(m_SurfacePtr);
            m_SurfacePtr = nullptr;
        }
    }

    Texture* Texture::LoadFromFile(const std::string& path)
    {
        SDL_Surface* pSurface = IMG_Load(path.c_str());
        if (!pSurface)
        {
            std::cout << "Texture::LoadFromFile() failed: " << SDL_GetError() << std::endl;
            return nullptr;
        }
        return new Texture(pSurface);
    }

    /**
     * \brief Sample the correct texel for the given uv
     * \param uv 
     * \return 
     */
    ColorRGB Texture::Sample(const Vector2& uv) const
    {
        if (uv.x < 0.0f or uv.x > 1.0f or uv.y < 0.0f or uv.y > 1.0f)
            return ColorRGB{0.39f, 0.39f, 0.39f};
        const int x{static_cast<int>(uv.x * static_cast<float>(m_pSurface->w))};
        const int y{static_cast<int>(uv.y * static_cast<float>(m_pSurface->h))};
        const int index{y * m_pSurface->w + x};
        const uint32_t pixel{m_pSurfacePixels[index]};
        uint8_t r, g, b;
        SDL_GetRGB(pixel, m_pSurface->format, &r, &g, &b);
        return ColorRGB{static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f};
    }
}
