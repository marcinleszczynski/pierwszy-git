#include<stdio.h>
#include<stdlib.h>
struct pair
{
    char* key;
    char* value;
};
struct pair_list
{
    struct pair* values;
    struct pair_list* next;
};
struct section
{
    char* name;
    struct pair_list* keys;
};
struct section_list
{
    struct section* SECTION;
    struct section_list* next;
};
void add_section(struct section_list** list, struct section* new_section)
{
    if(*list == NULL)
    {
        *list = malloc(sizeof(struct section*)+sizeof(struct section_list*));
        (*list) ->SECTION = new_section;
        (*list) ->next = NULL;
    }
    else
    {
        struct section_list* p = *list;
        while(p->next != NULL)
            p = p->next;
        p->next = malloc(sizeof(struct section_list*));
        p->next->SECTION = new_section;
        p->next->next = NULL;
    }
}
void add_pair(struct pair_list** list, struct pair* new_pair)
{
    if(*list == NULL)
    {
        *list = malloc(sizeof(struct pair_list*));
        (*list) -> values = new_pair;
        (*list) -> next = NULL;
    }
    else
    {
        struct pair_list* p = *list;
        while(p->next != NULL)
            p = p->next;
        p->next = malloc(sizeof(struct pair_list*));
        p->next->values = new_pair;
        p->next->next = NULL;
    }
}
void print_list(struct section_list* list)
{
    struct section_list* p = list;
    while(p != NULL)
    {
        printf("[%s]\n", p->SECTION->name);
        struct pair_list* s = p->SECTION->keys;
        while(s != NULL)
        {
            printf("%s = %s\n", s->values->key, s->values->value);
            s = s->next;
        }
        p = p->next;
        printf("\n");
    }
    printf("\n");
}
/*int main()
{
    struct section_list* list;
    list = NULL;
    struct pair_list* list_p;
    list_p = NULL;
    struct pair* PAIR;
    PAIR = malloc(sizeof(char*)*2);
    PAIR->key = calloc(1, 4);
    PAIR->value = calloc(1, 4);
    PAIR->key = "abc";
    PAIR->value = "def";
    struct pair* PAIR2;
    PAIR2 = malloc(sizeof(char*)*2);
    PAIR2->key = calloc(1, 4);
    PAIR2->value = calloc(1, 4);
    PAIR2->key = "ghi";
    PAIR2->value = "jkl";
    //adding pair test
    add_pair(&list_p, PAIR);
    add_pair(&list_p, PAIR2);
    struct section* sect1;
    sect1 = malloc(sizeof(struct section*));
    sect1->name = "section1";
    sect1->keys = list_p;
    add_section(&list, sect1);
    print_list(list);
    return 0;
}*/
