# B+ tree

Implements a b+ tree using bulk loading insertion to create the tree from a csv input file and allows user to search data id by indexed key and remove nodes.

To find an id from indexed key simulates how most relational databases can index table columns and performs fast searches because they indexes the column on a tree and execute this search working only with ids after it.

The program asks to user some information about the tree before create it. The information asked is:

* Column to index: which column of csv input file should be indexed
* Number of characteres to index: how many letters of column should be indexed
* Tree order: the maximum number of a single node childrens 
  
By default, the program users `data/normalized-data.csv` file to mount tree. This can be changed modifying `DATAFILEPATH` definition on `src/main.cpp` file. On this file it is possible to modify the `IDINDEX` definition to change csv input file id column, which, by default, points to column 0. (the first file column).

## Running program locally ## 

To run the program locally it is required to have the g++ compiler installed and run the following commands: 

```sh
./build.sh
./run.sh
```

where the first one compiles the program and de last one executes it.

The program looks like this: 

```
Welcome!
Type the column to index: 1
Alright!
Type the number of characters to index: 2
Ok.. Type the tree order: 3

The tree was created successfully. What you want to do ?

1.Print tree
2.Search id by Key
3.Remove a node
9.Exit Program

Enter with an option id:
```

This example creates a b+ tree using the second column(index12) of csv input file `examples.csv` using the first 2 characteres of column value.

The tree is printed like this: 

```
                De
        No
                No
We
                We
        do
                do
fu
                fu
        ro
                ro
```