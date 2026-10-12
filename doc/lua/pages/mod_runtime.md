Mod Runtime {#lua_mod_runtime}
===============

Mods influence the program execution runtime environment, such as providing behaviour, events,
and customizing the game experience.

Mods are written in a modified version of Lua 5.5, and need to follow a certain structure.

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
