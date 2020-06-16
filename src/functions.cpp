#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class comparator {
public:
  int column;
  bool operator()(vector<string> x, vector<string> y) {
    return x[column] < y[column];
  }

  comparator(int attr) { this->column = attr; }
};

class comparatorInt {
public:
  bool operator()(vector<string> x, vector<string> y) {
    return atoi(x[0].c_str()) < atoi(y[0].c_str());
  }
};

void printVector(vector<KeyType> _vector, int size) {

  vector<KeyType>::iterator IT = _vector.begin();

  for (; IT != _vector.begin() + size; ++IT)
    cout << " | " << *IT;
  cout << " |";
}

void showVector(vector<KeyType> _vector, int size) {

  cout << "printing vector ... \n";
  printVector(_vector, size);
  cout << '\n';
}

void ShowListOfVectors(vector<Node *> _nodes) {

  for (int i = 0; i < _nodes.size(); i++) {
    printVector(_nodes[i]->_keys, _nodes[i]->_numberKeys);
    printf("%s", (i < _nodes.size() - 1) ? "->" : "\n");
  }
}

void _sort(vector<KeyType> &_vector, int size) {

  sort(_vector.begin(), _vector.begin() + size);
}

// --------------------------Binary Searches-----------------------------

int BinarySearch(KeyType value, CSVDatabase _vector, int _left, int _right) {

  int _index = (_left + _right) / 2;
  string position = _vector[_index][0];

  if (!position.compare(value))
    return _index;

  if (_left == _right)
    return -1;

  if (position.compare(value) < 0)
    return BinarySearch(value, _vector, _index + 1, _right);

  return BinarySearch(value, _vector, _left, _index - 1);
}

int BinarySearch(KeyType value, vector<KeyType> _vector, int _left,
                 int _right) {

  int _index = (_left + _right) / 2;
  string position = _vector[_index];

  if (!position.compare(value))
    return _index;

  if (_left == _right)
    return -1;

  if (position < value)
    return BinarySearch(value, _vector, _index + 1, _right);

  return BinarySearch(value, _vector, _left, _index - 1);
}

bool DataCollect(int &n_ATTR, int &n_CHAR, int &n_ORDER) {

  cout << "Enter with number of attribute : ";

  scanf("%d", &n_ATTR);

  cout << "Alright!\nEnter with number of characters that you want ot index: ";
  scanf("%d", &n_CHAR);

  cout << "Ok.. Enter with order of tree: ";
  while (scanf("%d", &n_ORDER) && n_ORDER < 3) {

    cout << "Oops.. The order of tree must be a integer greater than 2..\n";
    cout << "please, re-enter: ";
  }

  return true;
}
// -------------------------------------------------------------------------
