#pragma once
#ifndef SUCCESSOR_H
#define SUCCESSOR_H


#include <iostream>
#include <list>
#include <utility>

struct Successor
{

	char *succID;
	int weight;
	Successor*nextSucc;


	Successor(const char*id, int w = -1);
	Successor(const Successor&other) = delete;
	Successor&operator=(const Successor&other) = delete;
	~Successor();

	void print(std::ostream&);
};



#endif