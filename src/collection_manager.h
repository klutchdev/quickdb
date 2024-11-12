#ifndef COLLECTION_MANAGER_H
#define COLLECTION_MANAGER_H

#include "collection.h"

CollectionManager *createCollectionManager();

Collection *getCollection(CollectionManager *manager, const char *name);

void addCollection(CollectionManager *manager, Collection *collection);
void deleteCollectionFromManager(CollectionManager *manager, const char *name);
void freeCollectionManager(CollectionManager *manager);

#endif // COLLECTION_MANAGER_H
