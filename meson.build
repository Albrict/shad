project('shad', 'c', version: '0.1.0')
sanitazers = ['-fsanitize=address', '-fsanitize=leak', '-fsanitize=undefined', '-fsanitize=null']
    
notcurses_dep = dependency('notcurses')

cc = meson.get_compiler('c')

src = ['src/main.c', 'src/shad.c', 'src/screen.c', 'src/shad_error.c']

if get_option('buildtype') == 'debug'
    add_project_arguments(sanitazers, language : 'c')
    add_project_link_arguments(sanitazers, language : 'c')
endif

executable('shad', src, dependencies: [notcurses_dep])
