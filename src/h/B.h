#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#define _IDCOLUMN 0

using namespace std;

typedef string KeyType;
typedef vector<string> CSVRow;
typedef vector<CSVRow> CSVDatabase;

class Node {

public:
  vector<KeyType> keys;
  vector<Node *> pointers;
  vector<KeyType> paths;
  Node *nextList;
  Node *lastList;
  bool isLeaf;
  int numberOfKeys;
  Node *parent;

  Node(int order, bool leaf) {
    keys.resize(order - 1);
    numberOfKeys = 0;
    nextList = NULL;
    lastList = NULL;
    isLeaf = leaf ? true : false;
    keys.clear();
    numberOfKeys = 0;
    if (isLeaf) {
      paths.resize(order - 1);
      paths.clear();
    } else {
      pointers.resize(order);
      pointers.clear();
    }
  }

  Node(int order) {
    keys.resize(order - 1);
    pointers.resize(order);
    nextList = NULL;
    lastList = NULL;
    numberOfKeys = 0;
    isLeaf = false;
    keys.clear();
    pointers.clear();
  }

  void sortKeys() {
    sort(this->keys.begin(), this->keys.begin() + this->numberOfKeys);
  }
};

void bppInsert(Node *&tree, KeyType value, int treeOrder);
Node *getNodeToAdd(Node *node, KeyType value);
void splitVector(Node *&base, KeyType &upper, Node *&left, Node *&right);
void printTree(Node *&tree, int tabs);
void upFirsts(vector<string> &keys, vector<Node *> &pointers, int treeOrder, Node *&tree);
void bulkLoadingInsert(Node *&tree, CSVDatabase &table, int treeOrder, int column);
bool searchPathByKey(Node *&tree, KeyType key, KeyType &path);
void removeNode(KeyType id, Node *&tree, CSVDatabase &table, int treeOrder, int column);
void removeNodeFromVector(Node *&node, KeyType searchKey, int index, int treeOrder);

#include "../hpp/B.cpp"
