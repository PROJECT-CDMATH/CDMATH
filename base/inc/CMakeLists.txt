FILE(GLOB include_base_HXX . *.hxx)

INSTALL(FILES ${include_base_HXX} DESTINATION  ${CMAKE_INSTALL_PREFIX}/include)
