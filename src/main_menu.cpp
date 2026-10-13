/*
 * main_menu.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <algorithm>
#include <charconv>
#include <cmath>
#include <optional>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <glm/vec2.hpp>

#include "gui/button.h"
#include "gui/frame.h"
#include "gui/label.h"
#include "gui/layout.h"
#include "gui/lineedit.h"
#include "os/popup.h"
#include "video/window.h"

#include "main_menu.h"

namespace
{
    constexpr st::float32 menu_width = 240.0f;
    constexpr st::float32 menu_height = 180.0f;
    constexpr st::float32 settings_width_fraction = 0.6f;
    constexpr st::float32 settings_height_fraction = 0.7f;

    void centre(st::gui::frame    &frame,
                st::video::window &window,
                glm::f32vec2       size) noexcept
    {
        frame.set_size(size);
        frame.set_position((window.size() - size) * 0.5f);
    }

    // One editable setting. Text and numbers use the line edit, booleans toggle on a button.
    struct setting_row
    {
        setting_row(const st::config_setting    &setting,
                    st::util::config::value      value,
                    st::gui::nestable_component *parent) noexcept :
            key(setting.key),
            current(std::move(value))
        {
            title.set_text(key);
            title.set_parent(parent);
            if (!setting.description.empty())
            {
                description.set_text(setting.description);
                description.set_parent(parent);
            }

            if (const auto *flag = std::get_if<st::bool_t>(&current))
            {
                set_flag(*flag);
                toggle.register_on_click(
                    [this]
                    {
                        if (const auto *state = std::get_if<st::bool_t>(&current))
                            set_flag(!*state);
                    });
                toggle.set_parent(parent);
                return;
            }
            if (const auto *text = std::get_if<std::string>(&current))
                edit.set_text(*text);
            else if (const auto *number = std::get_if<st::float64>(&current))
                edit.set_text(fmt::format("{}", *number));
            edit.set_parent(parent);
        }

        void set_flag(st::bool_t flag) noexcept
        {
            current = flag;
            toggle.set_text(flag ? "true" : "false");
        }

        // Returns nothing if the text of a number setting is not a finite number.
        [[nodiscard]] std::optional<st::util::config::value> read() const noexcept
        {
            if (std::holds_alternative<st::bool_t>(current))
                return current;
            if (std::holds_alternative<std::string>(current))
                return edit.text();

            const std::string &text = edit.text();
            st::float64        number = 0.0;
            const char        *end = text.data() + text.size();
            const auto         result = std::from_chars(text.data(), end, number);
            if (result.ec != std::errc{} || result.ptr != end || !std::isfinite(number))
                return std::nullopt;
            return number;
        }

        std::string             key;
        st::util::config::value current;
        st::gui::label          title;
        st::gui::label          description;
        st::gui::lineedit       edit;
        st::gui::button         toggle;
    };

    // Lists every setting and writes the changes to the config on Save.
    class settings_window
    {
    public:
        settings_window(st::video::window                     &window,
                        st::util::config                      &config,
                        const std::vector<st::config_setting> &settings,
                        const st::main_menu::save_callback    &save) noexcept :
            m_config(config),
            m_save(save)
        {
            m_frame.set_title("Settings");
            m_frame.set_collapsible(false);
            centre(m_frame, window, window.size() * glm::f32vec2{settings_width_fraction, settings_height_fraction});
            m_layout.set_parent(&m_frame);

            for (const auto &setting : settings)
            {
                const auto value = config.get(setting.key);
                if (!value)
                    continue;
                m_rows.push_back(std::make_unique<setting_row>(setting, *value, &m_layout));
            }

            m_save_button.set_text("Save");
            m_save_button.register_on_click([this] { apply(); });
            m_save_button.set_parent(&m_layout);
            m_frame.show();
        }

        [[nodiscard]] st::bool_t is_open() const noexcept { return !m_frame.is_closed(); }

    private:
        void apply() noexcept
        {
            std::vector<std::pair<std::string, st::util::config::value>> pending;
            for (const auto &row : m_rows)
            {
                auto value = row->read();
                if (!value)
                {
                    st::os::popup::warn("Settings", fmt::format("'{}' must be a number.", row->key));
                    return;
                }
                pending.emplace_back(row->key, std::move(*value));
            }

            for (const auto &[key, value] : pending)
                m_config.set(key, value);

            if (!m_save())
            {
                st::os::popup::error("Settings", "The settings could not be saved. See the log for details.");
                return;
            }
            m_frame.fade_out();
        }

        st::util::config                         &m_config;
        st::main_menu::save_callback              m_save;
        st::gui::frame                            m_frame;
        st::gui::vlayout                          m_layout;
        std::vector<std::unique_ptr<setting_row>> m_rows;
        st::gui::button                           m_save_button;
    };
} // namespace

struct st::main_menu::main_menu_impl
{
    main_menu_impl(st::video::window          &window,
                   st::util::config           &config,
                   std::vector<config_setting> settings,
                   save_callback               save) noexcept :
        m_window(window),
        m_config(config),
        m_settings(std::move(settings)),
        m_save(std::move(save))
    {
        std::sort(m_settings.begin(), m_settings.end(),
                  [](const config_setting &a, const config_setting &b) { return a.key < b.key; });

        m_frame.set_title("sticky");
        m_frame.set_closable(false);
        m_frame.set_collapsible(false);
        m_frame.set_movable(false);
        m_frame.set_resizable(false);
        centre(m_frame, m_window, {menu_width, menu_height});
        m_layout.set_parent(&m_frame);

        m_settings_button.set_text("Settings");
        m_settings_button.register_on_click([this] { open_settings(); });
        m_settings_button.set_parent(&m_layout);

        m_mods_button.set_text("Mods");
        m_mods_button.set_parent(&m_layout);

        m_quit_button.set_text("Quit");
        m_quit_button.register_on_click([this] { m_window.close(); });
        m_quit_button.set_parent(&m_layout);

        m_frame.show();
    }

    void open_settings() noexcept
    {
        if (m_settings_window && m_settings_window->is_open())
            return;
        m_settings_window = std::make_unique<settings_window>(m_window, m_config, m_settings, m_save);
    }

    st::video::window               &m_window;
    st::util::config                &m_config;
    std::vector<config_setting>      m_settings;
    save_callback                    m_save;
    st::gui::frame                   m_frame;
    st::gui::vlayout                 m_layout;
    st::gui::button                  m_settings_button;
    st::gui::button                  m_mods_button;
    st::gui::button                  m_quit_button;
    std::unique_ptr<settings_window> m_settings_window;
};

st::main_menu::main_menu(st::video::window          &window,
                         st::util::config           &config,
                         std::vector<config_setting> settings,
                         save_callback               save) noexcept :
    m_impl{
        std::make_unique<main_menu_impl>(window,
                                         config,
                                         std::move(settings),
                                         std::move(save)),
    }
{
}

st::main_menu::~main_menu() noexcept {}
