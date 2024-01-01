
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
    structures/tree/min_heap.h
    structures/tree/min_heap.c
    structures/tree/min_heap_node.h
    structures/tree/min_heap_node.c
    core/header.c
    core/header.h
)

add_executable(transmitr
    main.c
)

target_link_libraries(transmitr transmitr_lib)
target_link_libraries(transmitr readline)
