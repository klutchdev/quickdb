#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document.h"
#include "collection.h"

Document *createDocument(const char *documentId)
{
  Document *doc = (Document *)malloc(sizeof(Document));
  strcpy(doc->documentId, documentId);
  doc->fields = NULL;
  doc->fieldCount = 0;
  return doc;
}

void addField(Document *doc, const char *key, ValueType type, void *value)
{
  doc->fields = (Field *)realloc(doc->fields, (doc->fieldCount + 1) * sizeof(Field));
  Field *newField = &doc->fields[doc->fieldCount];
  strcpy(newField->key, key);
  newField->type = type;

  if (type == TYPE_INT)
  {
    newField->value.intValue = *(int *)value;
  }
  else if (type == TYPE_STRING)
  {
    strcpy(newField->value.stringValue, (char *)value);
  }

  doc->fieldCount++;
}

void updateField(Document *doc, const char *key, ValueType type, void *value)
{
  for (size_t i = 0; i < doc->fieldCount; i++)
  {
    if (strcmp(doc->fields[i].key, key) == 0)
    {
      doc->fields[i].type = type;
      if (type == TYPE_INT)
      {
        doc->fields[i].value.intValue = *(int *)value;
      }
      else if (type == TYPE_STRING)
      {
        strcpy(doc->fields[i].value.stringValue, (char *)value);
      }
      return;
    }
  }
  addField(doc, key, type, value);
}

void deleteField(Document *doc, const char *key)
{
  for (size_t i = 0; i < doc->fieldCount; i++)
  {
    if (strcmp(doc->fields[i].key, key) == 0)
    {
      for (size_t j = i; j < doc->fieldCount - 1; j++)
      {
        doc->fields[j] = doc->fields[j + 1];
      }
      doc->fieldCount--;
      doc->fields = (Field *)realloc(doc->fields, doc->fieldCount * sizeof(Field));
      return;
    }
  }
}

void printDocument(const Document *doc)
{
  printf("Document ID: %s\n", doc->documentId);
  for (size_t i = 0; i < doc->fieldCount; i++)
  {
    printf("  %s: ", doc->fields[i].key);
    if (doc->fields[i].type == TYPE_INT)
    {
      printf("%d\n", doc->fields[i].value.intValue);
    }
    else if (doc->fields[i].type == TYPE_STRING)
    {
      printf("%s\n", doc->fields[i].value.stringValue);
    }
  }
}

void freeDocument(Document *doc)
{
  free(doc->fields);
}
