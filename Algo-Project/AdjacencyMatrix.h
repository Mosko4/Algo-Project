#pragma once
#include <limits>
#include "List.h"
#include "Item.h"
#include <iostream>
constexpr int INF = -1;

class AdjacencyMatrix
{
private:

	const int m_vertices;
	float **m_adjMatrix = nullptr;

public:

	AdjacencyMatrix() = delete;
	AdjacencyMatrix(const AdjacencyMatrix& other) = delete;
	AdjacencyMatrix(int vertices);
	~AdjacencyMatrix();

	void MakeEmptyGraph(int vertices);
	bool IsAdjacent(int u, int v) const;
	void AddEdge(int u, int v, float weight);
	void RemoveEdge(int u, int v);
	void GetAdjList(int u, List& list) const;
	int getNumVertices() const { return m_vertices; }
	float getWeight(int u, int v)const;
	Item* BuildItemArray();
	// debugging
	void printMatrix();

};

