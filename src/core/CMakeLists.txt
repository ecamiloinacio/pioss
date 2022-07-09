add_library(piosscore STATIC pioss.c param.c mds.c cli.c dts.c csvexporter.c)

target_link_libraries(piosscore PUBLIC piossutil piossfiledist)

target_include_directories(piosscore PUBLIC ${PROJECT_SOURCE_DIR}/include)