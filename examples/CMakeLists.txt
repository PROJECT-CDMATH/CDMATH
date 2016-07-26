file(GLOB NICE_EXAMPLES_TO_INSTALL poisson thermique1d transport1d transport2d_ns transport2d_s)

install(DIRECTORY ${NICE_EXAMPLES_TO_INSTALL} DESTINATION share/doc/cdmath/examples)
