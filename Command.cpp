#include "Command.h"

bool searchForGraph(const char *path,const char*id)
{

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(path, &ffd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "FindFirstFile failed! Error code is " << GetLastError() << std::endl;
		return false;
	}

	do
	{

		if(strcmp(ffd.cFileName,id)==0)
		{

			return true;
		}

	} while (FindNextFile(hFind, &ffd) != 0);


	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		return false;
	}


	return false;
}

void Command::CREATE_GRAPH(const char*path,const char * id, bool isDirected,Graph*&currentGraph)
{

	bool result = searchForGraph(path, id);
	
	if (!result)
	{
		const char*filePath = new char[MAX_PATH + 3];

		filePath = returnFullPath(path, id);


		std::fstream newGraph(filePath);
		newGraph.open(filePath, std::fstream::out);
        
		if (isDirected)
			newGraph.write("directed",8);
		else
			newGraph.write("undirected",10);

		newGraph.close();


		//IF THERE IS CURRENT GRAPH LOADED IN MEMORY IT IS SAVED TO A FILE AND THEN DELETED FROM HEAP MEMORY
		if (currentGraph)
		{
			currentGraph->saveGraphToAFile(path);
			delete currentGraph;
		}
        
		currentGraph = new Graph(id,filePath, isDirected);
		return;
	}

	std::cout << "The graph " << id << " already exists!\n";
}

void Command::USE_GRAPH(const char * path, const char * id, Graph *&currentGraph)
{
	if (currentGraph)
	{
		if (strcmp(currentGraph->getID(), id) == 0)
		{
			std::cout << "The graph is already loaded!\n";
		}
		else
			std::cout << "Please first save the loaded graph!\n";

	}
	else if(!currentGraph)
	{
		char*filePath = new char[MAX_PATH + 3];


		filePath = returnFullPath(path, id);

		if (searchForGraph(path, id))
		{
			if (currentGraph)
			{
				currentGraph->saveGraphToAFile(path);
				delete currentGraph;
				currentGraph = NULL;
			}
			currentGraph = new Graph(id, filePath);
			currentGraph->loadGraph(filePath, id);
			return;
		}

		std::cout << "Graph " << id << " does not exists!\n";

		delete[]filePath;
	}

}

void Command::DELETE_GRAPH(const char * path, const char * id, Graph *& currenGraph)
{

	//deleting current graph
	if (currenGraph && strcmp(currenGraph->getID(), id) == 0)
	{
		delete currenGraph;
		currenGraph = NULL;
	}
	//if graph file is in the storage
	else if (searchForGraph(path, id))
	{
		char*fullName = returnFullPath(path, id);
		remove(fullName);
		delete[]fullName;
		fullName = NULL;
	}
	else
		std::cout << "Graph " << id << " does not exists\n";
}

void Command::CREATE_NODE(const char * id, Graph *& currentGraph)
{
	//this command works only if there is loadedgraph
	if (currentGraph)
		currentGraph->createNode(id, false);
	else
		std::cout << "Please first create or load any graph!\n";
}

void Command::DELETE_NODE(const char * id, Graph *& currentGraph)
{
	//this command works only if there is loadedgraph
	if (currentGraph)
		currentGraph->deleteNode(id);
	else
		std::cout << "Please first create or load any graph!\n";

	currentGraph->printAllNodesSucc(std::cout);

}

void Command::CREATE_ARC(const char * startNode, const char * endNode, int weight, Graph *& currentGraph)
{
	currentGraph->createArc(startNode, endNode, weight);
}



void Command::DELETE_ARC(const char * startNode, const char * endNode, Graph *& currentGraph)
{
	currentGraph->deleteArc(startNode, endNode);
}



void Command::SAVE(Graph *& currentGraph)
{
	currentGraph->saveGraphToAFile(currentGraph->getFullPath());
}



void Command::SAVE_AS(const char * newName, const char * path, Graph *& currentGraph)
{
	
	currentGraph->setNewFullPath(returnFullPath(path, newName));
    
	SAVE(currentGraph);

}
void Command::CLOSE(Graph *& currentGraph)
{
	char answer;
	std::cout << "Do you want to save current graph? Y/N";
	std::cin >> answer;
	if (answer == 'Y')
	{
		SAVE(currentGraph);
	}
	
	delete currentGraph;
	currentGraph = NULL;
}
/*
void Command::SEARCH(const char * startNode, const char * endNode, const char * algorithm,Graph*&currentGraph)
{
	if (strcmp(algorithm, "bfs"))
		currentGraph->

}*/


