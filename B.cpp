#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "B.h"
#include "functions.cpp"
	
using namespace std;

#define DEBUGGER false

/*
 *  Print tree starting by root
 *  Insert a tab to indicate a new level  
 */
void printTree(Node* &node, int tabs){

	if(node->_isLeaf){//Verify if current level is leaf to ends print 

		for(int i = 0; i < node->_numberKeys; i++){
			int currTabs = tabs;
			while(currTabs--)
				cout << "\t";
			cout << node->_keys[i];
			
			if(DEBUGGER)
				cout<< "->" << node->_paths[i];
			cout<<"\n";
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
/*
 *	Make a BulkLoading insert in tree 
 *  creating a minimum tree, 
 *  this function makes a leaf nodes and call other function
 *  'UpFisrts' that create others levels of tree.
 */
void BulkLoadingInsert(Node* &tree, CSVDatabase &_Table, int n_Order, int _Column){


	int count = 0;
	vector<Node*> nodes;
	vector<string> _uppperKeys;
	_uppperKeys.clear();
	vector<Node*> _upperPointers;
	_upperPointers.clear();

	while(count <  _Table.size()){ //walks in table 

		Node* _Temp = new Node(n_Order, true);
		_Temp->_paths.clear();
		// while node don't has the miminum number of elements this function insert elements
		while(_Temp->_numberKeys < (n_Order-1)/2 && count <  _Table.size()){

			if (_Temp->_numberKeys > 0 && !_Table[count][_Column].compare(_Temp->_keys[_Temp->_numberKeys-1])){
				_Temp->_paths[_Temp->_paths.size()-1]+= "-" + _Table[count][0];
			}
		
			else{
				_Temp->_keys.push_back(_Table[count][_Column]);
				_Temp->_paths.push_back(_Table[count][0]); //column of id
				_Temp->_numberKeys++;
			}	
			count++;
		}
	
		//cout << count;
		if(nodes.size() > 0){
			nodes[nodes.size()-1]->_nextList = _Temp;
			_Temp->_lastList = nodes[nodes.size()-1];
		}

		nodes.push_back(_Temp);

		while(count <  _Table.size() && nodes.size() > 0 && 
			!_Table[count][_Column].compare(nodes[nodes.size()-1]->_keys[nodes[nodes.size()-1]->_numberKeys-1])){
			 nodes[nodes.size()-1]->_paths[nodes[nodes.size()-1]->_paths.size()-1]+= "-" + (_Table[count][0]);
			 count++;
		}
	}

	//Return the tree if this just have a unique node(root) but don't have a mininum keys 
	if(nodes[0]->_numberKeys < (n_Order-1)/2){
		tree = nodes[0];
		tree->_dad = NULL;
		return;
	}

	//Fixes the last node if this don't has a minimun number of elements.
	if(nodes[nodes.size()-1]->_numberKeys <  (n_Order-1)/2){
		for(int i = 0; i < nodes[nodes.size()-1]->_numberKeys; i++){
			nodes[nodes.size()-2]->_keys.push_back(nodes[nodes.size()-1]->_keys[i]);
			nodes[nodes.size()-2]->_paths.push_back(nodes[nodes.size()-1]->_paths[i]);	
			nodes[nodes.size()-2]->_numberKeys++;
		}
		nodes[nodes.size()-2]->_nextList = NULL;
		nodes.erase(nodes.begin() + nodes.size()-1);
	}
		
	//Return the tree if this just have a unique node(root) and have a mininum keys 
	if(nodes.size() == 1){
		tree = nodes[0];
		tree->_dad = NULL;
		return;
	}
	
	for(int i = 1; i < nodes.size(); i++){

		_uppperKeys.push_back(nodes[i]->_keys[0]);	
		_upperPointers.push_back(nodes[i-1]);
	}
	_upperPointers.push_back(nodes[nodes.size()-1]);

	UpFirsts(_uppperKeys, _upperPointers, n_Order, tree);
}

void UpFirsts(vector<string> &_Keys, vector<Node*> &_Pointers, int n_Order, Node* &tree){

	vector<string> _uppperKeys;
	vector<Node*> _upperPointers;
	vector<Node*> _level;

	int count = 0;
	while(count < _Keys.size()){
		
		 Node* _Temp = new Node(n_Order);
		
		while(_Temp->_numberKeys < (n_Order-1)/2 && count <  _Keys.size()){
			
			_Temp->_keys.push_back(_Keys[count]);
			_Temp->_pointers.push_back(_Pointers[count]);
			_Pointers[count]->_dad = _Temp;
			_Temp->_numberKeys++;
			count++;
		}

		_Pointers[count]->_dad = _Temp;
		_Temp->_pointers.push_back(_Pointers[count]);
		_level.push_back(_Temp);
		
		if(_Keys.size() - count > (n_Order-1)/2){
			_uppperKeys.push_back(_Keys[count]);
			_upperPointers.push_back(_level[_level.size()-1]);
			count++;
		}
		else{

			while(count < _Keys.size()){
				_level[_level.size()-1]->_keys.push_back(_Keys[count]);
				_level[_level.size()-1]->_numberKeys++;
				_level[_level.size()-1]->_pointers.push_back(_Pointers[count+1]);
				count++;
			}
		}
	}

	_upperPointers.push_back(_level[_level.size()-1]);

	if(_level.size() == 1){
		tree = _level[0];
		tree->_dad = NULL;
		return ;
	}

	UpFirsts(_uppperKeys, _upperPointers, n_Order, tree);
}

bool searchPathByKey(Node* &tree, KeyType key, KeyType &path){
	int count = 0;

	while(key.compare(tree->_keys[count]) && count < tree->_numberKeys)
		count++;
	
	if(!key.compare(tree->_keys[count]) && tree->_isLeaf){
		path = tree->_paths[count];
		return true;
	}
	
	if(!tree->_isLeaf)
		searchPathByKey(tree->_pointers[count], key, path);
	
	return false;
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


	
	/*Node* currentLeaf = startLeafs;

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

	while(getline(path, currentPath, '-'))
		paths.push_back(currentPath);
	
	currentPath = "";

	int _aux = 0; 
	while(paths[_aux].compare(id))
		_aux++;
	
	if(paths.size() > 1){

		paths.erase(paths.begin() + _aux);
		
		_aux = 0;
		while(_aux < paths.size())
			currentPath += paths[_aux++] + "-";

		currentPath.erase(currentPath.size()-1);
		currentLeaf->_paths[_aux] = currentPath;

		return;
	}

	RemoveNodeOfVector(currentLeaf, _searchKey, _index, n_Order);
	*/
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