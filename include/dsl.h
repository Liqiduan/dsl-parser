#ifndef __DSL_H__
#define __DSL_H__ 

typedef enum {
    TYPE_VAR,
    TYPE_NUMBER,
    TYPE_STR,
    TYPE_ELEMENT
} DslType;

typedef struct DslElement DslElement;

typedef struct DslValue {
    struct DslValue* next;
    DslType type;
    union {
        int number;
        char str[0];
        char var[0];
        DslElement* element;
    };
} DslValue;

typedef struct DslElement {
    DslValue* name;
    DslValue* args;
    DslValue* body;
} DslElement;

int DslParse(char* str, DslElement** dsl);

DslValue* DslValueNewNumber(int number);
DslValue* DslValueNewStr(char* str);
DslValue* DslValueNewVar(char* var);
DslValue* DslValueNewElement(DslElement* element);

DslElement* DslElementNew(DslValue* name, DslValue* args, DslValue* body);

#endif /* ifndef __DSL_H__ */
