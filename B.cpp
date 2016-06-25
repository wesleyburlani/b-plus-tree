#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "B.h"
#include "functions.cpp"
	
using namespace std;

void printTree(Node* &node, int tabs){

	if(node->_isLeaf){
		for(int i = 0; i < node->_numberKeys; i++){
			int currTabs = tabs;
			while(currTabs--)
				cout << "\t";
			cout << node->_keys[i] << "\n";			
		}
		return;
	}
		
	for(int i = 0; i < node->_numberKeys; i++){
		
		printTree(node->_pointers[i], tabs + 1);

		int currTabs = tabs;
		while(currTabs--)
			cout <<  "\t";

		cout << node->_keys[i] << "\n";		 
 	}	
	printTree(node->_pointers[node->_numberKeys], tabs + 1);
}

bool SplitVector(Node* &base, KeyType &upper, Node* &left, Node* &right){

	upper = base->_keys[(base->_keys.size()-1)/2];

	int i = 0 ;
	while(i  < base->_keys.size()/2){
		left->_keys[left->_numberKeys] = base->_keys[i];
		left->_pointers[left->_numberKeys++] = base->_pointers[i++];
	}

	left->_pointers[left->_numberKeys] = base->_pointers[i];

	while(i  < base->_keys.size()){
		right->_keys[right->_numberKeys] = base->_keys[i];
		right->_pointers[right->_numberKeys++] = right->_pointers[i++];
	}

	right->_pointers[right->_numberKeys] = right->_pointers[i];
}

Node* getNodeToAdd(Node* node, KeyType value){
	
	Node* aux = node;

	if(aux->_isLeaf)
		return aux;

	int i = 0;
	while (value > aux->_keys[i] && i < node->_numberKeys)
		i++;

	aux = getNodeToAdd(aux->_pointers[i], value);
}

void BppInsert(Node* &tree, KeyType value, int n_Order){


	if(tree == NULL){
		tree = new Node(n_Order);
		tree->_keys[0] = value;
		tree->_isLeaf = true;
		tree->_numberKeys++;
		tree->_dad = NULL;
	}
	
	else if(tree->_numberKeys < 2*n_Order-1 && tree->_isLeaf){
			tree->_keys[tree->_numberKeys++] = value;
	}
	
	
	else if(tree->_numberKeys == 2*n_Order-1 && tree->_isLeaf){
		
		Node *left = new Node(n_Order),
			*right= new Node(n_Order);
		
		KeyType upper;

		tree->_keys.push_back(value);

		SplitVector(tree, upper, left, right);
		
		tree->_keys.resize(2*n_Order-1);
		tree->_keys[0] = upper;
		tree->_numberKeys = 1;
		tree->_isLeaf = false;
		tree->_pointers[0] = left;
		tree->_pointers[1] = right;

		right->_isLeaf = left->_isLeaf = true;
		right->_dad = left->_dad = tree;

		left->_pointers[2*n_Order] = right;
	}

	else{

		
	}
	
}

void BulkLoadingInsert(Node* &tree, CSVDatabase &_Table, int n_Order, int _Column){

	int count = 0;
	vector<Node*> nodes;

	while(count <  _Table.size()){

		Node* _Temp = new Node(n_Order, true);
		_Temp->_keys.clear();
		_Temp->_paths.clear();
		_Temp->_numberKeys = 0;

		while(_Temp->_numberKeys < (n_Order-1)/2 && count <  _Table.size()){
			
			_Temp->_keys.push_back(_Table[count][_Column]);
			_Temp->_paths.push_back(_Table[count][0]); //column of id
			_Temp->_numberKeys++;
			
			count++;
		}
		
		if(nodes.size() > 0){
			nodes[nodes.size()-1]->_nextList = _Temp;
			_Temp->_lastList = nodes[nodes.size()-1];
		}

		nodes.push_back(_Temp);
	}

	if(nodes[0]->_numberKeys <  (n_Order-1)/2){
		tree = nodes[0];
		tree->_dad = NULL;
		return;
	}

	if(nodes[nodes.size()-1]->_numberKeys <  (n_Order-1)/2){

		for(int i = 0; i < nodes[nodes.size()-1]->_numberKeys; i++){
			
			nodes[nodes.size()-2]->_keys.push_back(nodes[nodes.size()-1]->_keys[i]);
			nodes[nodes.size()-2]->_paths.push_back(nodes[nodes.size()-1]->_paths[i]);	
			nodes[nodes.size()-2]->_numberKeys++;
		}

		nodes[nodes.size()-2]->_nextList = NULL;
		nodes.erase(nodes.begin() + nodes.size()-1);
	}
	
	if(nodes.size() == 1){
		tree = nodes[0];
		tree->_dad = NULL;
		return;
	}
	
	UpFirsts(nodes, n_Order, tree);
}

