#include "Graph.h"

int Graph::numVertexes = 0;

Graph::Graph(const char * graphID, const char * fullPathName, bool isDirected)
{
	int idLength = strlen(graphID) + 1;
	this->graphID = new char[idLength];
	strcpy_s(this->graphID, idLength, graphID);

	int fullPathLength = strlen(fullPathName) + 1;
	this->fullPathName = new char[fullPathLength];
	strcpy_s(this->fullPathName, fullPathLength, fullPathName);

	top = end = NULL;
	this->isDirected = isDirected;
}

Graph::~Graph()
{
	deleteGraph();
}

void Graph::setNewFullPath(const char * newName)
{
	if (fullPathName)
	{
		delete[]fullPathName;
		fullPathName = NULL;
	}
	int fullPathLength = strlen(newName) + 1;
	this->fullPathName = new char[fullPathLength];
	strcpy_s(this->fullPathName, fullPathLength, newName);

}

void Graph::saveGraphToAFile(const char*filePath)
{
	std::ofstream saveIn(filePath,std::ofstream::trunc);

	this->printAllNodesSucc(saveIn);

	saveIn.close();
}


//loads existing graph from a file
void Graph::loadGraph(const char * filename,const char*id)
{

	std::ifstream file(filename);
	
	if (file.is_open())
	{
		char*line = new char[2048];

		file.getline(line, 2048, '\n');

		if (line[0] == 'd')
			this->isDirected = true;


		char*currNode = new char[32];
		char* succNode = new char[32];
		char*weight = new char[32];

		int weightNum;
		int itrInx;

		while (file.getline(line, 2048, '\n'))
		{
			int i = 0;
			itrInx = 0;

			int length = strlen(line);
			while (i<length)
			{
				
				if (itrInx == 0)
				{

					for (; line[i] != ' '; ++i)
					{
						currNode[i] = line[i];
					}
					currNode[i++] = '\0';
					++itrInx;
					this->createNode(currNode,true);
				}

				if (i>=length)
					break;

				int j;
				for (j=0; line[i] != ' ' && line[i]!='\0';++j, ++i)
					succNode[j] = line[i];
				
				succNode[j] = '\0';
				++i;

				if (i >= length)
					break;
                  
				for (j = 0; line[i] != ' ' && line[i]!='\0'; ++j, ++i)
					weight[j] = line[i];
				weight[j] = '\0';
				++i;
				weightNum = atoi(weight);
                
				this->createNode(succNode,true);
				this->createArc(currNode, succNode, weightNum);
			}
		}
		
		delete[]weight;
		weight = NULL;

		delete[]currNode;
		currNode = NULL;
		
		delete[]succNode;
		succNode = NULL;

		delete[]line;
		line = NULL;
	}
	file.close();
}


void Graph::createNode(const char * id,bool isForLoading)
{

	++numVertexes;

	Node*insertAfter = searchForNode(id);
	
	if (!isForLoading)
	{
		if (!insertAfter)
		{
			if (!top)
			{
				top = end = new Node(id);
				return;
			}

			end->nextNode = new Node(id);
			end = end->nextNode;
		}
		else
				std::cout << "Node " << id << " already exists!\n";
	}
	else
	{
		if (!insertAfter)
		{
			if (!top)
			{
				top = end = new Node(id);
				return;
			}

			end->nextNode = new Node(id);
			end = end->nextNode;
		}
	}

}

Node * Graph::searchForNode(const char * id)
{
	if (top)
	{
		if (strcmp(top->nodeID, id) == 0)
			return top;

		Node*temp = top;

		while (temp)
		{
			if (temp->nextNode && strcmp(temp->nextNode->nodeID, id) == 0)
				return temp;
			temp = temp->nextNode;
		}

	}
	//if there is not node with id
	return NULL;
}

void Graph::deleteNode(const char * id)
{
	Node*toDeleteAfter = searchForNode(id);
	if (!toDeleteAfter)
	{
		std::cout << "Node " << id << " does not exists!\n";
		return;
	}


	Node*temp = top;

	while (temp)
	{
		Node*temp2;
		
		//first node is the searched for deleting
		if (temp == top && toDeleteAfter == top && strcmp(toDeleteAfter->nodeID ,id)==0)
		{
			temp2 = top->nextNode;
			delete temp;
			top = temp2;
			temp = top;
		}
		
		else if (temp->nextNode && temp->nextNode==toDeleteAfter->nextNode)
		{
			temp2 = temp->nextNode->nextNode;
			delete temp->nextNode;
			temp->nextNode = temp2;
		}
	
		else
		{
			temp->removeSucc(id);
			temp = temp->nextNode;
		}
	}


}


void Graph::createArc(const char * id1, const char * id2, int weight)
{
	Node*firstNode = searchForNode(id1),
		*secondNode = searchForNode(id2);
	
	if (!firstNode|| !secondNode)
	{
		std::cout << "Cannot create arc between non-existing nodes!\n";
		return;
	}
	
	if (firstNode==top && strcmp(top->nodeID,id1)==0)
	{
		firstNode->addSuccessor(id2, weight);
		if (!isDirected)
			secondNode->addSuccessor(id1, weight);



	}
	else
	{
		
		firstNode->nextNode->addSuccessor(id2, weight);
		if (!isDirected)
			secondNode->nextNode->addSuccessor(id1, weight);
	}

}

void Graph::deleteArc(const char * id1, const char * id2)
{
	Node*firstNode = searchForNode(id1),
		*secondNode = searchForNode(id2);
	if (!firstNode || !secondNode)
	{
		std::cout << "Cannot delete arc between non-existing nodes!\n";
		return;
	}

	firstNode->removeSucc(id2);
	if (!isDirected)
		secondNode->removeSucc(id1);


}

void Graph::deleteGraph()
{

	if (graphID)
	{
		delete[]graphID;
		graphID = NULL;
	}

	if (fullPathName) 
	{
		delete[]fullPathName;
		fullPathName=NULL;
	}

	Node*temp=top;
	
	if (top)
	{

		top = top->nextNode;
        delete temp;
		temp = top;
	}
	top = end = NULL;
}

void Graph::printAllNodesSucc(std::ostream & os)
{
	Node*temp = top;
	if (this->isDirected)
		os << "directed\n";
	else
		os << "undirected\n";
	while (temp)
	{
		temp->printOneNodesSuccs(os);
		temp = temp->nextNode;
	}
}

/*
void Graph::bfs(const char * id1, const char * id2)
{
	
	//this is the equivalent of visited array
	std::list<std::pair<bool, char*>>visited;

	Node*temp = this->top;

	while (temp)
	{
		visited.push_back(std::pair<bool, char*>(false, temp->nodeID));
		temp = temp->nextNode;
	}
	
	std::list<char*> queue;

	visited.

	std::list<char*>::iterator i;

	while (!queue.empty())
	{

	}


	Node*start = searchForNode(id1);

	for (int i = 0; i < nu; i++)
		visited[i] = false;





	std::list<Node>queue;



	delete[]visited;
	visited = NULL;
}
*/



















