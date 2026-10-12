local function expect(condition)
    if not condition then
        expectation_failed()
    end
end

function check_textures()
    local good = st.video.texture.get('good')
    expect(good ~= nil)
    expect(good:name() == 'good')
    expect(good:path() == 'good.png')
    expect(good:width() == 16 and good:height() == 16)

    local nested = st.video.texture.get('nested')
    expect(nested ~= nil)
    expect(nested:path() == 'sub/nested.png')

    expect(st.video.texture.get('missing') == nil)
    expect(st.video.texture.get('escape') == nil)
    expect(st.video.texture.get('unknown') == nil)
end

function read_init_textures()
    return st.init.textures
end

function write_init_textures()
    st.init.textures = {}
end

function log_from_runtime()
    st.io.log.info('log from runtime phase')
end
