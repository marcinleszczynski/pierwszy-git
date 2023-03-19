#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
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
