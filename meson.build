project(
  'polyvoid',
  'c',
  version: '0.1.0',
  license: 'Apache-2.0',
  default_options: [
    'werror=true',
    'warning_level=3',
    'c_std=c89',
    'default_library=static'])

defines = ['-DPV_IMPLEMENTATION=1']
polyvoid_includes = include_directories('include')
polyvoid = declare_dependency(
  link_with: library(
    'polyvoid',
    'src/hashmap.c',
    'src/linked_list.c',
    include_directories: polyvoid_includes,
    c_args: defines),
  include_directories: polyvoid_includes)

subdir('test')
