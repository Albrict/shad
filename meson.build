project('shad', 'c', version: '0.1.0', 
        default_options : ['c_std=c99'])
sanitazers = ['-fsanitize=address', '-fsanitize=leak', '-fsanitize=undefined', '-fsanitize=null']
    
notcurses_dep = dependency('notcurses')
cjson_dep  = dependency('libcjson')

src = ['src/main.c', 'src/shad.c', 'src/shad_error.c', 'src/input.c', 'src/canvas.c',
  'src/instrument_panel.c', 'src/palette.c', 'src/util.c', 'src/button.c', 'src/label.c', 
  'src/color_picker_panel.c', 'src/choosen_color_panel.c', 'src/palette_window.c', 'src/panel_list.c']

if get_option('buildtype') == 'debug'
    add_project_arguments(sanitazers, language : 'c')
    add_project_link_arguments(sanitazers, language : 'c')
endif

executable('shad', src, dependencies: [notcurses_dep, cjson_dep])
