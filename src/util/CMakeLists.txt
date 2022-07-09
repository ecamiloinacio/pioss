add_library(piossutil STATIC memalloc.c logger.c timing.c screen.c)

target_include_directories(piossutil PUBLIC ${PROJECT_SOURCE_DIR}/include)