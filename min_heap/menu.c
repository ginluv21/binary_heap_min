#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "minheap.h"

// рекурсивно печатает поддерево с отступами в стиле ls --tree
static void print_node(minheap* h, int ind, char* prefix, int is_last){
    if(ind >= h->len) return;

    printf("%s%s%d\n", prefix, is_last ? "└── " : "├── ", h->data[ind].key);

    char new_prefix[1024];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    int l = get_l_child(ind);
    int r = get_r_child(ind);

    if(l < h->len && r < h->len){
        print_node(h, l, new_prefix, 0);
        print_node(h, r, new_prefix, 1);
    } else if(l < h->len){
        print_node(h, l, new_prefix, 1);
    } else if(r < h->len){
        print_node(h, r, new_prefix, 1);
    }
}

void minheap_print_array(minheap* h){
    if(h == NULL || h->len == 0){ printf("(куча пуста)\n"); return; }
    for(int i = 0; i < h->len; i++){
        printf("%d", h->data[i].key);
        if(i < h->len - 1) printf(" ");
    }
    printf("\n");
}

void minheap_print_tree(minheap* h){
    if(h == NULL || h->len == 0){ printf("(куча пуста)\n"); return; }

    printf("%d\n", h->data[0].key); // корень

    int l = get_l_child(0);
    int r = get_r_child(0);

    if(l < h->len && r < h->len){
        print_node(h, l, "", 0);
        print_node(h, r, "", 1);
    } else if(l < h->len){
        print_node(h, l, "", 1);
    }
}

int main(){
    minheap* h = NULL;
    int choice, n, key, newkey;
    heap_node* node;

    while(1){
        printf("\n=== Min-Heap ===\n");
        printf("1. Создать кучу\n");
        printf("2. Вставить элемент\n");
        printf("3. Извлечь минимум\n");
        printf("4. Показать минимум\n");
        printf("5. Уменьшить ключ\n");
        printf("6. Удалить элемент\n");
        printf("7. Вывести массив\n");
        printf("8. Вывести дерево\n");
        printf("0. Выход\n");
        printf("> ");

        if(scanf("%d", &choice) != 1) break;
        if(choice == 0) break;

        if(choice != 1 && h == NULL){
            printf("Сначала создайте кучу (пункт 1)\n");
            continue;
        }

        switch(choice){
            case 1:
                printf("Размер кучи (макс элементов): ");
                scanf("%d", &n);
                if(h) minheap_free(h);
                h = minheap_create(n);
                printf("Куча создана, ёмкость %d. Добавляйте элементы через пункт 2.\n", n);
                break;

            case 2:
                printf("Сколько элементов: ");
                scanf("%d", &n);
                {
                    int free_slots = h->cap - h->len;
                    if(n > free_slots){
                        printf("Нет места: свободно %d, запрошено %d. Вставлено не будет.\n", free_slots, n);
                        break;
                    }
                }
                printf("Введите %d чисел через пробел: ", n);
                for(int i = 0; i < n; i++){
                    scanf("%d", &key);
                    minheap_insert(h, key, NULL);
                }
                printf("Добавлено %d элементов\n", n);
                break;

            case 3:
                node = minheap_min(h);
                if(node){ printf("Минимум: %d\n", node->key); minheap_extractmin(h); }
                else printf("Куча пуста\n");
                break;

            case 4:
                node = minheap_min(h);
                if(node) printf("Минимум: %d\n", node->key);
                else printf("Куча пуста\n");
                break;

            case 5:
                printf("Ключ: ");
                scanf("%d", &key);
                printf("Новый ключ: ");
                scanf("%d", &newkey);
                minheap_decrease_key(h, key, newkey);
                printf("Готово\n");
                break;

            case 6:
                printf("Ключ: ");
                scanf("%d", &key);
                minheap_delete(h, key);
                printf("Удалён %d\n", key);
                break;

            case 7:
                minheap_print_array(h);
                break;

            case 8:
                minheap_print_tree(h);
                break;

            default:
                printf("Неверный пункт\n");
        }
    }

    if(h) minheap_free(h);
    return 0;
}
