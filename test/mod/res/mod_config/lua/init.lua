st.init.config = {
    { name = 'fullscreen', description = 'Fullscreen.', type = 'boolean', default = false },
    { name = 'player', description = 'Player.', type = 'string', default = 'John Smith' },
    { name = 'seed', description = 'Seed.', type = 'number', default = 12345 },
    { name = 'wrong', description = 'Mismatched type.', type = 'number', default = 'text' },
    { name = 'unknown', description = 'Unknown type.', type = 'table', default = {} },
    { description = 'No name.', type = 'string', default = 'x' },
}
