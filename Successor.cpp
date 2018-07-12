#include "Successor.h"

Successor::Successor(const char * id, int w)
{
	int idLength = strlen(id) + 1;
	this->succID = new char[idLength];
	strcpy_s(succID, idLength, id);

	weight = w;

	nextSucc = NULL;

}

Successor::~Successor()
{
	if (succID)
	{
		delete[]succID;
	}
	succID = NULL;
}

void Successor::print(std::ostream&os)
{
	os << succID;
	if (weight != -1)
		os << ' ' << weight;

}
