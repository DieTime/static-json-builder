dependencies = [
    dependency('munit', fallback: ['munit', 'munit_dep']),
    static_json_builder_dep,
]

sources = [
    'static-json-builder-tests.c'
]

test('static-json-builder-test',
      executable('static-json-builder-test', sources, dependencies: dependencies))
