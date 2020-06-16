#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

//--------------------------Defines-----------------------------

typedef vector<string> CSVRow;
typedef CSVRow::const_iterator CSVRowIterator;
typedef vector<CSVRow> CSVDatabase;
typedef CSVDatabase::const_iterator CSVDatabaseIterator;

//--------------------------Headers-----------------------------

bool readCSV(const char *filePath, CSVDatabase &database, int columnId,
             int columnIndex, int numberOfCharsToIndex);
void readCSV(istream &input, CSVDatabase &database, int columnId,
             int columnIndex, int numberOfCharsToIndex);

//--------------------------Functions-----------------------------

bool readCSV(const char *filePath, CSVDatabase &database, int columnId,
             int columnIndex, int numberOfCharsToIndex) {
  // Read the file in path 'FilePath'
  fstream file(filePath, std::ios::in);
  // Verify if path is valid
  if (!file.is_open()) {
    cout << "function 'readCSV': \"" << filePath << "\" not found!\n";
    return false;
  }
  // Read file data
  readCSV(file, database, columnId, columnIndex, numberOfCharsToIndex);
  if (database.size() > 0) {
    return true;
  }
  cout << "function 'readCSV': the table has no attributes.\n";
  return false;
}

void readCSV(istream &input, CSVDatabase &database, int columnId,
             int columnIndex, int numberOfCharsToIndex) {
  string line;
  database.clear();
  int counter = 0;

  try {
    // Read every line from file
    while (getline(input, line)) {
      // Read current line from file
      // Convert string to stringstream, cause is required to 'getLine'
      stringstream csvStream(line);
      CSVRow row;
      string column;
      // Read each column of current line in file
      while (getline(csvStream, column, ',')) {
        column = column.substr(0, numberOfCharsToIndex);
        row.push_back(column);
        counter++;
      }
      counter = 0;
      // Insert curren CSRow in database
      if (row.size() > 0) {
        database.push_back(row);
      }
    }
  } catch (exception &e) {
    cout << "function 'readCSV': error reading this column in input.\n";
  }
}

void display(const CSVDatabase &database) {
  // Verifiy if file contains data
  if (!database.size()) {
    return;
  }
  // Get the first row
  CSVDatabaseIterator row = database.begin();
  // Foreach row
  int i = 0;
  for (; row != database.end(); ++row) {
    // Verify if row contains data
    if (!row->size()) {
      continue;
    }
    // Get the first column
    CSVRowIterator column = row->begin();
    // Print first column, to no show ','
    cout << i++ << "->" << *(column++);
    // Print others columns
    for (; column != row->end(); ++column) {
      cout << ',' << *column;
    }
    // Broken line
    cout << "\n";
  }
}
