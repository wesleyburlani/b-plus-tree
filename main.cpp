#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "CSV.cpp"
#include "B.cpp"

#define IndexOfId 0

using namespace std;
    
int main(){

	//-----------------Definitions-----------------
	string _MESSAGE = "Welcome!\n";
  	cout << _MESSAGE;

	int n_ATTR;
	int n_CHAR;
	int n_ORDER;
	CSVDatabase _Table;
	CSVDatabase _NoGroup;		
	
	cout<< "Please, enter with number of attribute in CSV table that you want to index in tree: ";
	
	scanf("%d",&n_ATTR);

    cout<< "Please, enter with number of characters that you want ot index: ";
    scanf("%d",&n_CHAR);

    cout<< "Please, enter with order of tree: ";
	while(scanf("%d",&n_ORDER) && n_ORDER < 3){

	  cout << "+-------------------------------------------------------------+\n"; 
	  cout<<  "| WARNING: The order of tree must be a integer greater than 2 |\n";
	  cout << "+-------------------------------------------------------------+\n";
	  cout << "please, re-enter: "; 
	}
	
	bool success = readCSV("Book2.csv", _Table, IndexOfId, n_ATTR, n_CHAR);
	if(!success)
		return 1;
	
	_NoGroup = _Table;
	sort(_NoGroup.begin(), _NoGroup.end());
	sort(_Table.begin(), _Table.end(), comparator(n_ATTR));

	groupBy(_Table, 0, 1);

	display(_NoGroup);

	Node* tree = NULL;

	BulkLoadingInsert(tree, _Table, n_ORDER, n_ATTR);
	printTree(tree, 0);

	//RemoveNode("2", tree, _Table, _NoGroup, n_ORDER, 1);
	//printTree(tree, 0);
}
