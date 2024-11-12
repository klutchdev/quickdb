#ifndef CLI_H
#define CLI_H

#include "collection_manager.h"

// CLI function declarations
void showCollections(CollectionManager *manager);
void createNewCollection(CollectionManager *manager);
void removeCollection(CollectionManager *manager);
void addDocument(CollectionManager *manager);
void viewDocuments(CollectionManager *manager);
void searchDocuments(CollectionManager *manager);
void showMenu();

// Power user shell mode
void enterPowerUserMode(CollectionManager *manager);

#endif // CLI_H
