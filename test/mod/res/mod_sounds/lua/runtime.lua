local function expect(condition)
    if not condition then
        expectation_failed()
    end
end

function check_sounds()
    local good = st.audio.sound.get('good')
    expect(good ~= nil)
    expect(good:name() == 'good')
    expect(good:path() == 'good.wav')

    local nested = st.audio.sound.get('nested')
    expect(nested ~= nil)
    expect(nested:path() == 'sub/nested.wav')

    expect(st.audio.sound.get('missing') == nil)
    expect(st.audio.sound.get('escape') == nil)
    expect(st.audio.sound.get('unknown') == nil)
end

function read_init_sounds()
    return st.init.sounds
end

function write_init_sounds()
    st.init.sounds = {}
end

function check_source()
    local src = st.audio.source()
    expect(src ~= nil)
    expect(not src:is_playing())

    expect(src:set_sound(st.audio.sound.get('good')))
    expect(src:set_position(1.0, 2.0, 3.0))
    expect(src:set_velocity(0.0, 0.5, 0.0))
    expect(src:set_pitch(1.5))
    expect(src:set_gain(0.5))
    expect(src:set_loop(true))
    expect(src:set_spatial(true))

    expect(src:play())
    expect(src:is_playing())
    expect(src:pause())
    expect(src:is_paused())
    expect(src:resume())
    expect(src:is_playing())
    expect(src:stop())
    expect(src:is_stopped())
end

function set_nil_sound()
    st.audio.source():set_sound(nil)
end

function set_position_without_components()
    st.audio.source():set_position(1.0)
end
