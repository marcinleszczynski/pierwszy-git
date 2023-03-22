#include<stdio.h>
#include<stdlib.h>
struct section
{
    char* name;
    char** keys;
    char** values;
    int length;
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
        *list = malloc(sizeof(struct section_list));
        (*list) -> SECTION = new_section;
        (*list) -> next = NULL;
    }
    else
    {
        struct section_list* p = *list;
        while(p->next != NULL)
            p = p->next;
        p->next = malloc(sizeof(struct section_list));
        p->next->SECTION = new_section;
        p->next->next = NULL;
    }
}
void add_pair(struct section** sect, char* key, char* value)
{
    int len = (*sect) -> length;
    if(len == 0)
    {
        (*sect) -> keys = malloc(sizeof(char*));
        (*sect) -> values = malloc(sizeof(char*));
    }
    else
    {
        (*sect) -> keys = realloc((*sect) -> keys, (len+1)*sizeof(char*));
        (*sect) -> values = realloc((*sect) -> values, (len+1)*sizeof(char*));
    }
    (*sect)->keys[len] = calloc(1, strlen(key)+1);
    (*sect)->values[len] = calloc(1, strlen(value)+1);
    strncpy((*sect)->keys[len], key, strlen(key));
    strncpy((*sect)->values[len], value, strlen(value));
    (*sect) -> length += 1;
}
void print_list(struct section_list* list)
{
    struct section_list* p = list;
    while(p != NULL)
    {
        printf("[%s]\n", p->SECTION->name);
        int len = p->SECTION->length;
        for(int i=0; i<len; i++)
        {
            printf("%s = %s\n", p->SECTION->keys[i], p->SECTION->values[i]);
            fflush(stdout);
        }
        p = p->next;
        printf("\n");
    }
    printf("\n");
}
