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
