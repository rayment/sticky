st.init.config = {
    { name = 'flag', description = 'Flag.', type = 'boolean', default = false },
    { name = 'label', description = 'Label.', type = 'string', default = 'alpha' },
    { name = 'count', description = 'Count.', type = 'number', default = 3 },
}

init_read_result = st.util.config.get('flag')