void UpFirsts(vector<Node*> &_nodes, int n_Order, Node* &tree){
	
	vector<string> _uppperKeys;
	vector<Node*> _Uppers,
				  _upperPointers;

	for(int i = 1; i < _nodes.size(); i++){
		
		_uppperKeys.push_back(_nodes[i]->_keys[0]);	
		_upperPointers.push_back(_nodes[i-1]);
	}
	
	_upperPointers.push_back(_nodes[_nodes.size()-1]);
	
	int count = 0;
	while(count < _uppperKeys.size()){
			
		 Node* _Temp = new Node(n_Order);
		_Temp->_keys.clear();
		_Temp->_pointers.clear();
		_Temp->_numberKeys = 0;
		
		while(_Temp->_numberKeys < (n_Order-1)/2 && count <  _uppperKeys.size()){
			
			_Temp->_keys.push_back(_uppperKeys[count]);
			_Temp->_pointers.push_back(_upperPointers[count]);
			_upperPointers[count]->_dad = _Temp;
			_Temp->_numberKeys++;
			
			count++;
		}

		_Temp->_pointers.push_back(_upperPointers[count]);
		_upperPointers[count]->_dad = _Temp;
		_Uppers.push_back(_Temp);
	}

	if(_Uppers.size() == 1){
		tree = _Uppers[0];
		tree->_dad = NULL;
		return;
	}

	UpFirsts(_Uppers, n_Order,tree);	
}

void RemoveNode(KeyType id, Node* &tree, CSVDatabase &_Table, CSVDatabase &_NoGroup, int n_Order, int _Column){

	int _index = BinarySearch(id, _NoGroup, 0, _NoGroup.size()-1);

	if(_index == -1){
		cout << "Function 'RemoveNode': The informed id: '" + id + "' not exists in table\n";
		return;
	}

	string _searchKey = _NoGroup[_index][_Column];
	Node* startLeafs = tree;
	while(!startLeafs->_isLeaf)
		startLeafs = startLeafs->_pointers[0];
	
	Node* currentLeaf = startLeafs;

	_index = -1;
	while(currentLeaf != NULL && _index == -1){
		
		_index = BinarySearch(_searchKey, currentLeaf->_keys, 0, currentLeaf->_numberKeys-1);
		if(_index == -1)
			currentLeaf = currentLeaf->_nextList;
	}

	if(_index == -1){
		cout << "Function 'RemoveNode': The informed value: '" + _searchKey + "' not exists in tree\n";
		return;
	}

	stringstream path(currentLeaf->_paths[_index]);
	vector<string> paths;
	paths.clear();
	string currentPath;

	while(getline(path, currentPath, ','))
		paths.push_back(currentPath);
	
	currentPath = "";

	int _aux = 0; 
	while(paths[_aux].compare(id))
		_aux++;
	
	if(paths.size() > 1){

		paths.erase(paths.begin() + _aux);
		
		_aux = 0;
		while(_aux < paths.size())
			currentPath += paths[_aux++] + ",";

		currentPath.erase(currentPath.size()-1);
		currentLeaf->_paths[_aux] = currentPath;

		return;
	}

	RemoveNodeOfVector(currentLeaf, _searchKey, _index, n_Order);
}

void RemoveNodeOfVector(Node* &_node, KeyType _searchKey, int _index, int n_Order){

	_node->_keys.erase(_node->_keys.begin() + _index);
	_node->_numberKeys--;
	
	if(_node->_isLeaf)
		_node->_paths.erase(_node->_paths.begin() + _index);
	else
		_node->_pointers.erase(_node->_pointers.begin() + _index);

	
	if(_node->_numberKeys > (n_Order - 1)/2)
		return;
	
}