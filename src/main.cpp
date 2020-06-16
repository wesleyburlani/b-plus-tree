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
#define DATAFILEPATH "normalized-data.csv"

using namespace std;

int main() {

  //-----------------Definitions-----------------
  string _MESSAGE = "Welcome!\n";
  cout << _MESSAGE;

  Node *tree = NULL;
  int n_ATTR;
  int n_CHAR;
  int n_ORDER;
  string searched;
  string toRemove;
  CSVDatabase _Table;
  CSVDatabase _NoGroup;
  stringstream ss;
  char key[900];

  DataCollect(n_ATTR, n_CHAR, n_ORDER);

  bool success =
      readCSV(DATAFILEPATH, _Table, IndexOfId, n_ATTR, n_CHAR);
  if (!success)
    return 1;

  sort(_Table.begin(), _Table.end(), comparator(n_ATTR));

  BulkLoadingInsert(tree, _Table, n_ORDER, n_ATTR);

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
      RemoveNode(toRemove, tree, _Table, n_ORDER, n_ATTR);
      break;
    case 9:
      exit = true;
      break;
    default:
      cout << "Oops...this option not exists.\n\n";
    }
  }
}
