#include "Graph.h"
#include <fstream>
#include <string>

//----------------------------------------------
// Public methods of class AdjacencyList
//----------------------------------------------

// Constructor of class Graph
Graph::Graph()
	: adjacencyList(nullptr)
{}

// Destructor of class Graph
Graph::~Graph()
{
	delete adjacencyList;
}

/*
	Reading data from a file
	const std::string nameFile -- the name of the file from which we will read information
*/
void Graph::Parse(const std::string& nameFile)
{
	std::ifstream file(nameFile);				// File for reading

	if (file.is_open())
	{
		if (file.peek() == EOF)
			throw std::domain_error("The file is empty!");
		
		std::string data[3];					// One line of data from a file

		adjacencyList = new AdjacencyList();
		do
		{
			getline(file, data[0], ' ');
			getline(file, data[1], ' ');
			getline(file, data[2]);

			adjacencyList->AddInList(data[0], data[1], std::stoi(data[2]));
		} while (file.peek() != EOF);

		file.close();
	}
	else
		throw std::domain_error("Unable to open file!");
}

// The method displays the generated adjacency list
void Graph::PrintAdjacencyList() const
{
	if (adjacencyList == nullptr)
		throw std::domain_error("The graph is not set!");

	adjacencyList->PrintList();
}

// The method returns the generated adjacency list
std::string* Graph::GetAdjacencyList() const
{
	if (adjacencyList == nullptr)
		throw std::domain_error("The graph is not set!");

	return adjacencyList->GetList();
}

// A method for calculating the maximum network flow using the Ford-Fulkerson algorithm
size_t Graph::MaximumFlow() const
{
	if (adjacencyList == nullptr)
		throw std::domain_error("The graph is not set!");

	return adjacencyList->FordFulkerson();
}
