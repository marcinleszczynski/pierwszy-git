#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"text_functions.c"
int main(int argc, char **argv)
{
    FILE *file;
    char *file_name = argv[1];
    file = fopen(file_name, "r");
    struct section_list* p_file = parse_file(file); //parse file to section_list - for 4.0
    //for 3.0
    if(argc == 3)
    {
        char *s = argv[2];
        char **split = split_by_dot(s);
        char *section_find = split[0];
        char *key_find = split[1];
        struct section_list* p = p_file;
        //find the value
        char* value = find_by_section(p_file, section_find, key_find);
        if(value)
            puts(value);
    }
    else if(!strcmp(argv[2], "expression")) //for 5.0
    {
        char* expression = argv[3];
        char* value = evaluate(expression, p_file);
        if(value != NULL)
        {
            puts(value);
        }
        else
        {
            puts("invalid expression");
        }
    }
    fclose(file);
    return 0;
}
