FILE(GLOB cdmath_examples . Amr Transport2D_NACA Transport1D Transport2D_CARTESIAN)

INSTALL(DIRECTORY ${cdmath_examples} DESTINATION ${CMAKE_INSTALL_PREFIX}/${REP_EXAMPLES})
