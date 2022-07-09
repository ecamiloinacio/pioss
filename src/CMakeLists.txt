project("PIOSS Lite" VERSION 1.0 
                     DESCRIPTION "Parallel I/O and Storage System (PIOSS) simulation model; Lite Version"
                     LANGUAGES C)

add_subdirectory(util)
add_subdirectory(core)
add_subdirectory(file_dists)

add_executable(piosslite main.c)

target_link_libraries(piosslite PUBLIC piossutil piosscore)