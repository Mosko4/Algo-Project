#include "AlgorithmUtil.h"
#include "List.h"

AlgorithmUtil::AlgorithmUtil(int s, int vertices) : d(new Route[vertices]) , p(new int[vertices+1]())
{
	init(s, vertices);
}

AlgorithmUtil::~AlgorithmUtil()
{
	delete[] p;
	delete[] d;
}

bool AlgorithmUtil::relax(int u, int v , float weight)
{
	bool improved;
	if (d[v - 1].Inf == true && d[u - 1].Inf == true) // u and v = INF
		improved = false;
	else if (d[u - 1].Inf == true) // u = INF
		improved = false;
	else if (d[v - 1].Inf == true || (d[v - 1].distance > d[u - 1].distance + weight)) // v = INF or u->v improved distance
	{
		d[v - 1].distance = d[u - 1].distance + weight;
		d[v - 1].Inf = false;
		p[v] = u;
		improved = true;
	}
	else
		improved = false;

	return improved;
}

void AlgorithmUtil::init(int s, int vertices)
{
	for (int i = 0; i < vertices; i++)
	{
		d[i].Inf = true;
	}
	d[s - 1].distance = 0;
	d[s - 1].Inf = false;
}

bool Bellman_Ford(const AdjacencyMatrix& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);

	for (int j = 0; j < NumVertices - 1; j++) // main loop
	{
		for (int i = 1; i < NumVertices; i++)
		{
			List edges;
			graph.GetAdjList(i, edges);
			auto temp = edges.getListHead();
			while (temp) // relax edges from vertex i
			{
				algoUtil.relax(i, temp->vertex, temp->weight);
				temp = temp->_next;
			}
		}
	}

	for (int i = 1; i < NumVertices; i++)
	{
		for (int j = 1; j < NumVertices; j++)
		{
			if (graph.IsAdjacent(i, j) && algoUtil.relax(i, j, graph.getWeight(i, j)))
				return false; // negative cycle
		}
	}

	return true; // success
}

bool Bellman_Ford(const AdjacencyList& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);

	for (int j = 0; j < NumVertices - 1; j++) // main loop
	{
		for (int i = 1; i < NumVertices; i++) 
		{
			auto temp = graph.getListHead(i - 1);
			while (temp)
			{
				algoUtil.relax(i, temp->vertex, temp->weight);
				temp = temp->_next;
			}
		}
	}

	for (int i = 1; i < NumVertices; i++) 
	{
		auto temp = graph.getListHead(i - 1);
		while (temp)
		{
			if (algoUtil.relax(i, temp->vertex, temp->weight))
				return false; // Negative cycle
			temp = temp->_next;
		}
	}
	return true; // success
}

void Dijkstra_Heap(const AdjacencyMatrix& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);
	minHeap queue(NumVertices);
	queue.Build(vertex); // source vertex's key = 0, other vertices' keys = limit

	// search
	while(!queue.isEmpty()) 
	{
		Item u = queue.DeleteMin();
		List u_edges;
		graph.GetAdjList(u.data, u_edges); 
		auto temp = u_edges.getListHead();

		while (temp)
		{
			int v = temp->vertex;
			if (algoUtil.relax(u.data, v, temp->weight)) 
			{
				queue.DecreaseKey(v, algoUtil.d[v - 1].distance); // decrease (v, d[v])
			}
			temp = temp->_next;
		}
	}
}

void Dijkstra_Heap(const AdjacencyList& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);
	minHeap queue(NumVertices);
	queue.Build(vertex); // source vertex's key = 0, other vertices' keys = limit

	// search
	while (!queue.isEmpty())
	{
		Item u = queue.DeleteMin();
		auto u_edges = graph.getListHead(u.data - 1);

		while (u_edges)
		{
			int v = u_edges->vertex;
			if (algoUtil.relax(u.data, v, u_edges->weight))
			{
				queue.DecreaseKey(v, algoUtil.d[v - 1].distance); // decrease (v, d[v])
			}
			u_edges = u_edges->_next;
		}
	}
}

void Dijkstra_Array(const AdjacencyMatrix& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);
	minArray queue(NumVertices);
	queue.build(vertex);

	// search
	while (!queue.isEmpty())
	{
		Item u = queue.DeleteMin();
		List u_edges;
		graph.GetAdjList(u.data, u_edges);
		auto temp = u_edges.getListHead();

		while (temp)
		{
			int v = temp->vertex;
			if (algoUtil.relax(u.data, v, temp->weight))
			{
				queue.DecreaseKey(v, algoUtil.d[v - 1].distance); // decrease (v, d[v])
			}
			temp = temp->_next;
		}
	}
}

