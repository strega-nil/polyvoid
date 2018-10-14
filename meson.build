project(
  'polyvoid',
  'c',
  version: '0.1.0',
  license: 'Apache-2.0',
  default_options: [
    'werror=true',
    'warning_level=3',
    'c_std=c99',
    'default_library=static'])

compiler = meson.get_compiler('c').get_id()
if compiler == 'msvc'
  add_project_arguments('-wd4204', language: 'c')
endif


polyvoid_includes = include_directories('include')
polyvoid = declare_dependency(
  link_with: library(
    'polyvoid',
    'src/hashmap.c',
    'src/linked_list.c',
    include_directories: polyvoid_includes),
  include_directories: polyvoid_includes)

subdir('test')