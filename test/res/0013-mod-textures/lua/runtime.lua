function check_textures()
    local good = st.video.texture.get('good')
    assert(good ~= nil)
    assert(good:name() == 'good')
    assert(good:path() == 'good.png')
    assert(good:width() == 16 and good:height() == 16)

    local nested = st.video.texture.get('nested')
    assert(nested ~= nil)
    assert(nested:path() == 'sub/nested.png')

    assert(st.video.texture.get('missing') == nil)
    assert(st.video.texture.get('escape') == nil)
    assert(st.video.texture.get('unknown') == nil)
end

function check_init_locked()
    assert(not pcall(function() return st.init.textures end))
    assert(not pcall(function() st.init.textures = {} end))
end
