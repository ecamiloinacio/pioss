add_library(piossfiledist STATIC file_dist_random.c file_dist_roundrobin.c file_dist_n2r2.c)

target_link_libraries(piossfiledist PUBLIC tinymt)

target_include_directories(piossfiledist PUBLIC ${PROJECT_SOURCE_DIR}/include)