void readAndParseCommand(const char*path, const char*commandLine, Graph*&currentGraph)
{
	if (commandLine[0] == 'C' && commandLine[7] == 'G')
	{
		char*id = new char[MAX_PATH];
		bool option = false;

		int inx = 0;
		int i;
		int length = strlen(commandLine);
		for (i = 13; commandLine[i] != ' ' && i < length; ++i)
			id[inx++] = commandLine[i];
		id[inx] = '\0';

		++i;
		if (commandLine[i] == 'd')
			option = true;

		Command::CREATE_GRAPH(path, id, option, currentGraph);

	}

	else if (commandLine[0] == 'U' && commandLine[4] == 'G')
	{

		char*graphID = new char[32];
		int inx = strlen("USE GRAPH "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			graphID[inx2++] = commandLine[i];

		graphID[inx2] = '\0';

		char*fullName = new char[64];
		strcpy_s(fullName, 64, graphID);
		strcat_s(fullName, 64, ".txt");

		Command::USE_GRAPH(path, graphID, currentGraph);



		delete[]graphID;
		graphID = NULL;
		delete[]fullName;
		fullName = NULL;
	}

	else if (commandLine[0] == 'D' && commandLine[7] == 'G')
	{
		char*graphID = new char[32];
		
		int inx = strlen("DELETE GRAPH "), inx2 = 0;
		
		for (int i = inx; commandLine[i] != '\0'; ++i)
			graphID[inx2++] = commandLine[i];
		graphID[inx2] = '\0';
	
		Command::DELETE_GRAPH(path, graphID, currentGraph);
		
		delete[]graphID;
		graphID = NULL;
	}

	else if (commandLine[0] == 'C' && commandLine[7] == 'N')
	{
		char*nodeID = new char[32];

		int inx = strlen("CREATE NODE "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			nodeID[inx2++] = commandLine[i];
		nodeID[inx2] = '\0';

		Command::CREATE_NODE(nodeID, currentGraph);
		delete[]nodeID;
		nodeID = NULL;
	}
	
	else if (commandLine[0] == 'D' && commandLine[7] == 'N')
	{
		char*nodeID = new char[32];

		int inx = strlen("DELETE NODE "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			nodeID[inx2++] = commandLine[i];
		nodeID[inx2] = '\0';

		Command::DELETE_NODE(nodeID, currentGraph);
		delete[]nodeID;
		nodeID = NULL;
	}

	else if (commandLine[0] == 'C' && commandLine[7] == 'A')
	{
		char*nodeId1 = new char[32];
		char*nodeId2 = new char[32];
		char*weight = new char[32];
		int weightNum = 1;

		int inx = strlen("CREATE ARC "), inx2 = 0;

		for (; commandLine[inx] != ' '; ++inx2, ++inx)
			nodeId1[inx2] = commandLine[inx];
		nodeId1[inx2] = '\0';

		++inx;
		inx2 = 0;
		
		int length = strlen(commandLine);

		for (; commandLine[inx] != ' ' && inx <length; ++inx2, ++inx)
			nodeId2[inx2] = commandLine[inx];
		nodeId2[inx2] = '\0';

		
		if (++inx < length)
		{
			inx2 = 0;
			for (; commandLine[inx] != ' ' && inx<length; ++inx2, ++inx)
				weight[inx2] = commandLine[inx];
			weight[inx2] = '\0';
			weightNum = atoi(weight);
		}

		Command::CREATE_ARC(nodeId1, nodeId2, weightNum, currentGraph);
		

		delete[]nodeId1;
		nodeId1 = NULL;
		delete[]nodeId2;
		nodeId2 = NULL;
		if (weight)
		{
			delete[]weight;
			weight = NULL;
		}

	}

	else if (commandLine[0] == 'D' && commandLine[7] == 'A')
	{
		char*nodeId1 = new char[32];
		char*nodeId2 = new char[32];

		int inx = strlen("DELETE ARC "), inx2 = 0;

		for (; commandLine[inx] != ' '; ++inx2, ++inx)
			nodeId1[inx2] = commandLine[inx];
		nodeId1[inx2] = '\0';

		++inx;
		inx2 = 0;

		int length = strlen(commandLine);

		for (; commandLine[inx] != ' ' && inx <length; ++inx2, ++inx)
			nodeId2[inx2] = commandLine[inx];
		nodeId2[inx2] = '\0';


		Command::DELETE_ARC(nodeId1, nodeId2, currentGraph);


		delete[]nodeId1;
		nodeId1 = NULL;
		delete[]nodeId2;
		nodeId2 = NULL;

	}

	else if (commandLine[0] == 'S')
	{
		//SAVE AS COMMAND
		if (commandLine[5] == 'A')
		{
			char*filename = new char[512];
			char*path = new char[1024];

			size_t Inx = 0, i;

			for (i = 8; commandLine[i] != ' '; ++i)
			{
				filename[Inx++] = commandLine[i];
			}

			filename[Inx] = '\0';

			Inx = 0, i = 8 + strlen(filename);

			for (; i < strlen(commandLine) && commandLine[i] != ' '; ++i)
			{
				path[Inx++] = commandLine[i];
			}

			path[Inx] = '\0';

			Command::SAVE_AS(filename, path, currentGraph);
		}
		//SAVE COMMAND
		else if(strcmp(commandLine,"SAVE"))
			Command::SAVE(currentGraph);

	}

	else if (strcmp(commandLine, "CLOSE") == 0)
	{
		Command::CLOSE(currentGraph);
	}
	
	
}





char * returnFullPath(const char * path,const  char * fileName)
{
	int length = strlen(path) + strlen(fileName) + 7;
	char*newPath = new char[length];
	strcpy_s(newPath, length, path);
	strcat_s(newPath, length, "\\");
	strcat_s(newPath, length, fileName);
	strcat_s(newPath, length, ".txt");
	
	return newPath;
}



