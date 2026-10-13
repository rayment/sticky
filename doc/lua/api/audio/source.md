st.audio.source {#lua_api_audio_source}
===========

Class that manages a physical audio source capable of playing sounds.

Overview
--------

This class provides an audio source which can be positioned and manipulated during program runtime.
Audio sources are responsible for playing sounds. Multiple audio sources can play sounds concurrently, but each source
can only play one sound at a time.

Methods
-------

### st.audio.source() {#lua_api_audio_source_ctor}

This method constructs a new audio source instance. By default, no sound is attached and the source will be stopped.

**Returns:** `st.audio.source` - a new instance of the `st.audio.source` class

```lua
local src = st.audio.source()
```

### st.audio.source:set_sound(sound) {#lua_api_audio_source_set_sound}

This method sets the sound that the source plays, stopping the sound it was playing.

Setting a sound does not start playback; call @ref lua_api_audio_source_play to begin playing the sound.

| Parameter | Type             | Description                    |
|-----------|------------------|--------------------------------|
| `sound`   | `st.audio.sound` | The sound to play.             |

**Returns:** `boolean` - `true` if the sound was set, or `false` if no audio device is available

```lua
local src = st.audio.source()
src:set_sound(st.audio.sound.get("foobar"))
```

### st.audio.source:play() {#lua_api_audio_source_play}

This method starts playing the sound from the beginning.

If the source is already playing or paused, playback is restarted from the beginning.

**Returns:** `boolean` - `true` if playback was started successfully, or `false` otherwise

### st.audio.source:pause() {#lua_api_audio_source_pause}

This method pauses playback, keeping the current playback position.

Use @ref lua_api_audio_source_resume to continue playing from the position at which the source was paused.

**Returns:** `boolean` - `true` if the source was paused successfully, or `false` otherwise

### st.audio.source:resume() {#lua_api_audio_source_resume}

This method resumes playback from the current playback position.

Unlike @ref lua_api_audio_source_play, the sound is not restarted from the beginning.

**Returns:** `boolean` - `true` if playback was resumed successfully, or `false` otherwise

### st.audio.source:stop() {#lua_api_audio_source_stop}

This method stops playback and resets the playback position to the beginning of the sound.

**Returns:** `boolean` - `true` if the source was stopped successfully, or `false` otherwise

### st.audio.source:is_playing() {#lua_api_audio_source_is_playing}

This method queries whether the source is currently playing a sound.

**Returns:** `boolean` - `true` if the source is playing, or `false` otherwise

### st.audio.source:is_paused() {#lua_api_audio_source_is_paused}

This method queries whether the source is currently paused.

**Returns:** `boolean` - `true` if the source is paused, or `false` otherwise

### st.audio.source:is_stopped() {#lua_api_audio_source_is_stopped}

This method queries whether the source is currently stopped.

A source is stopped when no sound has been played yet, when playback was stopped explicitly, or when a non-looping sound
has finished playing.

**Returns:** `boolean` - `true` if the source is stopped, or `false` otherwise

### st.audio.source:set_position(x, y, z) {#lua_api_audio_source_set_position}

This method sets the position of the source in world space.

The position is used to spatialise the sound relative to the listener.

| Parameter | Type   | Description                       |
|-----------|--------|-----------------------------------|
| `x`       | number | The X coordinate of the source.   |
| `y`       | number | The Y coordinate of the source.   |
| `z`       | number | The Z coordinate of the source.   |

**Returns:** `boolean` - `true` if the position was set successfully, or `false` otherwise

```lua
src:set_position(10.0, 0.0, -5.0)
```

### st.audio.source:set_velocity(x, y, z) {#lua_api_audio_source_set_velocity}

This method sets the velocity of the source in world space.

The velocity does not move the source; it is only used to calculate the Doppler effect relative to the listener.

| Parameter | Type   | Description                              |
|-----------|--------|------------------------------------------|
| `x`       | number | The X component of the source velocity. |
| `y`       | number | The Y component of the source velocity. |
| `z`       | number | The Z component of the source velocity. |

**Returns:** `boolean` - `true` if the velocity was set successfully, or `false` otherwise

### st.audio.source:set_pitch(pitch) {#lua_api_audio_source_set_pitch}

This method sets the pitch multiplier of the source.

A value of `1.0` plays the sound at its original pitch, values below `1.0` lower the pitch (and slow playback) and values
above `1.0` raise it (and speed up playback). The default is `1.0`.

| Parameter | Type   | Description                          |
|-----------|--------|--------------------------------------|
| `pitch`   | number | The pitch multiplier; must be positive. |

**Returns:** `boolean` - `true` if the pitch was set successfully, or `false` otherwise

### st.audio.source:set_gain(gain) {#lua_api_audio_source_set_gain}

This method sets the gain (volume) of the source.

A value of `1.0` plays the sound at its original volume and `0.0` mutes the source. The default is `1.0`.

| Parameter | Type   | Description                              |
|-----------|--------|------------------------------------------|
| `gain`    | number | The gain multiplier; must not be negative. |

**Returns:** `boolean` - `true` if the gain was set successfully, or `false` otherwise

### st.audio.source:set_loop(loop) {#lua_api_audio_source_set_loop}

This method sets whether the sound restarts automatically when it reaches the end. The default is `false`.

| Parameter | Type    | Description                                  |
|-----------|---------|----------------------------------------------|
| `loop`    | boolean | `true` to loop the sound, `false` to play it once. |

**Returns:** `boolean` - `true` if the loop state was set successfully, or `false` otherwise

### st.audio.source:set_spatial(spatial) {#lua_api_audio_source_set_spatial}

This method sets whether the sound is spatialised according to the position of the source relative to the listener.
The default is `true`.

| Parameter | Type    | Description                                             |
|-----------|---------|---------------------------------------------------------|
| `spatial` | boolean | `true` to spatialise the sound, `false` to play it flat. |

**Returns:** `boolean` - `true` if the spatial state was set successfully, or `false` otherwise

See also
--------

- @ref lua_api_audio_sound
