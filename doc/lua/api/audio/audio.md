st.audio {#lua_api_audio}
========

Playing sounds that a mod declared in `st.init.sounds`. Available during the runtime phase.

Sounds are loaded by the engine during the init phase and looked up by name with `st.audio.sound.get`. A source plays
one sound at a time from a position in the world.

- @subpage lua_api_audio_sound
- @subpage lua_api_audio_source
