#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"structures.c"
char** split_by_dot(char* text)
{
    int len1 = 0;
    while(text[len1] != '.')
        len1++;
    int len2 = strlen(text) - len1 - 1;
    char** array = malloc(sizeof(char*)*2);
    array[0] = calloc(1, len1+1);
    array[1] = calloc(1, len2+1);
    strncpy(array[0], text, len1);
    strncpy(array[1], text+len1+1, len2);
    return array;
}
char** split_by_eq(char* text)
{
    int len1 = 0;
    while(text[len1] != ' ')
        len1++;
    int len2 = strlen(text) - len1 - 3;
    char** array = malloc(sizeof(char*)*2);
    array[0] = calloc(1, len1+1);
    array[1] = calloc(1, len2+1);
    strncpy(array[0], text, len1);
    strncpy(array[1], text+len1+3, len2);
    return array;
}
char* no_brackets(char* text)
{
    int len = strlen(text);
    char* new_string = calloc(1, len-1); //length - 2([, ]) + null character, so -2 + 1 -> (-1)
    strncpy(new_string, text+1, len-2);
    return new_string;
}
typedef enum CLASS {section_name,comment,empty,key,invalid} CLASS;
CLASS classify(char *text)
{
    int l = strlen(text);
    if(!l)
        return empty;
    if(text[0] == ';')
        return comment;
    if(text[0] == '[' && text[l-1] == ']')
    {
        for(int i=1; i < l-1; i++)
            if(!isalnum(text[i]) && text[i] != '-')
                return invalid;
        return section_name;
    }
    if(strstr(text, " = "))
    {
        int i = 0;
        while(text[i] != ' ')
        {
            if(!isalnum(text[i]) && text[i] != '-')
                return invalid;
            i++;
        }
        i += 3;
        while(i < l)
        {
            if(!isalnum(text[i]) && text[i] != '-')
                return invalid;
            i++;
        }
        return key;
    }
    return invalid;
}
char* getline(FILE* file)
{
    int l = 0;
    char c;
    char* line;
    if((c = fgetc(file)) == EOF)
        return NULL;
    if(c == '\n')
        return "";
    line = malloc(sizeof(char));
    line[l] = c;
    l++;
    while((c = fgetc(file)) != '\n')
    {
        line = realloc(line, (l+1)*sizeof(char));
        line[l] = c;
        l++;
    }
    line = realloc(line, (l+1)*sizeof(char));
    line[l] = '\0';
    return line;
}
struct section_list* parse_file(FILE* file)
{
    char *line;
    CLASS cl;
    struct section_list* p_file;
    p_file = NULL;
    //parse the data to p_file
    while((line = getline(file)) != NULL)
    {
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
            sect->length = 0;
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
            struct section_list* p = p_file;
            while(p->next != NULL)
                p = p->next;
            char* new_key = key_value[0];
            char* new_value = key_value[1];
            add_pair(&(p->SECTION), new_key, new_value);
        }
    }
    return p_file;
}
char* find_by_section(struct section_list* list, char* section_find, char* key_find)
{
    int found_section = 0;
    int found_key = 0;
    char* result;
    struct section_list* p = list;
    while(p != NULL)
    {
        if(!strcmp(p->SECTION->name, section_find))
        {
            found_section = 1;
            for(int i=0; i < p->SECTION->length; i++)
            {
                if(!strcmp(p->SECTION->keys[i], key_find))
                {
                    found_key = 1;
                    result = p->SECTION->values[i];//puts(p->SECTION->values[i]);
                    break;
                }
            }
        }
        p = p->next;
    }
    if(found_key)
    {
        return result;
    }
    else
    {
        if(found_section)
            printf("Failed to find key %s in section[%s]\n", key_find, section_find);
        else
            printf("Failed to find section [%s]\n", section_find);
        return NULL;
    }
}
char** split_expression(char* exp)
{
    int len1 = 0;
    while(exp[len1] != ' ')
        len1++;
    int len2 = strlen(exp) - len1 - 3;
    char** array = malloc(sizeof(char*)*3);
    array[0] = calloc(1, len1+1);
    array[1] = calloc(1, 2);
    array[2] = calloc(1, len2+1);
    strncpy(array[0], exp, len1);
    strncpy(array[1], exp+len1+1, 1);
    strncpy(array[2], exp+len1+3, len2);
    return array;
}
int isint(char* string)
{
    if(!isdigit(string[0]) || (string[0] == '0' && strlen(string) > 1))
        return 0;
    for(int i=1; i<strlen(string); i++)
    {
        if(!isdigit(string[i]))
            return 0;
    }
    return 1;
}
char* evaluate(char* expression, struct section_list* p_file)
{
    char** exp_split = split_expression(expression);
    char** split1 = split_by_dot(exp_split[0]);
    char** split2 = split_by_dot(exp_split[2]);
    char* value1 = find_by_section(p_file, split1[0], split1[1]);
    char* value2 = find_by_section(p_file, split2[0], split2[1]);
    if(!value1 || !value2)
        return NULL;
    char operand = exp_split[1][0];
    int int1 = isint(value1);
    int int2 = isint(value2);
    int both_int = int1 && int2;
    if(int1 ^ int2)
        return NULL;
    switch(operand)
    {
        case '+':
            if(both_int)
            {
                long result;
                result = strtol(value1, NULL, 10) + strtol(value2, NULL, 10);
                char str[12];
                sprintf(str, "%d", result);
                char* s = calloc(1, strlen(str));
                strcpy(s, str);
                return s;
            }
            else
            {
                char result[strlen(value1)+strlen(value2)+1];
                strcpy(result, value1);
                strcat(result, value2);
                char* s = calloc(1, strlen(result)+1);
                strcpy(s, result);
                return s;
            }
            break;
        case '-':
            if(both_int)
            {
                long result;
                result = strtol(value1, NULL, 10) - strtol(value2, NULL, 10);
                char str[12];
                sprintf(str, "%d", result);
                char* s = calloc(1, strlen(str)+1);
                strcpy(s, str);
                return s;
            }
            else
            {
                return NULL;
            }
            break;
        case '*':
            if(both_int)
            {
                long result;
                result = strtol(value1, NULL, 10) * strtol(value2, NULL, 10);
                char str[12];
                sprintf(str, "%d", result);
                char* s = calloc(1, strlen(str)+1);
                strcpy(s, str);
                return s;
            }
            else
            {
                return NULL;
            }
            break;
        case '/':
            if(both_int)
            {
                float result = (float)strtol(value1, NULL, 10) / strtol(value2, NULL, 10);
                char str[24];
                sprintf(str, "%f", result);
                char* s = calloc(1, strlen(str)+1);
                strcpy(s, str);
                return s;
            }
            else
            {
                return NULL;
            }
            break;
    }
    return NULL;
}
