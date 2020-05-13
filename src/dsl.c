#include <stdlib.h>
#include <string.h>

#include "dsl_parser.h"
#include "dsl.h"

DslValue* NewValue(DslType type, int size)
{
    DslValue* value;
    value = malloc(sizeof(DslValue) + size);
    value->next = NULL;
    value->type = type;
    return value;
}

DslValue* DslValueNewNumber(int number)
{
    DslValue* value;

    value = NewValue(TYPE_NUMBER, 0);
    value->number = number;
    return value;
}

DslValue* DslValueNewStr(char* str)
{
    DslValue* value;

    value = NewValue(TYPE_STR, strlen(str));
    strcpy(value->str, str);
    return value;
}

DslValue* DslValueNewVar(char* var)
{
    DslValue* value;

    value = NewValue(TYPE_VAR, strlen(var));
    strcpy(value->var, var);
    return value;
}

DslValue* DslValueNewElement(DslElement* element)
{
    DslValue* value;
    value = NewValue(TYPE_ELEMENT, 0);
    value->element = element;
    return value;
}

DslElement* DslElementNew(DslValue* name, DslValue* args, DslValue* body)
{
    DslElement* element;
    element = malloc(sizeof(DslElement));

    element->name = name;
    element->args = args;
    element->body = body;

    return element;
}
