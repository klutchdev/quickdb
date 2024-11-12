#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "collection_manager.h"

CollectionManager *createCollectionManager()
{
  CollectionManager *manager = (CollectionManager *)malloc(sizeof(CollectionManager));
  manager->collections = NULL;
  manager->collectionCount = 0;
  return manager;
}

void addCollection(CollectionManager *manager, Collection *collection)
{
  manager->collections = (Collection *)realloc(manager->collections, (manager->collectionCount + 1) * sizeof(Collection));
  manager->collections[manager->collectionCount] = *collection;
  manager->collectionCount++;
}

Collection *getCollection(CollectionManager *manager, const char *name)
{
  for (size_t i = 0; i < manager->collectionCount; i++)
  {
    if (strcmp(manager->collections[i].collectionName, name) == 0)
    {
      return &manager->collections[i];
    }
  }
  return NULL;
}

void deleteCollectionFromManager(CollectionManager *manager, const char *collectionName)
{
  printf("Attempting to delete collection: %s\n", collectionName);
  for (size_t i = 0; i < manager->collectionCount; i++)
  {
    if (strcmp(manager->collections[i].collectionName, collectionName) == 0)
    {
      // Free the collection and all its documents
      freeCollection(&manager->collections[i]);

      // Shift the remaining collections
      for (size_t j = i; j < manager->collectionCount - 1; j++)
      {
        manager->collections[j] = manager->collections[j + 1];
      }

      manager->collectionCount--;
      manager->collections = realloc(manager->collections, manager->collectionCount * sizeof(Collection));

      if (manager->collectionCount > 0 && manager->collections == NULL)
      {
        printf("Error reallocating memory for collections.\n");
        exit(1);
      }
      return;
    }
  }
  printf("Collection '%s' not found.\n", collectionName);
}

void freeCollectionManager(CollectionManager *manager)
{
  printf("Freeing collections");
  for (size_t i = 0; i < manager->collectionCount; i++)
  {
    freeCollection(&manager->collections[i]);
  }
  free(manager->collections);
  free(manager);
}
