#include "Command.h"


int main(int argc,char*argv[])

{
	char*command = new char[1024];

	Graph*graph = NULL;

	while (std::cin.getline(command, 1024, '\n'))
	{
		readAndParseCommand(argv[1], command,graph);
		graph->printAllNodesSucc(std::cout);
	}
	



	delete[]command;
	command = NULL;
	return 0;
}