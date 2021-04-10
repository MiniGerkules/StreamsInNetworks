#include "Adjacency_list.h"

//----------------------------------------------
// Private methods of class AdjacencyList
//----------------------------------------------

/*
	Method that divides a string by a given character
	const std::string& str -- string to be split
	char sep -- separator character
	size_t number -- number of separators
*/
std::string* AdjacencyList::Split(const std::string& str, const char sep, const size_t number) const
{
	std::string* result = nullptr;				// The resulting array of strings
	size_t index = 0;							// The current index of the element of the array of strings

	if (number != 0)
	{
		result = new std::string[number];

		for (size_t i = 0, j = 0; i < str.length(); ++i)
			if (str[i] == sep)
			{
				result[index++] = str.substr(j, i - j);
				j = i + 1;
			}
	}

	return result;
}

/*
	A method that returns a pointer to the desired element of the adjacency list
	const std::string& toFind -- the name of the vertex to find in the adjacency list
*/
AdjacencyList::MainNode* AdjacencyList::Find(const std::string& toFind) const
{
	MainNode* current = head;					// The pointer with which we go through the main list

	while (true)
	{
		if (current == nullptr)
			return nullptr;
		else if (current->name == toFind)
			return current;
		else
			current = current->mainNext;
	}
}

// The method initializes the fields of the elements of the adjacency lists to work
void AdjacencyList::Init()
{
	MainNode* mainCurrent = head;

	while (mainCurrent != nullptr)
	{
		mainCurrent->adressInSideList = nullptr;
		mainCurrent->comeFrom.clear();
		mainCurrent->direction = undefined;

		mainCurrent = mainCurrent->mainNext;
	}
}

/*
	A method that finds an incoming edge at a top of interest
	MainNode* way -- current top
	const std::string& name -- the name of the vertex from which we are looking for an edge to the current
*/
AdjacencyList::Node* AdjacencyList::FindIncomingEdge(const MainNode* const way, const std::string& name) const
{
	MainNode* inputTop;							// The vertex from which the edge comes out to our vertex
	Node* current;								// The vertex that contains the edge we need

	inputTop = Find(name);
	if (inputTop->comeFrom != "")
		return nullptr;

	current = inputTop->next;
	while (current->name != way->name)
		current = current->next;

	return current;
}

/*
	Method that finds the top with the maximum available flow that can be navigated to
	MainNode* way -- the top from which we are looking for a way
*/
AdjacencyList::MainNode* AdjacencyList::FindTopWithMaxAvailableFlow(const MainNode* const way) const
{
	MainNode* mainCurrent;						// The top in the main list, which we go to
	Node* current;								// The pointer with which we are looking for a vertex available for transition
	Node* maxFlow = nullptr;					// The top with the maximum available flow
	std::string* input;							// An array of vertices from which there are edges to our vertices
	std::string name;							// The name of the vertex to which we will go
	short direction;							// Flow direction

	input = Split(way->incomingEdges, ';', way->numberOfIncoming);

	// Determine the vertex we can go to
	current = way->next;
	while (true)
	{
		if (current == nullptr)
			break;
		else if ((Find(current->name)->comeFrom == "") && (current->capacity > 0))
		{
			maxFlow = current;
			direction = inTheDirection;
			name = current->name;
			break;
		}
		else
			current = current->next;
	}

	if (maxFlow == nullptr)
	{
		size_t index;

		for (size_t i = 0; i < way->numberOfIncoming; ++i)
		{
			current = FindIncomingEdge(way, input[i]);
			if (current == nullptr) 
				continue;

			if (current->stream > 0)
			{
				maxFlow = current;
				direction = againstTheDirection;
				name = input[i];
				index = i + 1;
				break;
			}
		}

		if (maxFlow == nullptr)
			return nullptr;

		// It makes sense to go only to the incoming edges
		for (size_t i = index; i < way->numberOfIncoming; ++i)
		{
			current = FindIncomingEdge(way, input[i]);
			if (current == nullptr)
				continue;

			if (current->stream > maxFlow->stream)
			{
				maxFlow = current;
				name = input[i];
			}
		}
	}
	else
	{
		// It makes sense to go on all the edges
		current = maxFlow->next;
		while (current != nullptr)
		{
			if ((Find(current->name)->comeFrom == "") && (current->capacity > maxFlow->capacity))
			{
				maxFlow = current;
				name = current->name;
			}

			current = current->next;
		}

		for (size_t i = 0; i < way->numberOfIncoming; ++i)
		{
			current = FindIncomingEdge(way, input[i]);
			if (current == nullptr)
				continue;

			if (direction == inTheDirection)
			{
				if (current->stream > maxFlow->capacity)
				{
					maxFlow = current;
					direction = againstTheDirection;
					name = input[i];
				}
			}
			else
				// This means that direction == againstTheDirection. There is no need to reset the direction value
				if (current->stream > maxFlow->stream)
				{
					maxFlow = current;
					name = input[i];
				}
		}
	}

	// We enter all the information into the vertex to which we will go
	mainCurrent = Find(name);
	mainCurrent->comeFrom = way->name;
	mainCurrent->direction = direction;
	mainCurrent->adressInSideList = maxFlow;

	return mainCurrent;
}

