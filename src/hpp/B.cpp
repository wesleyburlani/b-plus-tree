#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "../functions.cpp"

// Flag to turn on debug logs
#define DEBUGGER 0

using namespace std;

/*
 *  Print tree starting by root
 *  Insert a tab to indicate a new level
 */
void printTree(Node *&node, int tabs) {
  // Verify if current level is leaf to stop print
  if (node->isLeaf) {
    for (int i = 0; i < node->numberOfKeys; i++) {
      int currentTabs = tabs;
      while (currentTabs--) {
        cout << "\t";
      }
      cout << node->keys[i];
      if (DEBUGGER) {
        cout << "->" << node->paths[i];
      }
      cout << "\n";
    }
    return;
  }

  for (int i = 0; i < node->numberOfKeys; i++) {
    printTree(node->pointers[i], tabs + 1);
    int currentTabs = tabs;
    while (currentTabs--) {
      cout << "\t";
    }
    cout << node->keys[i] << "\n";
  }

  printTree(node->pointers[node->numberOfKeys], tabs + 1);
}

void splitVector(Node *&base, KeyType &upper, Node *&left, Node *&right) {
  upper = base->keys[(base->keys.size() - 1) / 2];

  size_t i = 0;
  while (i < base->keys.size() / 2) {
    left->keys[left->numberOfKeys] = base->keys[i];
    left->pointers[left->numberOfKeys++] = base->pointers[i++];
  }

  left->pointers[left->numberOfKeys] = base->pointers[i];

  while (i < base->keys.size()) {
    right->keys[right->numberOfKeys] = base->keys[i];
    right->pointers[right->numberOfKeys++] = right->pointers[i++];
  }

  right->pointers[right->numberOfKeys] = right->pointers[i];
}

Node *getNodeToAdd(Node *node, KeyType value) {
  Node *copyPointer = node;
  if (copyPointer->isLeaf) {
    return copyPointer;
  }

  int countPosition = 0;
  while (value > copyPointer->keys[countPosition] &&
         countPosition < node->numberOfKeys) {
    countPosition++;
  }
  return getNodeToAdd(copyPointer->pointers[countPosition], value);
}

/*
 *	Make a BulkLoading insert in tree
 *  creating a minimum tree,
 *  this function makes a leaf nodes and call other function
 *  'UpFisrts' that create others levels of tree.
 */
void bulkLoadingInsert(Node *&tree, CSVDatabase &table, int treeOrder,
                       int column) {

  size_t count = 0;
  vector<Node *> nodes;
  vector<string> uppperKeys;
  uppperKeys.clear();
  vector<Node *> upperPointers;
  upperPointers.clear();

  while (count < table.size()) {
    Node *temporaryNode = new Node(treeOrder, true);
    temporaryNode->paths.clear();
    // insert elements while node doesn't have the miminum number of elements
    try {
      while (temporaryNode->numberOfKeys < (treeOrder - 1) / 2 &&
             count < table.size()) {

        if (temporaryNode->numberOfKeys > 0 &&
            !table[count][column].compare(
                temporaryNode->keys[temporaryNode->numberOfKeys - 1])) {
          temporaryNode->paths[temporaryNode->paths.size() - 1] +=
              "-" + table[count][0];
        } else {
          temporaryNode->keys.push_back(table[count][column]);
          temporaryNode->paths.push_back(table[count][0]);
          temporaryNode->numberOfKeys++;
        }
        count++;
      }

      if (nodes.size() > 0) {
        nodes[nodes.size() - 1]->nextList = temporaryNode;
        temporaryNode->lastList = nodes[nodes.size() - 1];
      }

      nodes.push_back(temporaryNode);

      while (count < table.size() && nodes.size() > 0 &&
             !table[count][column].compare(
                 nodes[nodes.size() - 1]
                     ->keys[nodes[nodes.size() - 1]->numberOfKeys - 1])) {
        nodes[nodes.size() - 1]
            ->paths[nodes[nodes.size() - 1]->paths.size() - 1] +=
            "-" + (table[count][0]);
        count++;
      }
    } catch (...) {
      cout << "[Exception] function 'bulkloading': error during tree construct. this "
              "column doesn't exist in line :"
           << count << "\n";
      return;
    }
  }

  // Return the tree if this just have a unique node(root) but don't have a
  // mininum keys
  if (nodes[0]->numberOfKeys < (treeOrder - 1) / 2) {
    tree = nodes[0];
    tree->parent = NULL;
    return;
  }

  // Fixes the last node if this don't has a minimun number of elements.
  if (nodes[nodes.size() - 1]->numberOfKeys < (treeOrder - 1) / 2) {
    for (int i = 0; i < nodes[nodes.size() - 1]->numberOfKeys; i++) {
      nodes[nodes.size() - 2]->keys.push_back(nodes[nodes.size() - 1]->keys[i]);
      nodes[nodes.size() - 2]->paths.push_back(
          nodes[nodes.size() - 1]->paths[i]);
      nodes[nodes.size() - 2]->numberOfKeys++;
    }
    nodes[nodes.size() - 2]->nextList = NULL;
    nodes.erase(nodes.begin() + nodes.size() - 1);
  }

  // Return the tree if this just have a unique node(root) and have a mininum
  // keys
  if (nodes.size() == 1) {
    tree = nodes[0];
    tree->parent = NULL;
    return;
  }

  for (size_t i = 1; i < nodes.size(); i++) {
    uppperKeys.push_back(nodes[i]->keys[0]);
    upperPointers.push_back(nodes[i - 1]);
  }
  upperPointers.push_back(nodes[nodes.size() - 1]);

  upFirsts(uppperKeys, upperPointers, treeOrder, tree);
}

