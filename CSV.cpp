#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


using namespace std;

//--------------------------Defines-----------------------------

typedef vector<string> CSVRow;
typedef CSVRow::const_iterator CSVRowIterator;
typedef vector<CSVRow> CSVDatabase;
typedef CSVDatabase::const_iterator CSVDatabaseIterator;


//--------------------------Headers-----------------------------

bool readCSV(const char* FilePath, CSVDatabase &database, int _idColumn, int _indexColumn, int n_CHAR);
void readCSV(istream &input, CSVDatabase &database, int _idColumn, int _indexColumn, int n_CHAR);


//--------------------------Functions-----------------------------

bool readCSV(const char* FilePath, CSVDatabase &database, int _idColumn, int _indexColumn, int n_CHAR){

	//Read the file in path 'FilePath'
	fstream file(FilePath, std::ios::in);
	//Verify if path is valid
	if(!file.is_open()){

		cout << "Function 'readCSV': \"" << FilePath << "\" not found!\n";
		return false;
	}
	// Read file data
	readCSV(file, database, _idColumn, _indexColumn, n_CHAR);

	if(database.size() > 0)
		return true;

	cout << "No attributes.\n";
		return false;
}

void readCSV(istream &input, CSVDatabase &database, int _idColumn, int _indexColumn, int n_CHAR){
		
	string csvLine;
	database.clear();
	int counter = 0;
	// Read every line from file
	//getline(input, csvLine);// remove first line;

	while(getline(input, csvLine)){

		//Read current line from file
		stringstream csvStream(csvLine);// Convert string to stringstream, cause is required to 'getLine'
		CSVRow csvRow;
		string csvCol;
		
		//Read each column of current line in file
		while(getline(csvStream, csvCol, ',')){

			csvCol = csvCol.substr(0,n_CHAR);
			//Insert current column in CSVROW line
			//if(counter == _idColumn || counter == _indexColumn){
				//csvCol.erase(remove(csvCol.begin(), csvCol.end(), '\"'), csvCol.end());
				csvRow.push_back(csvCol);
			//}
	  		counter++;
		}
		counter = 0;
		//Insert curren CSRow in database
		//if(csvRow.size() > 0)
		database.push_back(csvRow);
	}

}

void display(const CSVDatabase &database){
  
  	//Verifiy if file contains data
	if(!database.size())
		return;
	// Get the first row
	CSVDatabaseIterator row = database.begin();
	// Foreach row
	int i = 0 ;
	for(; row != database.end(); ++row){
		//Verify if row contains data
		if(!row->size())
    		continue;
  		// Get the first column
  		CSVRowIterator column = row->begin();
  		// Print first column, to no show ','

  		cout<< i++ << "->" <<*(column++);
  		//Print others columns
  		for(;column != row->end();++column)
    		cout<<','<<*column;
    	//Broken line
		cout<< "\n";
	}
}

void groupBy(CSVDatabase &database, int _indexId, int _indexColumn){

	CSVDatabase _Temp;

	if(!database.size()){
		cout << "";
		return;
	}

	int count = 1;

	while(count < database.size()){

		CSVRow _TempRow;
		_TempRow.clear();

		string offset = "";
		offset += database[count-1][_indexId];
		
		while(count < database.size() && !database[count][_indexColumn].compare(database[count-1][_indexColumn])){
			offset += "-" + database[count][_indexId];
			count++;
		}

		_TempRow.push_back(offset);
		_TempRow.push_back(database[count-1][_indexColumn]);
		_Temp.push_back(_TempRow);

		count++;
	}
	database = _Temp;
}


