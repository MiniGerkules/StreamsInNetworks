#include "../StreamsInNetworks/Graph/Graph.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(ParseTest)
		{
			Graph* graph = new Graph();
			std::string absResult[] = { "S: O 3 0, P 3 0", "O: Q 3 0, P 2 0", "P: R 2 0", "Q: R 4 0, T 2 0", "R: T 3 0", "T" };
			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data1.txt");
			std::string* methodResult = graph->GetAdjacencyList();
			for (size_t i = 0; i < 6; ++i)
				Assert::AreEqual(absResult[i], methodResult[i]);

			delete graph;
		}

		TEST_METHOD(AddInListTest1)
		{
			AdjacencyList* list = new AdjacencyList();
			std::string absResult[] = { "1: 2 5 0", "2: 1 5 0", "4: 5 3 0", "5", "3: 2 1 0" };

			list->AddInList("1", "2", 5);
			list->AddInList("2", "1", 5);
			list->AddInList("4", "5", 3);
			list->AddInList("3", "2", 1);

			std::string* methodResult = list->GetList();
			for (size_t i = 0; i < 5; ++i)
				Assert::AreEqual(methodResult[i], absResult[i]);

			delete list;
		}

		TEST_METHOD(AddInListTest2)
		{
			AdjacencyList* list = new AdjacencyList();

			list->AddInList("7", "3", 3);
			std::string* methodResult = list->GetList();
			Assert::AreEqual((std::string)"7: 3 3 0", methodResult[0]);

			list->AddInList("7", "4", 4);
			methodResult = list->GetList();
			Assert::AreEqual((std::string)"7: 3 3 0, 4 4 0", methodResult[0]);

			list->AddInList("7", "5", 5);
			methodResult = list->GetList();
			Assert::AreEqual((std::string)"7: 3 3 0, 4 4 0, 5 5 0", methodResult[0]);

			list->AddInList("3", "2", 6);
			methodResult = list->GetList();
			Assert::AreEqual((std::string)"3: 2 6 0", methodResult[1]);

			list->AddInList("3", "3", 6);
			methodResult = list->GetList();
			Assert::AreEqual((std::string)"3: 2 6 0, 3 6 0", methodResult[1]);
			Assert::AreEqual((std::string)"4", methodResult[2]);

			delete list;
		}

		TEST_METHOD(GetAdjacencyListTest1)
		{
			Graph* graph = new Graph();

			try
			{
				graph->GetAdjacencyList();
			}
			catch (std::domain_error error)
			{
				Assert::AreEqual("The graph is not set!", error.what());
			}

			delete graph;
		}

		TEST_METHOD(GetAdjacencyListTest2)
		{
			Graph* graph = new Graph();
			std::string absResult[] = { "S: O 3 0, P 3 0", "O: Q 3 0, P 2 0", "P: R 2 0", "Q: R 4 0, T 2 0", "R: T 3 0", "T" };

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data1.txt");
			std::string* functionResult = graph->GetAdjacencyList();

			for (size_t i = 0; i < 2; ++i)
				Assert::AreEqual(absResult[i], functionResult[i]);

			delete graph;
		}

		TEST_METHOD(GetListTest)
		{
			AdjacencyList* list = new AdjacencyList();

			try
			{
				list->GetList();
			}
			catch (std::domain_error error)
			{
				Assert::AreEqual("There are no items in the graph!", error.what());
			}

			delete list;
		}

		TEST_METHOD(MaximumFlowTestUncorrectData)
		{
			Graph* graph = new Graph();
			
			try
			{
				graph->MaximumFlow();
			}
			catch (std::domain_error error)
			{
				Assert::AreEqual("The graph is not set!", error.what());
			}

			delete graph;
		}

		TEST_METHOD(MaximumFlowTest1)
		{
			Graph* graph = new Graph();

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data1.txt");
			Assert::AreEqual((size_t)5, graph->MaximumFlow());

			delete graph;
		}

		TEST_METHOD(MaximumFlowTest2)
		{
			Graph* graph = new Graph();

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data2.txt");
			Assert::AreEqual((size_t)19, graph->MaximumFlow());

			delete graph;
		}

		TEST_METHOD(MaximumFlowTest3)
		{
			Graph* graph = new Graph();

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data3.txt");
			Assert::AreEqual((size_t)33, graph->MaximumFlow());

			delete graph;
		}

		TEST_METHOD(MaximumFlowTest4)
		{
			Graph* graph = new Graph();

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data4.txt");
			Assert::AreEqual((size_t)14, graph->MaximumFlow());

			delete graph;
		}

		TEST_METHOD(MaximumFlowTest5)
		{
			Graph* graph = new Graph();

			graph->Parse("C:\\Users\\Евгений\\OneDrive\\Рабочий стол\\Лабораторные работы\\4 семестр\\АиСД\\kurs\\StreamsInNetworks\\StreamsInNetworks\\data5.txt");
			Assert::AreEqual((size_t)31, graph->MaximumFlow());

			delete graph;
		}
	};
}
