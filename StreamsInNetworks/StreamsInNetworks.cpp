#include "Graph/Graph.h"

int main()
{
    Graph graph;
    std::string nameFile("data5.txt");

    try
    {
        std::cout << "Reading will be done from the file -- " << nameFile << std::endl;
        graph.Parse(nameFile);
        std::cout << "The data was read successfully." << std::endl;

        std::cout << "\nThe following adjacency lists have been generated:" << std::endl;
        graph.PrintAdjacencyList();

        std::cout << "Maximum flow in a given graph = " << graph.MaximumFlow() << std::endl;

        std::cout << "\nThe following adjacency lists after the algorithm runs:" << std::endl;
        graph.PrintAdjacencyList();
    }
    catch (std::domain_error error)
    {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}
