#include "Node.h"

Node::Node(const char * id)
{
	int idLength = strlen(id) + 1;
	this->nodeID = new char[idLength];
	strcpy_s(nodeID, idLength, id);
	nextNode = NULL;
	successorsTop = NULL;
	successorsEnd = NULL;

}

Node::~Node()
{
	if (nodeID)
		delete nodeID;
	nodeID = NULL;
	clearAllSucc();
}

void Node::addSuccessor(const char * id, int w)
{
	if (!successorsTop)
	{
		successorsTop=successorsEnd = new Successor(id, w);
	}
	else
	{
		successorsEnd->nextSucc = new Successor(id, w);
	    successorsEnd=successorsEnd->nextSucc;
	}
}

Successor* Node::checkIfSuccessor(const char * id)
{
	Successor*temp = successorsTop;

	if (temp  && strcmp(temp->succID, id) == 0)
		return temp;

	while (temp)
	{
		if (temp->nextSucc && strcmp(temp->nextSucc->succID,id) == 0)
		{
			return temp;
		}
		temp = temp->nextSucc;
	}

	return NULL;

}

void Node::removeSucc(const char * id)
{

	Successor*deleteAfter = checkIfSuccessor(id);

	Successor*temp;
	
	 if (successorsTop && deleteAfter == successorsTop &&  strcmp(successorsTop->succID,id)==0)
	 {
		bool hasOnlySucc = successorsTop == successorsEnd;
		
		temp = successorsTop->nextSucc;
		delete successorsTop;
		successorsTop = temp;
		if (hasOnlySucc)
			successorsEnd = temp;
		return;
	 }

	    //if successor exists and it is not the top
	 if (deleteAfter->nextSucc)
	 {
		 bool flag = successorsEnd == deleteAfter->nextSucc;
		
		 temp = deleteAfter->nextSucc->nextSucc;
		
		 delete deleteAfter->nextSucc;
		 
		 deleteAfter->nextSucc = temp;
		 
		 if (flag)
		 {
			 if (temp)
				 successorsEnd = temp;
			 else
				 successorsEnd = deleteAfter;
		 }
	 }
	 

}



void Node::clearAllSucc()
{
	if (successorsTop)
	{
		//if there is only one successor
		if (successorsTop == successorsEnd)
			delete successorsEnd;
		else
		{
			Successor*temp = successorsTop;
			while (successorsTop)
			{
				successorsTop = successorsTop->nextSucc;
				delete temp;
				temp = successorsTop;
			}
		}
		
		successorsTop = successorsEnd = NULL;
	}
}

void Node::printOneNodesSuccs(std::ostream & os)
{
	Successor*temp = successorsTop;
	os << nodeID;
	while (temp)
	{
		os << ' ';
		temp->print(os);
		temp = temp->nextSucc;
	}
	os << '\n';
}







