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

// Flag to turn on debug logs
#define DEBUGGER 0
// Column index which input file has id data
#define IDINDEX 0
// File used to mount B+ tree using bulk loading operation
#define DATAFILEPATH "normalized-data.csv"
// Alias to menu options
#define OPT_PRINTTREE 1
#define OPT_GETIDBYKEY 2
#define OPT_REMOVENODE 3
#define OPT_EXIT 9

using namespace std;

void printMenu();
void optionPrintTree(Node *tree);
void optionSearchPathByKey(Node *tree);
void optionRemoveNode(Node *tree, CSVDatabase table, int treeOrder,
                      int numberOfAttributes);

int main() {
  cout << "Welcome!\n";
  //-----------------Definitions-----------------
  Node *tree = NULL;
  int numberOfAttributes;
  int numberOfCharsToIndex;
  int treeOrder;
  CSVDatabase table;

  stdInDataCollect(numberOfAttributes, numberOfCharsToIndex, treeOrder);

  bool success = readCSV(DATAFILEPATH, table, IDINDEX, numberOfAttributes,
                         numberOfCharsToIndex);
  if (!success) {
    return 1;
  }

  sort(table.begin(), table.end(), comparator(numberOfAttributes));

  bulkLoadingInsert(tree, table, treeOrder, numberOfAttributes);

  if (DEBUGGER) {
    printTree(tree, 0);
  }

  cout << "\nOk.. The tree has been created. What you want to do ?\n\n";

  int option;
  printMenu();
  while (scanf("%d", &option)) {

    if (option == OPT_PRINTTREE) {
      optionPrintTree(tree);
    } else if (option == OPT_GETIDBYKEY) {
      optionSearchPathByKey(tree);
    } else if (option == OPT_REMOVENODE) {
      optionRemoveNode(tree, table, treeOrder, numberOfAttributes);
    } else if (option == OPT_EXIT) {
      return 0;
    } else {
      cout << "Oops...this option not exists.\n\n";
    }
    printMenu();
  }
}

void printMenu() {
  cout << "1.Print the tree\n";
  cout << "2.Search an id by Key\n";
  cout << "3.Remove a node\n";
  cout << "9.Exit Program\n";
  cout << "\nEnter with option: ";
}

void optionPrintTree(Node *tree) {
  printTree(tree, 0);
  cout << "Ok.. This is the current tree.. and now?\n\n";
}

void optionSearchPathByKey(Node *tree) {
  string input;
  cout << "Alright!\nEnter the key value: ";
  getline(cin, input);
  getline(cin, input);
  if (searchPathByKey(tree, input, input)) {
    cout << "The search returns this values:...\n" << input << "\n\n";
    return;
  }

  cout << "Oops...Value not exists in tree\n\n";
}

void optionRemoveNode(Node *tree, CSVDatabase table, int treeOrder,
                      int numberOfAttributes) {
  string input;
  cout << "Enter with the key id: ";
  cin >> input;
  removeNode(input, tree, table, treeOrder, numberOfAttributes);
}
