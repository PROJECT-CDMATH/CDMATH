file(GLOB NICE_EXAMPLES_TO_INSTALL amr thermique1d transport1d transport2d_ns transport2d_s)

install(DIRECTORY ${NICE_EXAMPLES_TO_INSTALL} DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
