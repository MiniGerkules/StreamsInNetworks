#pragma once
#include "Adjacency_list.h"

// The class describing the graph
class Graph
{
private:
	AdjacencyList* adjacencyList;				// Adjacency list for a given graph

public:
	// Constructor of class Graph
	Graph();

	// Destructor of class Graph
	~Graph();

	/*
		Reading data from a file
		const std::string nameFile -- the name of the file from which we will read information
	*/
	void Parse(const std::string& nameFile);

	// The method displays the generated adjacency list
	void PrintAdjacencyList() const;

	// The method returns the generated adjacency list
	std::string* GetAdjacencyList() const;

	// A method for calculating the maximum network flow using the Ford-Fulkerson algorithm
	size_t MaximumFlow() const;
};
