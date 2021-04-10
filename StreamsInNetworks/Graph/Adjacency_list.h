#pragma once
#include <iostream>
#include <string>

enum FlowDirection
{
	againstTheDirection = -1,
	undefined, 
	inTheDirection
};

// Adjacency list class
class AdjacencyList
{
private:
	// Class describing an adjacency list item for one vertex
	class Node
	{
	public:
		Node* next;								// Pointer to next Node
		const std::string name;					// Vertex name
		size_t capacity;						// The capacity of an edge entering a given vertex
		size_t stream;							// The value of the flow flowing along the edge entering the given vertex

		/*
			Node class constructor
			Node* next_ -- pointer to next Node
			std::string name_ -- vertex name
			size_t capacity_-- the capacity of an edge entering a given vertex
			size_t stream_ -- the value of the flow flowing along the edge entering the given vertex
		*/
		Node(Node* next_, std::string name_, size_t capacity_, size_t stream_)
			:  next(next_), name(std::move(name_)), capacity(capacity_), stream(stream_)
		{}
	};

	// Class describing an element of the adjacency list of major vertices
	class MainNode : public Node
	{
	public:
		MainNode* mainNext;						// Pointer to next MainNode
		Node* adressInSideList;					// Address where you came from
		std::string comeFrom;					// The name of the vertex from which we came
		std::string incomingEdges;				// A string containing all input edges
		size_t numberOfIncoming;				// The number of incoming edges for a given vertex
		short direction;						// Direction of movement through the top

		/*
			MainNode class constructor
			Node* mainNext_ -- pointer to next MainNode
			Node* next_ -- pointer to next Node
			std::string name_ -- vertex name
		*/
		MainNode(MainNode* mainNext_, Node* next_, std::string name_)
			: Node(next_, name_, 0, 0), mainNext(mainNext_), adressInSideList(nullptr), comeFrom(""),
				direction(undefined), incomingEdges(""), numberOfIncoming(0)
		{}
	};

	MainNode* head;							// Pointer to the head of an adjacency list
	MainNode* source;						// Pointer to the source of the network
	MainNode* runoff;						// Pointer to the network drain
	size_t flow;							// Flow in a given graph
	bool calculatedFlow;					// Flag showing whether the stream was previously calculated in this network

	/*
		Method that divides a string by a given character
		const std::string& str -- string to be split
		char sep -- separator character
		size_t number -- number of separators
	*/
	std::string* Split(const std::string& str, const char sep, const size_t number) const;

	/*
		A method that returns a pointer to the desired element of the adjacency list
		const std::string& toFind -- the name of the vertex to find in the adjacency list
	*/
	MainNode* Find(const std::string& toFind) const;

	// The method initializes the fields of the elements of the adjacency lists to work
	void Init();

	/*
		A method that finds an incoming edge at a top of interest
		const MainNode* const way -- current top
		const std::string& name -- the name of the vertex from which we are looking for an edge to the current
	*/
	Node* FindIncomingEdge(const MainNode* const way, const std::string& name) const;

	/*
		Method that finds the top with the maximum available flow that can be navigated to
		MainNode* way -- the top from which we are looking for a way
	*/
	MainNode* FindTopWithMaxAvailableFlow(const MainNode* const way) const;

	/*
		The first step of the Ford-Fulkerson algorithm
		MainNode* way -- vertex indicating the beginning of the path
	*/
	void FirstStep(MainNode* way);

	/*
		The second step of the Ford-Fulkerson algorithm
		MainNode* way -- the top from which we are looking for a way
	*/
	void SecondStep(MainNode* way);

	/*
		The third step of the Ford-Fulkerson algorithm
		MainNode* way -- the top from which we are looking for a way
	*/
	void ThirdStep(MainNode* way);

	// The fourth step of the Ford-Fulkerson algorithm
	void FourthStep();

public:
	// Ñonstructor of class AdjacencyList
	AdjacencyList();

	// Destructor of class AdjacencyList
	~AdjacencyList();
	
	/*
		Method adding a vertex to a graph
		const std::string& mainName -- the top from which the edge comes
		const std::string name -- the name of the added vertex
		const size_t capacity -- the capacity of an edge entering a given vertex
	*/
	void AddInList(const std::string& mainName, const std::string& name, const size_t capacity);

	// The method displays the generated adjacency list
	void PrintList() const;

	// The method returns the generated adjacency list
	std::string* GetList() const;

	// The method for calculating the maximum flow using the Ford-Fulkerson method
	size_t FordFulkerson();
};
