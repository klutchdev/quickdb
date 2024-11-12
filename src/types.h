#ifndef TYPES_H
#define TYPES_H

typedef enum
{
    TYPE_INT,
    TYPE_STRING,
    TYPE_DOCUMENT
} ValueType;

typedef struct
{
    char key[50];
    ValueType type;
    union
    {
        int intValue;
        char stringValue[100];
    } value;
} Field;

typedef struct
{
    char documentId[50];
    Field *fields;
    size_t fieldCount;
} Document;

typedef struct
{
    char collectionName[50];
    Document *documents;
    size_t documentCount;
} Collection;

typedef struct
{
    Collection *collections;
    size_t collectionCount;
} CollectionManager;
#endif // TYPES_H
