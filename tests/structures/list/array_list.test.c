
#include "structures/list/array_list.h"
#include "utils/prints.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// ---------------
// Helper funcs
// ---------------
static ArrayList *test_array_list_create() {
    ArrayList *list;
    list = malloc(sizeof(ArrayList));
    array_list_init(list, sizeof(int));
    return list;
}

static void test_array_list_fill(ArrayList *list, size_t count) {
    int tmp;
    for (int i = 0; i < count; i++) {
        // tmp = rand() % 100;
        array_list_add(list, &i);
    }
}

static _Bool test_array_list_delete(ArrayList *list) {
    array_list_destroy(list);
    free(list);
    list = NULL;
    return true;
}

// ---------------
// Actual tests
// ---------------
void test_array_list_binary_save_and_load() {
    printf("### test_array_list_binary_save_and_load()\n\n");
    // Writing to binary file
    ArrayList *list = test_array_list_create();
    test_array_list_fill(list, 20);
    printf("Writing array to file:\n");
    array_list_to_txt(list, stdout, print_int);
    printf("\n");
    array_list_save_to_file(list, "array-list.bin");

    // Reading from binary file
    ArrayList list2;
    array_list_read_from_file(&list2, "array-list.bin");
    printf("Loaded array from file:\n");
    array_list_to_txt(&list2, stdout, print_int);
    printf("\n");

    if (!array_list_equals(list, &list2)) {
        fprintf(stderr, "LISTS ARE NOT EQUAL");
        test_array_list_delete(list);
        array_list_destroy(&list2);
        exit(1);
    }

    test_array_list_delete(list);
    array_list_destroy(&list2);
    printf("\n\n");
}

void test_array_list_iterator() {
    printf("### test_array_list_iterator()\n\n");

    // Create array_list
    ArrayList *list;
    list = test_array_list_create();
    test_array_list_fill(list, 10);

    array_list_to_txt(list, stdout, print_int);
    printf("\n");

    ArrayListIterator it;
    array_list_iterator_init(&it, list);
    while (array_list_iterator_has_next(&it)) {
        void *tmp = array_list_iterator_move_next(&it);
        if (!(*(int *)tmp == it.index - 1)) {
            array_list_iterator_destroy(&it);
            test_array_list_delete(list);
            exit(1);
        }
        print_int(tmp, stdout);
    }
    printf("<END>\n");
    array_list_iterator_destroy(&it);

    test_array_list_delete(list);

    printf("\n\n");
}

void test_array_list_set() {
    printf("### test_array_list_set()\n\n");

    ArrayList *list = test_array_list_create();
    test_array_list_fill(list, 9);

    array_list_to_txt(list, stdout, print_int);
    printf("Before 'set':\n");

    int i = 99;
    array_list_set_at(list, 2, &i);
    array_list_set_at(list, 4, &i);
    array_list_set_at(list, 8, &i);

    printf("After 'set':\n");
    array_list_to_txt(list, stdout, print_int);

    ArrayListIterator it;
    array_list_iterator_init(&it, list);
    while (array_list_iterator_has_next(&it)) {
        void *tmp = array_list_iterator_move_next(&it);
        if (it.index == 2 + 1 || it.index == 4 + 1 || it.index == 8 + 1) {
            if (*(int *)tmp != 99) {
                fprintf(stderr, "Wrong value set");

                array_list_iterator_destroy(&it);
                test_array_list_delete(list);

                exit(1);
            }
        }
    }
    printf("<END>\n");
    array_list_iterator_destroy(&it);
    test_array_list_delete(list);
    printf("\n\n");
}

void test_array_list_assign() {
    printf("test_array_list_assign()\n\n");
    // Original list
    ArrayList *list = test_array_list_create();
    test_array_list_fill(list, 20);
    printf("Generated list:\n");
    array_list_to_txt(list, stdout, print_int);
    printf("\n");

    // Assign list into list2
    ArrayList list2;
    array_list_init(&list2, sizeof(int));
    array_list_assign(&list2, list);
    printf("Assigned list:\n");
    array_list_to_txt(&list2, stdout, print_int);
    printf("\n");

    if (!array_list_equals(list, &list2)) {
        fprintf(stderr, "LISTS ARE NOT EQUAL");
        test_array_list_delete(list);
        array_list_destroy(&list2);
        exit(1);
    }

    test_array_list_delete(list);
    array_list_destroy(&list2);

    printf("\n\n");
}

int main(void) {
    printf("\n");
    test_array_list_binary_save_and_load();
    test_array_list_iterator();
    test_array_list_set();
    test_array_list_assign();
}
