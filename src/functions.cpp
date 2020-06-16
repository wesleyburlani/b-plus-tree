#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class stringComparator {
public:
  int column;
  bool operator()(vector<string> x, vector<string> y) {
    return x[column] < y[column];
  }

  stringComparator(int attr) { this->column = attr; }
};

class intComparator {
public:
  bool operator()(vector<string> x, vector<string> y) {
    return atoi(x[0].c_str()) < atoi(y[0].c_str());
  }
};

void printVector(vector<KeyType> _vector, int size) {
  vector<KeyType>::iterator iterator = _vector.begin();

  for (; iterator != _vector.begin() + size; ++iterator) {
    cout << " | " << *iterator;
  }
  cout << " |";
}

void showVector(vector<KeyType> _vector, int size) {
  cout << "printing vector ... \n";
  printVector(_vector, size);
  cout << '\n';
}

void showListOfVectors(vector<Node *> _nodes) {
  for (size_t i = 0; i < _nodes.size(); i++) {
    printVector(_nodes[i]->keys, _nodes[i]->numberOfKeys);
    printf("%s", (i < _nodes.size() - 1) ? "->" : "\n");
  }
}

void sortVector(vector<KeyType> &_vector, int size) {
  sort(_vector.begin(), _vector.begin() + size);
}

int binarySearch(KeyType value, CSVDatabase _vector, int leftNode,
                 int rightNode) {
  int index = (leftNode + rightNode) / 2;
  string position = _vector[index][0];

  if (!position.compare(value)) {
    return index;
  }
  if (leftNode == rightNode) {
    return -1;
  }
  if (position.compare(value) < 0) {
    return binarySearch(value, _vector, index + 1, rightNode);
  }
  return binarySearch(value, _vector, leftNode, index - 1);
}

int binarySearch(KeyType value, vector<KeyType> _vector, int leftNode,
                 int rightNode) {
  int index = (leftNode + rightNode) / 2;
  string position = _vector[index];

  if (!position.compare(value)) {
    return index;
  }
  if (leftNode == rightNode) {
    return -1;
  }
  if (position < value) {
    return binarySearch(value, _vector, index + 1, rightNode);
  }
  return binarySearch(value, _vector, leftNode, index - 1);
}

bool stdInDataCollect(int &columnToIndex, int &numberOfCharsToIndex,
                      int &treeOrder) {
  cout << "Type the column to index: ";
  scanf("%d", &columnToIndex);

  cout << "Alright!\nType the number of characters to index: ";
  scanf("%d", &numberOfCharsToIndex);

  cout << "Ok.. Type the tree order: ";
  while (scanf("%d", &treeOrder) && treeOrder < 3) {
    cout << "Oops.. The tree order must be an integer greater than 2..\n";
    cout << "please, re-enter: ";
  }

  return true;
}
