/*
 * texture.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <utility>

#include <fmt/format.h>

#include "io/log.h"
#include "video/texture.h"

static SDL_Surface *
load_rgba_surface(const std::filesystem::path &path) noexcept
{
    const std::string utf8 = path.u8string();
    SDL_Surface      *surface = SDL_LoadPNG(utf8.c_str());
    if (surface == nullptr)
    {
        st::io::log::error(fmt::format("Failed to load PNG '{}': {}", utf8, SDL_GetError()));
        return nullptr;
    }

    // Normalizes 8/16-bit and RGB/RGBA sources to byte-ordered RGBA.
    SDL_Surface *converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);
    if (converted == nullptr)
        st::io::log::error(fmt::format("Failed to convert PNG '{}': {}", utf8, SDL_GetError()));
    return converted;
}

static st::uint32
upload_surface(const SDL_Surface &surface) noexcept
{
    GLint previous_binding = 0;
    GLint previous_row_length = 0;
    GLint previous_alignment = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_binding);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &previous_row_length);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_alignment);

    GLuint handle = 0;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, surface.pitch / 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface.w, surface.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface.pixels);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, previous_row_length);
    glPixelStorei(GL_UNPACK_ALIGNMENT, previous_alignment);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_binding));

    if (glGetError() != GL_NO_ERROR)
    {
        glDeleteTextures(1, &handle);
        return 0;
    }
    return handle;
}

struct st::video::texture::texture_impl
{
    texture_impl() noexcept = default;

    ~texture_impl() noexcept { glDeleteTextures(1, &handle); }

    ST_DELETE_COPY_CTORS(texture_impl);
    ST_DELETE_MOVE_CTORS(texture_impl);

    st::uint32 handle = 0;
    st::uint32 width = 0;
    st::uint32 height = 0;
};

st::video::texture::texture(std::unique_ptr<texture_impl> impl) noexcept : m_impl(std::move(impl)) {}

st::video::texture::~texture() noexcept = default;

st::uint32
st::video::texture::width() const noexcept
{
    return m_impl->width;
}

st::uint32
st::video::texture::height() const noexcept
{
    return m_impl->height;
}

st::uint32
st::video::texture::gl_handle() const noexcept
{
    return m_impl->handle;
}

st::uint64
st::video::texture::imgui_id() const noexcept
{
    return m_impl->handle;
}

std::shared_ptr<st::video::texture>
st::video::texture::load_file(const std::filesystem::path &path) noexcept
{
    SDL_Surface *surface = load_rgba_surface(path);
    if (surface == nullptr)
        return nullptr;

    const st::uint32 handle = upload_surface(*surface);
    const st::uint32 width = static_cast<st::uint32>(surface->w);
    const st::uint32 height = static_cast<st::uint32>(surface->h);
    SDL_DestroySurface(surface);

    if (handle == 0)
    {
        st::io::log::error(fmt::format("Failed to upload texture '{}' to OpenGL.", path.u8string()));
        return nullptr;
    }
    auto impl = std::make_unique<texture_impl>();
    impl->handle = handle;
    impl->width = width;
    impl->height = height;
    return std::shared_ptr<texture>(new texture(std::move(impl)));
}
