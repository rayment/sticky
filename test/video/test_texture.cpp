/*
 * test_texture.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <array>
#include <filesystem>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <doctest/doctest.h>

#include "headless_session.h"
#include "video/texture.h"
#include "video/window.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR) / "video";

    const fs::path good_empty = res_dir / "good_empty.txt";
    const fs::path good_text = res_dir / "good_text.txt";
    const fs::path good_tree = res_dir / "good_tree";
    const fs::path good_wav = res_dir / "good_wav.wav";
    const fs::path good_rgb8 = res_dir / "good_rgb8.png";
    const fs::path good_rgb16 = res_dir / "good_rgb16.png";
    const fs::path good_rgba8 = res_dir / "good_rgba8.png";
    const fs::path good_rgba16 = res_dir / "good_rgba16.png";

    // Never created on disk.
    const fs::path bad_missing = res_dir / "bad_missing.png";

    // Exceptions are disabled, so doctest's REQUIRE is unavailable.
#define REQUIRE_LOADED(ptr)                                                                                            \
    if ((ptr) == nullptr)                                                                                              \
    {                                                                                                                  \
        FAIL_CHECK("texture failed to load");                                                                          \
        return;                                                                                                        \
    }

    using rgba = std::array<st::uint8, 4>;

    constexpr st::uint32 image_size = 16;
    constexpr rgba       red = {255, 0, 0, 255};
    constexpr rgba       blue = {0, 122, 255, 255};
    constexpr rgba       clear = {0, 0, 0, 0};

    // Textures need a current OpenGL context, which the window provides.
    // The window is a fixture member so it outlives textures held by tests.
    struct texture_fixture
    {
        st::test::headless_session &session = st::test::headless_session::instance();
        st::video::window           window;

        texture_fixture() { static_cast<void>(glGetError()); }
    };

    std::vector<st::uint8> read_pixels(const st::video::texture &texture)
    {
        const st::size         size = static_cast<st::size>(texture.width()) * texture.height() * 4;
        std::vector<st::uint8> pixels(size);
        glBindTexture(GL_TEXTURE_2D, texture.gl_handle());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glBindTexture(GL_TEXTURE_2D, 0);
        return pixels;
    }

    rgba pixel(const std::vector<st::uint8> &pixels,
               st::uint32                    x,
               st::uint32                    y)
    {
        const st::size i = ((static_cast<st::size>(y) * image_size) + x) * 4;
        return {pixels[i], pixels[i + 1], pixels[i + 2], pixels[i + 3]};
    }

    void check_loads(const fs::path &path)
    {
        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(path);

        REQUIRE_LOADED(texture);
        CHECK_EQ(image_size, texture->width());
        CHECK_EQ(image_size, texture->height());
        CHECK_NE(0, texture->gl_handle());
        CHECK_EQ(GL_NO_ERROR, glGetError());
    }

    void check_opaque(const fs::path &path)
    {
        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(path);

        REQUIRE_LOADED(texture);
        const std::vector<st::uint8> pixels = read_pixels(*texture);
        CHECK_EQ(red, pixel(pixels, 0, 0));
        CHECK_EQ(red, pixel(pixels, 8, 8));
        CHECK_EQ(red, pixel(pixels, 15, 15));
    }

    void check_alpha(const fs::path &path)
    {
        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(path);

        REQUIRE_LOADED(texture);
        const std::vector<st::uint8> pixels = read_pixels(*texture);
        CHECK_EQ(clear, pixel(pixels, 0, 0));
        CHECK_EQ(clear, pixel(pixels, 15, 15));
        CHECK_EQ(blue, pixel(pixels, 2, 0));
        CHECK_EQ(blue, pixel(pixels, 0, 2));
        CHECK_EQ(red, pixel(pixels, 8, 8));
    }
} // namespace

TEST_SUITE("video::texture")
{
    TEST_CASE_FIXTURE(texture_fixture, "load_file loads PNG files of every supported format")
    {
        check_loads(good_rgb8);
        check_loads(good_rgb16);
        check_loads(good_rgba8);
        check_loads(good_rgba16);
    }

    TEST_CASE_FIXTURE(texture_fixture, "textures are valid OpenGL texture objects")
    {
        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(good_rgba8);

        REQUIRE_LOADED(texture);
        CHECK(glIsTexture(texture->gl_handle()));
    }

    TEST_CASE_FIXTURE(texture_fixture, "imgui_id matches the OpenGL handle")
    {
        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(good_rgb8);

        REQUIRE_LOADED(texture);
        CHECK_EQ(static_cast<st::uint64>(texture->gl_handle()), texture->imgui_id());
    }

    TEST_CASE_FIXTURE(texture_fixture, "RGB images are opaque")
    {
        check_opaque(good_rgb8);
        check_opaque(good_rgb16);
    }

    TEST_CASE_FIXTURE(texture_fixture, "RGBA images preserve alpha")
    {
        check_alpha(good_rgba8);
        check_alpha(good_rgba16);
    }

    TEST_CASE_FIXTURE(texture_fixture, "load_file does not disturb OpenGL texture binding")
    {
        GLuint sentinel = 0;
        glGenTextures(1, &sentinel);
        glBindTexture(GL_TEXTURE_2D, sentinel);

        const std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(good_rgba8);

        GLint bound = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
        CHECK_EQ(sentinel, static_cast<GLuint>(bound));

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &sentinel);
    }

    TEST_CASE_FIXTURE(texture_fixture, "load_file returns distinct textures for the same file")
    {
        const std::shared_ptr<st::video::texture> first = st::video::texture::load_file(good_rgba8);
        const std::shared_ptr<st::video::texture> second = st::video::texture::load_file(good_rgba8);

        REQUIRE_LOADED(first);
        REQUIRE_LOADED(second);
        CHECK_NE(first, second);
        CHECK_NE(first->gl_handle(), second->gl_handle());
    }

    TEST_CASE_FIXTURE(texture_fixture, "load_file returns null for missing paths")
    {
        CHECK_EQ(nullptr, st::video::texture::load_file(bad_missing));
        CHECK_EQ(nullptr, st::video::texture::load_file(fs::path{}));
        CHECK_EQ(nullptr, st::video::texture::load_file(good_rgb8 / "bad_below_file.png"));
    }

    TEST_CASE_FIXTURE(texture_fixture, "load_file returns null for directories")
    {
        CHECK_EQ(nullptr, st::video::texture::load_file(res_dir));
        CHECK_EQ(nullptr, st::video::texture::load_file(good_tree));
    }

    TEST_CASE_FIXTURE(texture_fixture, "load_file returns null for non-PNG files")
    {
        CHECK_EQ(nullptr, st::video::texture::load_file(good_empty));
        CHECK_EQ(nullptr, st::video::texture::load_file(good_text));
        CHECK_EQ(nullptr, st::video::texture::load_file(good_wav));
    }

    TEST_CASE_FIXTURE(texture_fixture, "failed loads leave OpenGL without errors")
    {
        CHECK_EQ(nullptr, st::video::texture::load_file(bad_missing));
        CHECK_EQ(nullptr, st::video::texture::load_file(good_text));
        CHECK_EQ(GL_NO_ERROR, glGetError());
    }

    TEST_CASE_FIXTURE(texture_fixture, "a load can succeed after a failed one")
    {
        CHECK_EQ(nullptr, st::video::texture::load_file(bad_missing));
        CHECK_NE(nullptr, st::video::texture::load_file(good_rgba8));
    }

    TEST_CASE_FIXTURE(texture_fixture, "destroying a texture releases its OpenGL object")
    {
        std::shared_ptr<st::video::texture> texture = st::video::texture::load_file(good_rgba8);

        REQUIRE_LOADED(texture);
        const st::uint32 handle = texture->gl_handle();
        texture.reset();
        CHECK_FALSE(glIsTexture(handle));
        CHECK_EQ(GL_NO_ERROR, glGetError());
    }
}
