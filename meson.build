project('static-json-builder', 'c',
         default_options: ['c_std=c99', 'warning_level=3'],
         version: '0.1.0')

subdir('lib')

if get_option('tests')
    subdir('tests')
endif

if get_option('examples')
    subdir('examples')
endif
