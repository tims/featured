env = Environment()
env.Append(
  LIBS=['thrift', 'boost_system-mt','boost_thread-mt', 'boost_filesystem-mt'], 
  CCFLAGS = ['-g','-O3', '-DHAVE_INTTYPES_H', '-DHAVE_NETINET_IN_H'])
env.Append(CPPPATH=['/usr/local/include/thrift','src/gen-cpp'])


sources = [
  'src/featured_server.cpp',
  'src/FeatureIndex.cpp',
  'src/gen-cpp/featured_constants.cpp',
  'src/gen-cpp/featured_types.cpp',
  'src/gen-cpp/FeaturedService.cpp'
]
featured_server = env.Program(target='bin/featured_server', source=sources)

Default(featured_server)