/*
	The first step of the Ford-Fulkerson algorithm
	MainNode* way -- vertex indicating the beginning of the path
*/
void AdjacencyList::FirstStep(MainNode* way)
{
	// Remove labels from vertices before iteration
	Init();
	// We start the path from the source
	way = source;
	way->comeFrom = "-";

	SecondStep(way);
}

/*
	The second step of the Ford-Fulkerson algorithm
	MainNode* way -- vertex indicating the beginning of the path
*/
void AdjacencyList::SecondStep(MainNode* way)
{
	MainNode* maxFlow = nullptr;					// The top with the maximum available flow

	maxFlow = FindTopWithMaxAvailableFlow(way);
	if (maxFlow == nullptr)
		ThirdStep(way);

	// If the node with the maximum bandwidth is not a sink, repeat step 2
	else if (maxFlow != runoff)
		SecondStep(maxFlow);
	// Otherwise, we recalculate the flows
	else
		FourthStep();
}

/*
	The third step of the Ford-Fulkerson algorithm
	MainNode* way -- vertex indicating the beginning of the path
*/
void AdjacencyList::ThirdStep(MainNode* way)
{
	if (way != source)
	{
		MainNode* newWay = Find(way->comeFrom);
		way->comeFrom = "Don't use";
		SecondStep(newWay);
	}
}

// The fourth step of the Ford-Fulkerson algorithm
void AdjacencyList::FourthStep()
{
	MainNode* mainCurrent;					// The pointer with which we go through the main list
	size_t minFlow;							// Minimum flow through the chosen path

	// Find the minimum flow through the edges
	minFlow = runoff->adressInSideList->capacity;
	mainCurrent = Find(runoff->comeFrom);
	while (mainCurrent != source)
	{
		if ((mainCurrent->direction == inTheDirection) && (mainCurrent->adressInSideList->capacity < minFlow))
			minFlow = mainCurrent->adressInSideList->capacity;
		else if ((mainCurrent->direction == againstTheDirection) && (mainCurrent->adressInSideList->stream < minFlow))
			minFlow = mainCurrent->adressInSideList->stream;

		mainCurrent = Find(mainCurrent->comeFrom);
	}

	// Increasing or decreasing flows through the edges
	mainCurrent = runoff;
	while (mainCurrent != source)
	{
		if (mainCurrent->direction == inTheDirection)
		{
			mainCurrent->adressInSideList->stream += minFlow;
			mainCurrent->adressInSideList->capacity -= minFlow;
		}
		else
		{
			mainCurrent->adressInSideList->stream -= minFlow;
			mainCurrent->adressInSideList->capacity += minFlow;
		}

		mainCurrent = Find(mainCurrent->comeFrom);
	}

	flow += minFlow;
	FirstStep(source);
}

//----------------------------------------------
// Public methods of class AdjacencyList
//----------------------------------------------

// Ñonstructor of class AdjacencyList
AdjacencyList::AdjacencyList()
	: head(nullptr), source(nullptr), runoff(nullptr), flow(0), calculatedFlow(false)
{}

// Destructor of class AdjacencyList
AdjacencyList::~AdjacencyList()
{
	while (head != nullptr)
	{
		Node* current = head;				// The pointer with which we go through the side list
		Node* deleted;						// The pointer with which to remove elements

		head = head->mainNext;
		// In the first iteration, current == head, so it is guaranteed
		do
		{
			deleted = current;
			current = current->next;
			delete deleted;
		} while (current != nullptr);
	}
}

