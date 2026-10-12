function check_sounds()
    local good = st.audio.sound.get('good')
    assert(good ~= nil)
    assert(good:name() == 'good')
    assert(good:path() == 'good.wav')

    local nested = st.audio.sound.get('nested')
    assert(nested ~= nil)
    assert(nested:path() == 'sub/nested.wav')

    assert(st.audio.sound.get('missing') == nil)
    assert(st.audio.sound.get('escape') == nil)
    assert(st.audio.sound.get('unknown') == nil)
end

function check_init_locked()
    assert(not pcall(function() return st.init.sounds end))
    assert(not pcall(function() st.init.sounds = {} end))
end

function check_source()
    local src = st.audio.source()
    assert(src ~= nil)
    assert(not src:is_playing())

    assert(src:select(st.audio.sound.get('good')))
    assert(src:set_position(1.0, 2.0, 3.0))
    assert(src:set_velocity(0.0, 0.5, 0.0))
    assert(src:set_pitch(1.5))
    assert(src:set_gain(0.5))
    assert(src:set_loop(true))
    assert(src:set_spatial(true))

    assert(src:play())
    assert(src:is_playing())
    assert(src:pause())
    assert(src:is_paused())
    assert(src:resume())
    assert(src:is_playing())
    assert(src:stop())
    assert(src:is_stopped())

    assert(not pcall(function() src:select(nil) end))
    assert(not pcall(function() src:set_position(1.0) end))
end
