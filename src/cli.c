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
    printf("---------------------------------\n");

    printf("show collections\n");
    printf("   - Lists all collections in the database.\n");

    printf("show documents <collection>\n");
    printf("   - Lists all documents within a specified collection.\n");

    printf("add document <collection> <docId> <field1:value1> <field2:value2> ...\n");
    printf("   - Adds a new document to the specified collection with specified fields.\n");

    printf("update document <collection> <docId> <field1:value1> <field2:value2> ...\n");
    printf("   - Updates an existing document in the specified collection with new field values.\n");

    printf("delete document <collection> <docId>\n");
    printf("   - Deletes a document from the specified collection by document ID.\n");

    printf("add field <collection> <docId> <field:value>\n");
    printf("   - Adds a new field to an existing document in the specified collection.\n");

    printf("update field <collection> <docId> <field:value>\n");
    printf("   - Updates a specific field in an existing document.\n");

    printf("delete field <collection> <docId> <field>\n");
    printf("   - Deletes a specific field from a document.\n");

    printf("delete collection <collection>\n");
    printf("   - Deletes an entire collection along with all documents it contains.\n");

    printf("search <collection> <field1:value1> <field2:value2> ...\n");
    printf("   - Searches for documents in a collection matching all specified field criteria.\n");

    printf("sort <collection> by <field>\n");
    printf("   - Sorts and displays documents in a collection based on the specified field.\n");

    printf("help\n");
    printf("   - Displays this help menu with all available commands.\n");

    printf("exit\n");
    printf("   - Exits Power User Mode.\n");
}