/*
	Method adding a vertex to a graph
	const std::string& mainName -- the top from which the edge comes
	const std::string name -- the name of the added vertex
	const size_t capacity -- the capacity of an edge entering a given vertex
*/
void AdjacencyList::AddInList(const std::string& mainName, const std::string& name, const size_t capacity)
{
	MainNode* mainCurrent;							// Pointer to the element of the main list we need

	// Adding output edge from mainName
	mainCurrent = Find(mainName);
	if (mainCurrent != nullptr)
	{
		Node* current = mainCurrent;				// The pointer with which we go through the side list

		if (current->next != nullptr)
			do
			{
				current = current->next;
			} while (current->next != nullptr);

		current->next = new Node(nullptr, name, capacity, 0);
	}
	else
	{
		if (head == nullptr)
		{
			head = new MainNode(nullptr, nullptr, mainName);
			head->next = new Node(nullptr, name, capacity, 0);

			if (mainName == "S")
				source = head;
			else if (mainName == "T")
				runoff = head;
		}
		else
		{
			mainCurrent = head;
			while (mainCurrent->mainNext != nullptr)
				mainCurrent = mainCurrent->mainNext;

			mainCurrent->mainNext = new MainNode(nullptr, nullptr, mainName);
			mainCurrent->mainNext->next = new Node(nullptr, name, capacity, 0);

			if (mainName == "S")
				source = mainCurrent;
			else if (mainName == "T")
				runoff = mainCurrent;
		}
	}

	// Add an input edge to name
	mainCurrent = Find(name);
	if (mainCurrent == nullptr)
	{
		mainCurrent = head;
		while (mainCurrent->mainNext != nullptr)
			mainCurrent = mainCurrent->mainNext;

		mainCurrent->mainNext = new MainNode(nullptr, nullptr, name);
		mainCurrent = mainCurrent->mainNext;

		if (name == "S")
			source = mainCurrent;
		else if (name == "T")
			runoff = mainCurrent;
	}

	mainCurrent->incomingEdges += mainName + ";";
	++mainCurrent->numberOfIncoming;
}

// The method displays the generated adjacency list
void AdjacencyList::PrintList() const
{
	MainNode* mainCurrent;						// The pointer with which we go through the main list
	Node* current;								// The pointer with which we go through the side list

	if (head == nullptr)
		throw std::domain_error("There are no items in the graph!");

	mainCurrent = head;
	do
	{
		if (mainCurrent != runoff)
		{
			std::cout << "There are routes from " << mainCurrent->name << " to:" << std::endl;
			current = mainCurrent->next;
			while (current != nullptr)
			{
				std::cout << " * " << current->name << ": capacity -- " << current->capacity << ", stream -- " 
					<< current->stream << std::endl;
				current = current->next;
			}
			std::cout << std::endl;
		}
		mainCurrent = mainCurrent->mainNext;
	} while (mainCurrent != nullptr);
}

// The method returns the generated adjacency list
std::string* AdjacencyList::GetList() const
{
	MainNode* mainCurrent = head;				// The pointer with which we go through the main list
	Node* current;								// The pointer with which we go through the side list
	std::string* result;						// The string into which we write adjacency lists
	size_t numberOfTops = 0;					// Number of nodes in the graph

	if (head == nullptr)
		throw std::domain_error("There are no items in the graph!");

	// Counting nodes in the graph
	while (mainCurrent != nullptr)
	{
		++numberOfTops;
		mainCurrent = mainCurrent->mainNext;
	}

	result = new std::string[numberOfTops];
	mainCurrent = head;
	for (size_t i = 0; i < numberOfTops; ++i)
	{
		result[i] = mainCurrent->name + ": ";
		current = mainCurrent->next;
		while (current != nullptr)
		{
			result[i] += current->name + " " + std::to_string(current->capacity) + " " + std::to_string(current->stream) + ", ";
			current = current->next;
		}
		result[i].erase(result[i].length() - 2, 2);

		mainCurrent = mainCurrent->mainNext;
	}

	return result;
}

// The method for calculating the maximum flow using the Ford-Fulkerson method
size_t AdjacencyList::FordFulkerson()
{
	if (head == nullptr)
		throw std::domain_error("There are no items in the graph!");

	if (calculatedFlow)
		return flow;

	FirstStep(source);
	calculatedFlow = true;

	return flow;
}
