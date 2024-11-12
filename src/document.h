#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "types.h"

Document *createDocument(const char *documentId);

void addField(Document *doc, const char *key, ValueType type, void *value);
void updateField(Document *doc, const char *key, ValueType type, void *value);
void deleteField(Document *doc, const char *key);
void printDocument(const Document *doc);
void freeDocument(Document *doc);

#endif // DOCUMENT_H
