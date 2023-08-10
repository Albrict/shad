project('shad', 'c', version: '0.1.0', 
        default_options : ['c_std=c11'])
    
notcurses_dep = dependency('notcurses')

src = ['src/main.c', 'src/shad.c', 'src/shad_error.c', 'src/canvas.c',
  'src/instrument_panel.c', 'src/palette.c', 'src/util.c', 'src/button.c', 'src/label.c', 
  'src/color_picker_panel.c', 'src/selected_color_panel.c', 'src/ncpanel.c', 'src/state.c',
  'libs/cJSON.c', 'libs/libattopng.c'] 

executable('shad', src, dependencies: [notcurses_dep])