void upFirsts(vector<string> &keys, vector<Node *> &pointers, int treeOrder,
              Node *&tree) {
  vector<string> uppperKeys;
  vector<Node *> upperPointers;
  vector<Node *> level;

  size_t count = 0;
  while (count < keys.size()) {
    Node *temporaryNode = new Node(treeOrder);

    while (temporaryNode->numberOfKeys < (treeOrder - 1) / 2 &&
           count < keys.size()) {
      temporaryNode->keys.push_back(keys[count]);
      temporaryNode->pointers.push_back(pointers[count]);
      pointers[count]->parent = temporaryNode;
      temporaryNode->numberOfKeys++;
      count++;
    }

    pointers[count]->parent = temporaryNode;
    temporaryNode->pointers.push_back(pointers[count]);
    level.push_back(temporaryNode);

    if (keys.size() - count > ((size_t)treeOrder - 1) / 2) {
      uppperKeys.push_back(keys[count]);
      upperPointers.push_back(level[level.size() - 1]);
      count++;
    } else {
      while (count < keys.size()) {
        level[level.size() - 1]->keys.push_back(keys[count]);
        level[level.size() - 1]->numberOfKeys++;
        level[level.size() - 1]->pointers.push_back(pointers[count + 1]);
        pointers[count + 1]->parent = level[level.size() - 1];
        count++;
      }
    }
  }

  upperPointers.push_back(level[level.size() - 1]);

  if (level.size() == 1) {
    tree = level[0];
    tree->parent = NULL;
    return;
  }

  upFirsts(uppperKeys, upperPointers, treeOrder, tree);
}

bool searchPathByKey(Node *&tree, KeyType key, KeyType &path) {
  int count = 0;
  while (key.compare(tree->keys[count]) > 0 && count < tree->numberOfKeys) {
    count++;
  }

  if (!key.compare(tree->keys[count]) && tree->isLeaf) {
    path = tree->paths[count];
    return true;
  }

  if (!tree->isLeaf) {
    searchPathByKey(tree->pointers[count], key, path);
  }

  return true;
}

bool searchNodeBykey(Node *&tree, KeyType key, Node *&node) {
  int count = 0;
  while (key.compare(tree->keys[count]) > 0 && count < tree->numberOfKeys) {
    count++;
  }

  if (!key.compare(tree->keys[count]) && tree->isLeaf) {
    node = tree;
    return true;
  }

  if (!tree->isLeaf) {
    searchNodeBykey(tree->pointers[count], key, node);
  }

  return true;
}

