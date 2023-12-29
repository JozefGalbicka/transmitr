
add_library (transmitr_lib
    core/client.h
    core/client.c
    core/server.h
    core/server.c
    structures/list/array_list.h
    structures/list/array_list.c
    ui/console.h
    ui/console.c
    utils/strings.h
    utils/strings.c
    utils/prints.h
    utils/prints.c
)

add_executable(transmitr
    main.c
)

target_link_libraries(transmitr transmitr_lib)
target_link_libraries(transmitr readline)
