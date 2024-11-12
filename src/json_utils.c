#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_utils.h"
#include "cJSON.h"
#include "document.h"
#include "collection.h"

void saveCollectionManagerToFile(CollectionManager *manager, const char *filename)
{
    cJSON *jsonManager = cJSON_CreateObject();
    cJSON *collectionsArray = cJSON_CreateArray();

    for (size_t i = 0; i < manager->collectionCount; i++)
    {
        cJSON *jsonCollection = collectionToJSON(&manager->collections[i]);
        cJSON_AddItemToArray(collectionsArray, jsonCollection);
    }

    cJSON_AddItemToObject(jsonManager, "collections", collectionsArray);

    char *jsonString = cJSON_Print(jsonManager);
    FILE *file = fopen(filename, "w");
    if (file)
    {
        fputs(jsonString, file);
        fclose(file);
        printf("Data saved to %s\n", filename);
    }
    else
    {
        printf("Failed to save data to %s\n", filename);
    }

    cJSON_Delete(jsonManager);
    free(jsonString);
}

CollectionManager *loadCollectionManagerFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("No saved data found.\n");
        return createCollectionManager(); // Return empty manager if no file found
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonString = (char *)malloc(fileSize + 1);
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';
    fclose(file);

    cJSON *jsonManager = cJSON_Parse(jsonString);
    free(jsonString);

    CollectionManager *manager = createCollectionManager();
    cJSON *collectionsArray = cJSON_GetObjectItem(jsonManager, "collections");
    cJSON *jsonCollection = NULL;
    cJSON_ArrayForEach(jsonCollection, collectionsArray)
    {
        Collection *collection = jsonToCollection(jsonCollection);
        addCollection(manager, collection);
    }

    cJSON_Delete(jsonManager);
    return manager;
}

cJSON *collectionToJSON(Collection *collection)
{
    cJSON *jsonCollection = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonCollection, "collectionName", collection->collectionName);

    cJSON *documentsArray = cJSON_CreateArray();
    for (size_t i = 0; i < collection->documentCount; i++)
    {
        cJSON *jsonDoc = cJSON_CreateObject();
        Document *doc = &collection->documents[i];

        cJSON_AddStringToObject(jsonDoc, "documentId", doc->documentId);

        cJSON *fieldsArray = cJSON_CreateArray();
        for (size_t j = 0; j < doc->fieldCount; j++)
        {
            Field *field = &doc->fields[j];
            cJSON *jsonField = cJSON_CreateObject();

            cJSON_AddStringToObject(jsonField, "key", field->key);
            cJSON_AddNumberToObject(jsonField, "type", field->type);
            if (field->type == TYPE_INT)
            {
                cJSON_AddNumberToObject(jsonField, "value", field->value.intValue);
            }
            else if (field->type == TYPE_STRING)
            {
                cJSON_AddStringToObject(jsonField, "value", field->value.stringValue);
            }
            cJSON_AddItemToArray(fieldsArray, jsonField);
        }
        cJSON_AddItemToObject(jsonDoc, "fields", fieldsArray);
        cJSON_AddItemToArray(documentsArray, jsonDoc);
    }

    cJSON_AddItemToObject(jsonCollection, "documents", documentsArray);
    return jsonCollection;
}

Collection *jsonToCollection(cJSON *jsonCollection)
{
    Collection *collection = createCollection(cJSON_GetObjectItem(jsonCollection, "collectionName")->valuestring);

    cJSON *documentsArray = cJSON_GetObjectItem(jsonCollection, "documents");
    cJSON *jsonDoc = NULL;

    cJSON_ArrayForEach(jsonDoc, documentsArray)
    {
        Document *doc = createDocument(cJSON_GetObjectItem(jsonDoc, "documentId")->valuestring);

        cJSON *fieldsArray = cJSON_GetObjectItem(jsonDoc, "fields");
        cJSON *jsonField = NULL;
        cJSON_ArrayForEach(jsonField, fieldsArray)
        {
            char *key = cJSON_GetObjectItem(jsonField, "key")->valuestring;
            int type = cJSON_GetObjectItem(jsonField, "type")->valueint;

            if (type == TYPE_INT)
            {
                int intValue = cJSON_GetObjectItem(jsonField, "value")->valueint;
                addField(doc, key, TYPE_INT, &intValue);
            }
            else if (type == TYPE_STRING)
            {
                char *strValue = cJSON_GetObjectItem(jsonField, "value")->valuestring;
                addField(doc, key, TYPE_STRING, strValue);
            }
        }
        addDocumentToCollection(collection, doc);
    }

    return collection;
}