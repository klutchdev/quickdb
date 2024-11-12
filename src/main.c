#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "json_utils.h"

#define SAVE_FILE "quickdb_data.json"

void clearTerminal()
{
#ifdef _WIN32
  system("cls"); // Windows
#else
  system("clear"); // Linux/macOS
#endif
}

int main()
{
  CollectionManager *manager = loadCollectionManagerFromFile(SAVE_FILE);
  int choice;

  clearTerminal();

  while (1)
  {
    showMenu();
    scanf("%d", &choice);
    getchar();

    switch (choice)
    {
    case 1:
      showCollections(manager);
      break;

    case 2:
      createNewCollection(manager);
      break;

    case 3:
      removeCollection(manager);
      break;

    case 4:
      addDocument(manager);
      break;

    case 5:
      viewDocuments(manager);
      break;

    case 6:
      searchDocuments(manager);
      break;

    case 7:
      enterPowerUserMode(manager);
      break;

    case 8:
      printf("Saving data and exiting...\n");
      saveCollectionManagerToFile(manager, SAVE_FILE);
      freeCollectionManager(manager);
      return 0;

    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }
  }
}
