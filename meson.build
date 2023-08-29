project('shad', 'c', version: '0.1.0', 
        default_options : ['c_std=c11'])
    
cc = meson.get_compiler('c')
sanitazers = ['-fsanitize=address', '-fsanitize=leak', '-fsanitize=undefined', '-fsanitize=null']

deps = [
  cc.find_library('notcurses'),
  cc.find_library('notcurses-core')
]

src = ['src/main.c', 'src/shad.c', 'src/shad_error.c', 'src/canvas.c',
  'src/instrument_panel.c', 'src/palette.c', 'src/button.c', 'src/label.c', 
  'src/color_picker_panel.c', 'src/selected_color_panel.c', 
  'src/ncpanel.c', 'src/state.c', 'src/main_screen_panel.c',
  'src/color.c',
  'src/menu.c', 'libs/cJSON.c', 'libs/libattopng.c'] 

executable('shad', src, dependencies: [deps])