void removeNode(KeyType id, Node *&tree, CSVDatabase &table, int treeOrder,
                int column) {

  CSVDatabase sortingAttribute = table;
  sort(sortingAttribute.begin(), sortingAttribute.end(), stringComparator(0));

  int index =
      binarySearch(id, sortingAttribute, 0, sortingAttribute.size() - 1);

  if (index == -1) {
    cout << "function 'removeNode': the informed id: '" + id +
                "' doesn't exist in the table\n";
    return;
  }

  string searchingKey = sortingAttribute[index][column];

  Node *nodeToRemove;
  bool success = searchNodeBykey(tree, searchingKey, nodeToRemove);
  if (!success) {
    cout << "Function 'RemoveNode': the searched key: '" << searchingKey
         << "' not exits";
    return;
  }

  if (DEBUGGER) {
    showVector(nodeToRemove->keys, nodeToRemove->keys.size());
  }

  int nodeIndex = binarySearch(searchingKey, nodeToRemove->keys, 0,
                               nodeToRemove->keys.size() - 1);

  vector<string> paths;
  string currentPath;
  stringstream path(nodeToRemove->paths[nodeIndex]);

  int indexInPath = 0;
  int count = 0;
  while (getline(path, currentPath, '-')) {
    if (!currentPath.compare(id)) {
      indexInPath = count;
    }
    paths.push_back(currentPath);
    count++;
  }

  if (paths.size() > 1) {
    nodeToRemove->paths[nodeIndex] = "";
    paths.erase(paths.begin() + indexInPath);
    size_t returnPaths = 0;
    while (returnPaths < paths.size()) {
      nodeToRemove->paths[nodeIndex] += paths[returnPaths++] + "-";
    }
    nodeToRemove->paths[nodeIndex].erase(nodeToRemove->paths[nodeIndex].size() -
                                         1);
    return;
  }

  nodeToRemove->paths.erase(nodeToRemove->paths.begin() + nodeIndex);
  nodeToRemove->keys.erase(nodeToRemove->keys.begin() + nodeIndex);
  nodeToRemove->numberOfKeys--;
  removeNodeFromVector(nodeToRemove, searchingKey, nodeIndex, treeOrder);
}

void removeNodeFromVector(Node *&node, KeyType searchingKey, int index,
                          int treeOrder) {
  if (node->numberOfKeys > (treeOrder - 1) / 2) {
    return;
  }
  Node *parent = node->parent;

  // Find the index of dad in dad's node
  int parentIndex = 0;
  while (searchingKey.compare(parent->keys[parentIndex]) > 0 &&
         parentIndex < parent->numberOfKeys) {
    parentIndex++;
  }

  // Case the right brother has more than mininum number of keys.
  if (parent->pointers.size() - 1 > (size_t)parentIndex &&
      parent->pointers[parentIndex + 1]->numberOfKeys > (treeOrder - 1) / 2) {

    Node *neighboor = parent->pointers[parentIndex + 1];
    node->keys.push_back(neighboor->keys[0]);
    neighboor->keys.erase(neighboor->keys.begin());

    if (node->isLeaf) {
      node->paths.push_back(neighboor->paths[0]);
      neighboor->paths.erase(neighboor->paths.begin());
    }

    node->numberOfKeys++;
    neighboor->numberOfKeys--;
    parent->keys[parentIndex] = neighboor->keys[0];
    return;
  }

  // Case the left brother has more than mininum number of keys.
  if (parentIndex > 0 &&
      parent->pointers[parentIndex - 1]->numberOfKeys > (treeOrder - 1) / 2) {

    Node *neighboor = parent->pointers[parentIndex - 1];

    node->keys.insert(node->keys.begin(),
                      neighboor->keys[neighboor->numberOfKeys - 1]);
    neighboor->keys.erase(neighboor->keys.begin() + neighboor->numberOfKeys -
                          1);

    if (node->isLeaf) {
      node->paths.insert(node->paths.begin(),
                         neighboor->paths[neighboor->numberOfKeys - 1]);
      neighboor->paths.erase(neighboor->paths.begin() +
                             neighboor->numberOfKeys - 1);
    }

    node->numberOfKeys++;
    neighboor->numberOfKeys--;
    parent->keys[parentIndex] = node->keys[0];
    return;
  }

  // Case brothers haven't more than minimun number of keys
  Node *neighboor = parent->pointers[parentIndex - 1];

  // When current node is leaf...
  if (node->isLeaf) {
    while (node->numberOfKeys) {
      neighboor->keys.push_back(node->keys[0]);
      node->keys.erase(node->keys.begin());

      neighboor->paths.push_back(node->paths[0]);
      node->paths.erase(node->paths.begin());

      node->numberOfKeys--;
      neighboor->numberOfKeys++;
    }
    parent->pointers.erase(parent->pointers.begin() + parentIndex);
    parent->keys.erase(parent->keys.begin() + parentIndex - 1);
    parent->numberOfKeys--;

  } 

  removeNodeFromVector(node->parent, searchingKey, index, treeOrder);
}
