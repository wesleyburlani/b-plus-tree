#include "../csv/CSV.cpp"
#include "../src/h/B.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define IndexOfId 0
#define DEBUGGER 0
// File used to mount B+ tree using bulk loading operation
#define DATAFILEPATH "normalized-data.csv"

using namespace std;

int main() {

  //-----------------Definitions-----------------
  string _MESSAGE = "Welcome!\n";
  cout << _MESSAGE;

  Node *tree = NULL;
  int numberOfAttributes;
  int numberOfCharsToIndex;
  int treeOrder;
  string searched;
  string toRemove;
  CSVDatabase table;
  CSVDatabase _NoGroup;
  stringstream stringStream;
  char key[900];

  stdInDataCollect(numberOfAttributes, numberOfCharsToIndex, treeOrder);

  bool success =
      readCSV(DATAFILEPATH, table, IndexOfId, numberOfAttributes, numberOfCharsToIndex);
  if (!success)
    return 1;

  sort(table.begin(), table.end(), comparator(numberOfAttributes));

  bulkLoadingInsert(tree, table, treeOrder, numberOfAttributes);

  if (DEBUGGER)
    printTree(tree, 0);

  cout << "\nOk.. The tree has been created. What you want to do ?\n\n";

  bool exit = false;
  while (!exit) {

    int option;
    cout << "1.Print the tree\n";
    cout << "2.Search a id by Key\n";
    cout << "3.Remove a node\n";
    cout << "9.Exit Program\n";
    cout << "\nEnter with option: ";
    scanf("%d", &option);

    switch (option) {
    case 1:
      printTree(tree, 0);
      cout << "Ok.. This is tree current tree.. and now?\n\n";
      break;
    case 2:
      cout << "Alright!\nEnter the key value: ";
      getline(cin, searched);
      getline(cin, searched);
      success = searchPathByKey(tree, searched, searched);
      if (success)
        cout << "The search returns this values:...\n" << searched << "\n\n";
      else
        cout << "Oops...Value not exists in tree\n\n";
      break;
    case 3:
      cout << "Enter with the key id: ";
      cin >> toRemove;
      removeNode(toRemove, tree, table, treeOrder, numberOfAttributes);
      break;
    case 9:
      exit = true;
      break;
    default:
      cout << "Oops...this option not exists.\n\n";
    }
  }
}
