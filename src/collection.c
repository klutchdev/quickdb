#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "collection.h"
#include "document.h"

Collection *createCollection(const char *name)
{
  Collection *collection = (Collection *)malloc(sizeof(Collection));
  strcpy(collection->collectionName, name);
  collection->documents = NULL;
  collection->documentCount = 0;
  return collection;
}

void addDocumentToCollection(Collection *collection, Document *doc)
{
  collection->documents = (Document *)realloc(collection->documents, (collection->documentCount + 1) * sizeof(Document));
  collection->documents[collection->documentCount] = *doc;
  collection->documentCount++;
}

Document *findDocumentById(Collection *collection, const char *documentId)
{
  for (size_t i = 0; i < collection->documentCount; i++)
  {
    if (strcmp(collection->documents[i].documentId, documentId) == 0)
    {
      return &collection->documents[i];
    }
  }
  return NULL;
}

Document **findDocumentsByField(Collection *collection, const char *fieldKey, void *fieldValue, ValueType type, size_t *resultCount)
{
  Document **results = NULL;
  *resultCount = 0;

  for (size_t i = 0; i < collection->documentCount; i++)
  {
    Document *doc = &collection->documents[i];
    for (size_t j = 0; j < doc->fieldCount; j++)
    {
      Field *field = &doc->fields[j];
      if (strcmp(field->key, fieldKey) == 0 && field->type == type)
      {
        int match = 0;
        if (type == TYPE_INT && field->value.intValue == *(int *)fieldValue)
        {
          match = 1;
        }
        else if (type == TYPE_STRING && strcmp(field->value.stringValue, (char *)fieldValue) == 0)
        {
          match = 1;
        }
        if (match)
        {
          results = realloc(results, (*resultCount + 1) * sizeof(Document *));
          results[*resultCount] = doc;
          (*resultCount)++;
          break;
        }
      }
    }
  }

  return results;
}

void deleteDocumentFromCollection(Collection *collection, const char *documentId)
{
  for (size_t i = 0; i < collection->documentCount; i++)
  {
    if (strcmp(collection->documents[i].documentId, documentId) == 0)
    {
      freeDocument(&collection->documents[i]);
      for (size_t j = i; j < collection->documentCount - 1; j++)
      {
        collection->documents[j] = collection->documents[j + 1];
      }
      collection->documentCount--;
      collection->documents = (Document *)realloc(collection->documents, collection->documentCount * sizeof(Document));
      return;
    }
  }
}

void printCollection(const Collection *collection)
{
  printf("Collection: %s\n", collection->collectionName);
  for (size_t i = 0; i < collection->documentCount; i++)
  {
    printDocument(&collection->documents[i]);
  }
}

void freeCollection(Collection *collection)
{
  for (size_t i = 0; i < collection->documentCount; i++)
  {
    freeDocument(&collection->documents[i]);
  }
  free(collection->documents);
  free(collection);
}
