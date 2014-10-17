file(GLOB nice_examples_directories thermique1d transport1d transport2d_ns transport2d_s)

install(DIRECTORY ${nice_examples_directories} DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
