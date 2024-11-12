#ifndef COLLECTION_H
#define COLLECTION_H

#include "types.h"

// Collection functions
Collection *createCollection(const char *name);

Document *findDocumentById(Collection *collection, const char *documentId);
Document **findDocumentsByField(Collection *collection, const char *fieldKey, void *fieldValue, ValueType type, size_t *resultCount);

void addDocumentToCollection(Collection *collection, Document *doc);
void deleteDocumentFromCollection(Collection *collection, const char *documentId);
void printCollection(const Collection *collection);
void freeCollection(Collection *collection);

#endif // COLLECTION_H
