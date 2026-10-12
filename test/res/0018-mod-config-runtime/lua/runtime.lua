local function expect(condition)
    if not condition then
        expectation_failed()
    end
end

load_time_flag = st.util.config.get('flag')

function check_init_read()
    expect(init_read_result == nil)
end

function check_load_time()
    expect(load_time_flag == false)
end

function check_defaults()
    expect(st.util.config.get('flag') == false)
    expect(st.util.config.get('label') == 'alpha')
    expect(st.util.config.get('count') == 3)
    expect(st.util.config.get('missing') == nil)
end

function write_values()
    expect(st.util.config.set('flag', true))
    expect(st.util.config.set('label', 'beta'))
    expect(st.util.config.set('count', 7.5))
    expect(st.util.config.get('flag') == true)
    expect(st.util.config.get('label') == 'beta')
    expect(st.util.config.get('count') == 7.5)
end

function reject_wrong_type()
    expect(st.util.config.set('flag', 'yes') == false)
    expect(st.util.config.set('count', true) == false)
    expect(st.util.config.set('label', 5) == false)
    expect(st.util.config.set('flag', {}) == false)
    expect(st.util.config.get('flag') == false)
end

function reject_non_finite()
    expect(st.util.config.set('count', 0 / 0) == false)
    expect(st.util.config.set('count', math.huge) == false)
    expect(st.util.config.get('count') == 3)
end

function reject_undeclared()
    expect(st.util.config.set('missing', 1) == false)
    expect(st.util.config.get('missing') == nil)
end

function read_other()
    expect(st.util.config.get_from('0019-mod-config-other', 'shared') == 'from other')
    expect(st.util.config.get_from('0019-mod-config-other', 'missing') == nil)
    expect(st.util.config.get_from('no-such-mod', 'shared') == nil)
    expect(st.util.config.get_from('0018-mod-config-runtime', 'flag') == false)
end

function write_other()
    expect(st.util.config.set('shared', 'hijacked') == false)
    expect(st.util.config.get_from('0019-mod-config-other', 'shared') == 'from other')
end

function write_values_in_coroutine()
    local routine = coroutine.wrap(function()
        expect(st.util.config.set('label', 'from coroutine'))
    end)
    routine()
end

function check_stored_count()
    expect(st.util.config.get('count') == 9)
end