void Dijkstra_Array(const AdjacencyList& graph, int vertex, AlgorithmUtil& algoUtil)
{
	// init
	int NumVertices = graph.getNumVertices();
	algoUtil.init(vertex, NumVertices);
	minArray queue(NumVertices);
	queue.build(vertex); 

	// search
	while (!queue.isEmpty())
	{
		Item u = queue.DeleteMin();
		auto u_edges = graph.getListHead(u.data - 1);

		while (u_edges)
		{
			int v = u_edges->vertex;
			if (algoUtil.relax(u.data, v, u_edges->weight))
			{
				queue.DecreaseKey(v, algoUtil.d[v - 1].distance); // decrease (v, d[v])
			}
			u_edges = u_edges->_next;
		}
	}
}


bool getVertexFile(string& str, int& vertex)
{
	bool validInput = true;
	string numStr;
	int indexBegin = 0, indexEnd;
	indexEnd = str.find_first_of(' ', indexBegin);

	for (int i = indexBegin; i < indexEnd; i++)
	{
		if (str[i] > '9' || str[i] < '0')
		{
			validInput = false;
			break;
		}
		else
		{
			numStr.push_back(str[i]);
		}
	}

	str = string(str.c_str() + indexEnd + 1); // string str = string(char *str + indexBegin)
	vertex = atoi(numStr.c_str());
	return validInput;

}

bool getWeightFile(string& str, float& weight)
{
	bool validInput = true, firstDecimalPoint = false;
	string numStr;
	int index = str.find_first_of(' ');
	if (index != string::npos) // if there's a space after the weight number we make sure the rest of the line is only whitespace
	{
		for (int i = index; i < str.length() && validInput; i++)
		{
			if (!isspace(str[i]))
				validInput = false;
		}
	}

	if (validInput)
	{
		for (auto& itr : str)
		{
			if (itr == '.')
			{
				if (firstDecimalPoint)
					validInput = false;
				else
					firstDecimalPoint = true;
			}
			else if (itr > '9' || itr < '0')
				validInput = false;

			if (!validInput)
				break;
		}

		weight = stof(str, nullptr);
	}


	return validInput;
}

void getEdges(fstream& file, AdjacencyList& adjList, AdjacencyMatrix& adjMatrix)
{
	string currentRead;
	int vertices = adjMatrix.getNumVertices(), vertex_i, vertex_j;
	float weight;
	bool valid1, valid2, valid3;




	/* read from file to get edges and their weight
	   assuming format: number-space-number-space-number as a valid input
	                       ^           ^            ^
	                       |           |            |
	                    vertex_i    vertex_j      weight
	*/
	while (!file.eof())
	{
		getline(file, currentRead);
		if (currentRead == "") // assuming blank lines = end of file
			break;

		/*valid1 = getVertexFile(currentRead, vertex_i);
		valid2 = getVertexFile(currentRead, vertex_j);
		valid3 = getWeightFile(currentRead, weight);*/
		char buffer[MAX_SIZE];
		buffer[0] = '\0';

		// we use sscanf to parse the line from file into the variables. if a non-empty string is read after the weight
		// or no. of variables parsed is not 3, there is a non-valid input.
		int n = sscanf(currentRead.c_str(), "%d %d %f%s", &vertex_i, &vertex_j, &weight, buffer);
		if (n != 3 || string(buffer) != "" || vertex_i == vertex_j || vertex_i > vertices
			|| vertex_j > vertices || adjMatrix.IsAdjacent(vertex_i, vertex_j))
		{
			invalid_input

		}

		/*if (!valid1 || !valid2 || !valid3 ||
			vertex_i == vertex_j || vertex_i > vertices
			|| vertex_j > vertices || adjMatrix.IsAdjacent(vertex_i, vertex_j))
		{
			invalid_input
		}*/

		adjList.AddEdge(vertex_i, vertex_j, weight);
		adjMatrix.AddEdge(vertex_i, vertex_j, weight);
	}
}

int checkLine(string& currentRead)
{
	string numberStr;
	for (auto& itr : currentRead)
	{
		if (itr >= '0' && itr <= '9') {
			numberStr.push_back(itr);
		}
		else if (itr != '\n' && itr != '\0')
		{
			invalid_input
		}
	}

	return stoi(numberStr, nullptr, 10);
}

void getVertices(fstream& file, int& vertices, int& source, int& dest)
{
	string currentRead;

	// get no. of vertices, source and destination vertex
	getline(file, currentRead);
	vertices = checkLine(currentRead);
	getline(file, currentRead);
	source = checkLine(currentRead);
	getline(file, currentRead);
	dest = checkLine(currentRead);
	if (source > vertices || vertices < 1 || source < 1 || dest < 1 || dest > vertices)
	{
		invalid_input
	}
}
