cmake_minimum_required(VERSION 3.5)
project(C-Coroutine)

set(SOURCES
    coroutine.c
    queue.c
    main.c
)

add_executable(main ${SOURCES})
