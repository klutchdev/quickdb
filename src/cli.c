#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cli.h"
#include "document.h"
#include "collection.h"
#include "collection_manager.h"
#include "json_utils.h"

// Prototype for parseCommand to avoid implicit declaration warning
void parseCommand(CollectionManager *manager, char *command);

void showMenu()
{
    printf("\nQuickDB CLI\n");
    printf("  1. Show Collections\n");
    printf("  2. Add Collection\n");
    printf("  3. Delete Collection\n");
    printf("  4. Add Document to Collection\n");
    printf("  5. View Documents in Collection\n");
    printf("  6. Search Documents by Field\n");
    printf("  7. Enter Power User Mode\n");
    printf("  8. Exit\n");
    printf("Enter selection: \n");
}

void showCollections(CollectionManager *manager)
{
    if (manager->collectionCount == 0)
    {
        printf("No collections found.\n");
    }
    else
    {
        printf("Collections:\n");
        for (size_t i = 0; i < manager->collectionCount; i++)
        {
            printf(" - %s\n", manager->collections[i].collectionName);
        }
    }
}

void createNewCollection(CollectionManager *manager)
{
    char name[50];
    printf("Enter collection name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    Collection *collection = createCollection(name);
    addCollection(manager, collection);
    printf("Collection '%s' added.\n", name);
}

void removeCollection(CollectionManager *manager)
{
    char name[50];
    printf("Enter collection name to delete: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    deleteCollectionFromManager(manager, name);
    printf("Collection '%s' deleted.\n", name);
}

void addDocument(CollectionManager *manager)
{
    char collectionName[50];
    printf("Enter collection name to add document to: ");
    fgets(collectionName, sizeof(collectionName), stdin);
    collectionName[strcspn(collectionName, "\n")] = '\0';

    Collection *collection = getCollection(manager, collectionName);
    if (!collection)
    {
        printf("Collection not found.\n");
        return;
    }

    char docId[50];
    printf("Enter document ID: ");
    fgets(docId, sizeof(docId), stdin);
    docId[strcspn(docId, "\n")] = '\0';

    Document *doc = createDocument(docId);

    int fieldCount;
    printf("Enter number of fields: ");
    scanf("%d", &fieldCount);
    getchar();

    for (int i = 0; i < fieldCount; i++)
    {
        char fieldKey[50];
        int fieldType;
        printf("Enter field %d key: ", i + 1);
        fgets(fieldKey, sizeof(fieldKey), stdin);
        fieldKey[strcspn(fieldKey, "\n")] = '\0';

        printf("Enter field type (1 for int, 2 for string): ");
        scanf("%d", &fieldType);
        getchar();

        if (fieldType == 1)
        {
            int intValue;
            printf("Enter integer value: ");
            scanf("%d", &intValue);
            getchar();
            addField(doc, fieldKey, TYPE_INT, &intValue);
        }
        else if (fieldType == 2)
        {
            char strValue[100];
            printf("Enter string value: ");
            fgets(strValue, sizeof(strValue), stdin);
            strValue[strcspn(strValue, "\n")] = '\0';
            addField(doc, fieldKey, TYPE_STRING, strValue);
        }
        else
        {
            printf("Invalid field type.\n");
        }
    }

    addDocumentToCollection(collection, doc);
    printf("Document '%s' added to collection '%s'.\n", docId, collectionName);
}

void viewDocuments(CollectionManager *manager)
{
    char collectionName[50];
    printf("Enter collection name to view: ");
    fgets(collectionName, sizeof(collectionName), stdin);
    collectionName[strcspn(collectionName, "\n")] = '\0';

    Collection *collection = getCollection(manager, collectionName);
    if (!collection)
    {
        printf("Collection not found.\n");
        return;
    }

    printCollection(collection);
}

void searchDocuments(CollectionManager *manager)
{
    char collectionName[50];
    printf("Enter collection name to search in: ");
    fgets(collectionName, sizeof(collectionName), stdin);
    collectionName[strcspn(collectionName, "\n")] = '\0';

    Collection *collection = getCollection(manager, collectionName);
    if (!collection)
    {
        printf("Collection not found.\n");
        return;
    }

    char fieldKey[50];
    int fieldType;
    printf("Enter field key to search by: ");
    fgets(fieldKey, sizeof(fieldKey), stdin);
    fieldKey[strcspn(fieldKey, "\n")] = '\0';

    printf("Enter field type (1 for int, 2 for string): ");
    scanf("%d", &fieldType);
    getchar();

    size_t resultCount = 0;
    if (fieldType == 1)
    {
        int intValue;
        printf("Enter integer value to search for: ");
        scanf("%d", &intValue);
        getchar();
        Document **results = findDocumentsByField(collection, fieldKey, &intValue, TYPE_INT, &resultCount);
        printf("Documents found:\n");
        for (size_t i = 0; i < resultCount; i++)
        {
            printDocument(results[i]);
        }
        free(results);
    }
    else if (fieldType == 2)
    {
        char strValue[100];
        printf("Enter string value to search for: ");
        fgets(strValue, sizeof(strValue), stdin);
        strValue[strcspn(strValue, "\n")] = '\0';
        Document **results = findDocumentsByField(collection, fieldKey, strValue, TYPE_STRING, &resultCount);
        printf("Documents found:\n");
        for (size_t i = 0; i < resultCount; i++)
        {
            printDocument(results[i]);
        }
        free(results);
    }
    else
    {
        printf("Invalid field type.\n");
    }
}

void enterPowerUserMode(CollectionManager *manager)
{
    char command[256];

    printf("Entering Power User Mode. Type 'help' for a list of commands or 'exit' to leave.\n");

    while (1)
    {
        printf(">> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            printf("Exiting Power User Mode.\n");
            break;
        }

        parseCommand(manager, command);
    }
}

void showHelpMenu()
{
    printf("\nPower User Mode Commands:\n");
    printf("  help                                - Show this help menu.\n");
    printf("  show collections                    - Display all collections.\n");
    printf("  add collection <name>               - Add a new collection with the given name.\n");
    printf("  delete collection <name>            - Delete the specified collection.\n");
    printf("  add document <collection> <doc_id> <key:value> ... - Add a document to a collection.\n");
    printf("  delete document <collection> <doc_id> - Delete a document from a collection.\n");
    printf("  exit                                - Exit Power User Mode.\n");
    printf("\nExamples:\n");
    printf("  add collection Pets\n");
    printf("  add document Pets doc1 name:Whiskers age:3\n");
    printf("  show collections\n");
    printf("  delete document Pets doc1\n");
}

void parseCommand(CollectionManager *manager, char *command)
{
    char *token = strtok(command, " ");

    if (strcmp(token, "help") == 0)
    {
        showHelpMenu();
    }
    else if (strcmp(token, "add") == 0)
    {
        token = strtok(NULL, " ");
        if (strcmp(token, "document") == 0)
        {
            char *collectionName = strtok(NULL, " ");
            char *docId = strtok(NULL, " ");
            Document *doc = createDocument(docId);

            char *field;
            while ((field = strtok(NULL, " ")))
            {
                char *key = strtok(field, ":");
                char *value = strtok(NULL, ":");

                if (key && value)
                {
                    if (isdigit(value[0]))
                    {
                        int intValue = atoi(value);
                        addField(doc, key, TYPE_INT, &intValue);
                    }
                    else
                    {
                        addField(doc, key, TYPE_STRING, value);
                    }
                }
            }

            Collection *collection = getCollection(manager, collectionName);
            if (collection)
            {
                addDocumentToCollection(collection, doc);
                printf("Document '%s' added to collection '%s'.\n", docId, collectionName);
            }
            else
            {
                printf("Collection '%s' not found.\n", collectionName);
                freeDocument(doc);
            }
        }
    }
}