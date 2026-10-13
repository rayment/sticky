function write_shared()
    if not st.util.config.set('shared', 'changed by owner') then
        expectation_failed()
    end
end