void parseCommand(CollectionManager *manager, char *command)
{
    char *token = strtok(command, " ");

    if (token == NULL)
    {
        printf("Error: Command cannot be empty. Type 'help' for a list of commands.\n");
        return;
    }

    if (strcmp(token, "help") == 0)
    {
        showHelpMenu();
    }

    // Show Collections Command
    else if (strcmp(token, "show") == 0 && strcmp(strtok(NULL, " "), "collections") == 0)
    {
        printf("Collections:\n");
        for (size_t i = 0; i < manager->collectionCount; i++)
        {
            printf(" - %s\n", manager->collections[i].collectionName);
        }
    }

    // Show Documents in Collection Command
    else if (strcmp(token, "show") == 0 && strcmp(strtok(NULL, " "), "documents") == 0)
    {
        char *collectionName = strtok(NULL, " ");
        if (collectionName == NULL)
        {
            printf("Error: Collection name is required for 'show documents'.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        if (!collection)
        {
            printf("Collection '%s' not found.\n", collectionName);
            return;
        }

        printf("Documents in collection '%s':\n", collectionName);
        printCollection(collection);
    }

    // Add Document Command
    else if (strcmp(token, "add") == 0)
    {
        char *subCommand = strtok(NULL, " ");
        if (subCommand == NULL || strcmp(subCommand, "document") != 0)
        {
            printf("Error: Invalid syntax for 'add'. Expected 'add document <collection> <docId> <field:value> ...'.\n");
            return;
        }

        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        if (collectionName == NULL || docId == NULL)
        {
            printf("Error: Collection name and document ID are required.\n");
            return;
        }

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
            else
            {
                printf("Error: Malformed field '%s'. Expected format '<key>:<value>'.\n", field);
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

    // Update Document Command
    else if (strcmp(token, "update") == 0)
    {
        char *subCommand = strtok(NULL, " ");
        if (subCommand == NULL || strcmp(subCommand, "document") != 0)
        {
            printf("Error: Invalid syntax for 'update'. Expected 'update document <collection> <docId> <field:value> ...'.\n");
            return;
        }

        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        if (collectionName == NULL || docId == NULL)
        {
            printf("Error: Collection name and document ID are required.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        if (!collection)
        {
            printf("Collection '%s' not found.\n", collectionName);
            return;
        }

        Document *doc = findDocumentById(collection, docId);
        if (!doc)
        {
            printf("Document '%s' not found in collection '%s'.\n", docId, collectionName);
            return;
        }

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
                    updateField(doc, key, TYPE_INT, &intValue);
                }
                else
                {
                    updateField(doc, key, TYPE_STRING, value);
                }
            }
            else
            {
                printf("Error: Malformed field '%s'. Expected format '<key>:<value>'.\n", field);
            }
        }
        printf("Document '%s' in collection '%s' updated.\n", docId, collectionName);
    }

    // Delete Document Command
    else if (strcmp(token, "delete") == 0)
    {
        char *subCommand = strtok(NULL, " ");
        if (subCommand == NULL || strcmp(subCommand, "document") != 0)
        {
            printf("Error: Invalid syntax for 'delete'. Expected 'delete document <collection> <docId>'.\n");
            return;
        }

        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        if (collectionName == NULL || docId == NULL)
        {
            printf("Error: Collection name and document ID are required.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        if (!collection)
        {
            printf("Collection '%s' not found.\n", collectionName);
            return;
        }

        deleteDocumentFromCollection(collection, docId);
        printf("Document '%s' deleted from collection '%s'.\n", docId, collectionName);
    }

    // Add Field Command
    else if (strcmp(token, "add") == 0 && strcmp(strtok(NULL, " "), "field") == 0)
    {
        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        char *field = strtok(NULL, " ");

        if (collectionName == NULL || docId == NULL || field == NULL)
        {
            printf("Error: Expected 'add field <collection> <docId> <field:value>'.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        Document *doc = findDocumentById(collection, docId);
        if (collection && doc)
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
            printf("Field '%s' added to document '%s' in collection '%s'.\n", key, docId, collectionName);
        }
        else
        {
            printf("Collection or document not found.\n");
        }
    }

    // Update Field Command
    else if (strcmp(token, "update") == 0 && strcmp(strtok(NULL, " "), "field") == 0)
    {
        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        char *field = strtok(NULL, " ");

        if (collectionName == NULL || docId == NULL || field == NULL)
        {
            printf("Error: Expected 'update field <collection> <docId> <field:value>'.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        Document *doc = findDocumentById(collection, docId);
        if (collection && doc)
        {
            char *key = strtok(field, ":");
            char *value = strtok(NULL, ":");

            if (key && value)
            {
                if (isdigit(value[0]))
                {
                    int intValue = atoi(value);
                    updateField(doc, key, TYPE_INT, &intValue);
                }
                else
                {
                    updateField(doc, key, TYPE_STRING, value);
                }
            }
            printf("Field '%s' updated in document '%s' in collection '%s'.\n", key, docId, collectionName);
        }
        else
        {
            printf("Collection or document not found.\n");
        }
    }

    // Delete Field Command
    else if (strcmp(token, "delete") == 0 && strcmp(strtok(NULL, " "), "field") == 0)
    {
        char *collectionName = strtok(NULL, " ");
        char *docId = strtok(NULL, " ");
        char *field = strtok(NULL, " ");

        if (collectionName == NULL || docId == NULL || field == NULL)
        {
            printf("Error: Expected 'update field <collection> <docId> <field:value>'.\n");
            return;
        }

        Collection *collection = getCollection(manager, collectionName);
        Document *doc = findDocumentById(collection, docId);
        if (collection && doc)
        {
            char *key = strtok(field, ":");
            char *value = strtok(NULL, ":");

            if (key && value)
            {
                if (isdigit(value[0]))
                {
                    int intValue = atoi(value);
                    deleteField(doc, key);
                }
                else
                {
                    deleteField(doc, key);
                }
            }
            printf("Field '%s' updated in document '%s' in collection '%s'.\n", key, docId, collectionName);
        }
        else
        {
            printf("Collection or document not found.\n");
        }
    }

    // Delete Collection Command
    else if (strcmp(token, "delete") == 0 && strcmp(strtok(NULL, " "), "collection") == 0)
    {
        char *collectionName = strtok(NULL, " ");
        if (collectionName == NULL)
        {
            printf("Error: Collection name is required.\n");
            return;
        }

        deleteCollectionFromManager(manager, collectionName);
        printf("Collection '%s' deleted.\n", collectionName);
    }
    else
    {
        printf("Error: Unrecognized command '%s'. Type 'help' for a list of valid commands.\n", token);
    }
}
