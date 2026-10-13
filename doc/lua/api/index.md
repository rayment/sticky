API Reference {#lua_api_index}
=============

Everything the engine exposes to Lua lives in the global `st` table. Each module below is available in the phases
listed next to it; outside of those phases its functions are missing or raise an error.

| Module                     | Init phase | Runtime phase | Contents                                       |
|----------------------------|:----------:|:-------------:|------------------------------------------------|
| @subpage lua_api_app       | yes        | yes           | Quitting the application                        |
| @subpage lua_api_init      | yes        | no            | Declaring settings, textures and sounds         |
| @subpage lua_api_audio     | no         | yes           | Sounds and audio sources                        |
| @subpage lua_api_gui       | no         | yes           | Windows, widgets and layouts                    |
| @subpage lua_api_io        | yes        | yes           | Logging, and mouse buttons at runtime           |
| @subpage lua_api_util      | no         | yes           | Reading and writing settings                    |
| @subpage lua_api_video     | no         | yes           | Textures                                        |

See @ref lua_mod_runtime for what the phases are.
