API Reference {#lua_api_index}
=============

## Shared API

The following pages document shared API features that are available in both the init and runtime mod phases.

- @subpage lua_api_io_log

## Init API

The following API features are only available during the init phase, and are turned off at runtime.

- @subpage lua_api_init

## Runtime API

The following API features are only available during the runtime mod phase, and are not yet active during init.

- @subpage lua_api_audio_sound
- @subpage lua_api_audio_source
- @subpage lua_api_video_texture
- @subpage lua_api_io_mouse

### GUI

The GUI classes are drawn by the engine once they are shown. Start with @ref lua_gui_lifecycle to learn how long they
live and how to release them.

- @subpage lua_gui_lifecycle
- @subpage lua_api_gui_component
- @subpage lua_api_gui_frame
- @subpage lua_api_gui_button
- @subpage lua_api_gui_label
- @subpage lua_api_gui_progress
- @subpage lua_api_gui_rect
- @subpage lua_api_gui_hlayout
- @subpage lua_api_gui_vlayout
- @subpage lua_api_gui_popup
