#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "collection_manager.h"
#include "cJSON.h"

void saveCollectionManagerToFile(CollectionManager *manager, const char *filename);
CollectionManager *loadCollectionManagerFromFile(const char *filename);

cJSON *collectionToJSON(Collection *collection);
Collection *jsonToCollection(cJSON *jsonCollection);

#endif // JSON_UTILS_H
