#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"text_functions.c"
#include"structures.c"
int main(int argc, char **argv)
{
    char *file_name = "TEST2.ini";//argv[1];
    char *s = "doting-number.vapid-bite";//argv[2];
    char **split = split_by_dot(s);
    char *section = split[0];
    char *key = split[1];
    int len_section = strlen(section);
    int len_key = strlen(key);
    FILE *file;
    file = fopen(file_name, "r");
    char line[70];
    int found_section = 0;
    int found_key = 0;
    CLASS cl;
    struct section_list* p_file;
    p_file = NULL;
    int a = 0;
    while(fgets(line, 70, file) != NULL)
    {
        line[strlen(line)-1] = '\0';
        cl = classify(line);
        if(cl == invalid)
        {
            printf("Invalid line: ");
            puts(line);
            break;
        }
        else if(cl == section_name)
        {
            struct section* sect;
            sect = malloc(sizeof(struct section));
            sect->name = no_brackets(line);
            sect->keys = NULL;
            add_section(&p_file, sect);
        }
        else if(cl == 3)
        {
            if(p_file == NULL)
            {
                puts("Error: missing section name");
                break;
            }
            char** key_value = split_by_eq(line);
            struct pair* new_pair = malloc(sizeof(struct pair*));
            new_pair->key = key_value[0];
            strncpy(new_pair->key, key_value[0], strlen(key_value[0]));
            new_pair->value = key_value[1];
            strncpy(new_pair->value, key_value[1], strlen(key_value[1]));
            struct section_list* p = p_file;
            while(p->next != NULL)
                p = p->next;
            add_pair(&(p->SECTION->keys), new_pair);
            print_list(p_file);
        }
    }
    print_list(p_file);
    fclose(file);
    return 0;
}
