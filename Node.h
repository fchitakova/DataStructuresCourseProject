#pragma once
#ifndef NODE_H
#define NODE_H

#include "Successor.h"

struct Node
{
	char*nodeID;
	Node*nextNode;

	Successor*successorsTop;
	Successor*successorsEnd;

	Node(const char*id);
	Node(const Node&other) = delete;
	Node&operator=(const Node&other) = delete;
	~Node();


   //adds arc from this node to node with id
   void addSuccessor(const char*id, int w=-1);

   //checks if this successor occurs and if yes returns pointer to it
   Successor* checkIfSuccessor(const char*id);

   //removes the successor if it is found in the list
   void removeSucc(const char*id);

   //deletes all the successors
   void clearAllSucc();

   //prints the successors of one node in the graph
   void printOneNodesSuccs(std::ostream&os);
};

#endif