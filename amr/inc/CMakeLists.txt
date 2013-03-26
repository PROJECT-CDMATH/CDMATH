FILE(GLOB include_amr_HXX . *.hxx)

INSTALL(FILES ${include_amr_HXX} DESTINATION  ${CMAKE_INSTALL_PREFIX}/include)
