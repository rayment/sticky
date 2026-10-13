Mod Runtime {#lua_mod_runtime}
===============

Mods influence the program execution runtime environment, such as providing behaviour, events,
and customizing the game experience.

Mods are written in a modified version of Lua 5.5, and need to follow a certain structure.

## Layout

Each mod is a folder inside the `mod` folder of the game:

```
mod/
  my_mod/
    mod.json        metadata, see below
    lua/
      init.lua      entrypoint of the init phase
      runtime.lua   entrypoint of the runtime phase
      ...           modules loaded with require
    res/            textures and sounds declared by init.lua
```

A folder without `mod.json`, `lua/init.lua` and `lua/runtime.lua` is ignored.

`mod.json` is a JSON object with these string keys, all required:

| Key            | Description                                                                                         |
|----------------|-----------------------------------------------------------------------------------------------------|
| `name`         | Unique name of the mod: 1 to 64 lowercase letters, digits or underscores. Prefixes its settings.     |
| `display_name` | Name shown to the player.                                                                           |
| `version`      | Version shown to the player.                                                                        |
| `author`       | Author shown to the player.                                                                         |
| `description`  | Description shown to the player.                                                                    |

A mod with an invalid `mod.json` is skipped and the reason is logged. When two folders declare the same `name`, the
folder that sorts first is loaded and the player is told that the other was skipped.

Mods run in the order of their names: every phase and every callback reaches the mods alphabetically.

## Phases

The Lua API splits mods into two distinct parts: the init phase, and the runtime phase. Each mod executes in its own
Lua runtime environment.

Within each phase, the runtime environment will change, such that certain parts of the API are only accessible in the
init phase, and others in the runtime phase respectively.

### Init Phase

The init phase executes during game start-up, and allows mods to define their global configuration, such as a list of
setting items, or a list of textures or sounds that should be loaded.

The init phase only runs once per mod, and the entrypoint is the `init.lua` file which must be located in the `lua`
folder underneath the mod root directory.

### Runtime Phase

The runtime phase executes during normal gameplay and allows interaction with the game world, the screen, and other
input/output devices in real-time. The gameplay event loop is fired during this phase and may automatically execute
specifically-named Lua functions if they are defined.

The runtime phase is spawned once per mod, but callbacks from the engine will occur during runtime, such as when the
event system notifies Lua of a specific event.

The entrypoint for the runtime phase is the `runtime.lua` file which must be located in the `lua` folder underneath the
mod root directory. All callbacks and other code that should be executed at runtime must be defined within this file or
imported with the `require` function.

A mod whose `init.lua` or `runtime.lua` raises an error is unloaded and the error is logged. The other mods keep
running.

### Engine callbacks

The engine calls these global functions of every mod that defines them. Errors raised inside them are logged and do not
stop later calls.

| Function            | When                                                                                         |
|---------------------|----------------------------------------------------------------------------------------------|
| `on_start()`        | Once, after every mod has finished its runtime phase.                                        |
| `on_fixed_step(dt)` | At a fixed rate, every `dt` seconds (0.02 by default). Several calls may happen in one frame. |
| `on_step(dt)`       | Once per frame. `dt` is the duration of the last frame in seconds, at most one fixed step.    |
| `on_quit()`         | Once, when the application closes, including after @ref lua_api_app_quit.                   |

```lua
function on_step(dt)
    timer = timer + dt
end
```

### Graphical user interface

The engine renders GUI components itself. A mod builds its windows from the `st.gui` classes and shows them, and the
engine draws them every frame until they are closed, so no render callback exists. Windows are closed by the script or
by the user, and the engine releases them when that happens. See @ref lua_gui_lifecycle.
