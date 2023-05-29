sources = [ 'static-json-builder.c' ]
headers = [ 'static-json-builder.h' ]

compile_args_common = []
compile_args_target = []

if get_option('default_library') == 'shared'
    compile_args_common += [ '-DSTATIC_JSON_BUILDER_USE_SHARED_LIBRARY' ]
    compile_args_target += [ '-DSTATIC_JSON_BUILDER_BUILD_SHARED_LIBRARY' ] + compile_args_common
endif

static_json_builder = library('static-json-builder', sources,
                               version: meson.project_version(),
                               c_args: compile_args_target,
                               gnu_symbol_visibility: 'hidden',
                               install: true)

static_json_builder_dep = declare_dependency(compile_args: compile_args_common,
                                             link_with: static_json_builder,
                                             include_directories: include_directories('.'))

pkg = import('pkgconfig')
pkg.generate(static_json_builder, description: 'Library for easy building static json\'s')

install_headers(headers)
