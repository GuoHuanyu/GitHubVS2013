#pragma once
#include"Graph.h"
#include<list>
#include<limits>
class Graphl : public Graph {
private:
	std::list<Edge>** vertex; // List headers
	int numVertex, numEdge; // Number of vertices, edges
	int *mark; // Pointer to mark array
	std::list<Edge>::iterator it;
public:
	Graphl(int numVert)
	{
		Init(numVert);
	}
	~Graphl() { // Destructor
		delete[] mark; // Return dynamically allocated memory
		for (int i = 0; i<numVertex; i++) 
			delete vertex[i];
		delete[] vertex;
	}
	void Init(int n) 
	{
		int i;
		numVertex = n;
		numEdge = 0;
		mark = new int[n]; // Initialize mark array
		for (i = 0; i<numVertex; i++) mark[i] = std::numeric_limits<int>::max();
		// Create and initialize adjacency lists
		vertex = (std::list<Edge>**) new std::list<Edge>*[numVertex];
		for (i = 0; i<numVertex; i++)
			vertex[i] = new std::list<Edge>();
	}
	int n() { return numVertex; } // Number of vertices
	int e() { return numEdge; } // Number of edges
	int first(int v) { // Return first neighbor of "v"
		if (vertex[v]->empty())
			return n(); // No neighbor
		it=vertex[v]->begin();
		Edge eit = *it;
		return eit.vertex();
	}
	// Get v’s next neighbor after w
	int next(int v, int w) {
		Edge eit;
		if (isEdge(v, w)) {
			if (++it!= vertex[v]->end())
			{
				eit = *it;
				return eit.vertex();
			}
		}
		return n(); // No neighbor
	}
	// Set edge (i, j) to "weight"
	void setEdge(int i, int j, int weight) {
		Edge currEdge(j, weight);
		if (isEdge(i, j)) 
		{ // Edge already exists in graph
			vertex[i]->remove(*it);
			vertex[i]->push_back(currEdge);
		}
		else 
		{ // Keep neighbors sorted by vertex index
			numEdge++;
			vertex[i]->push_back(currEdge);
		}
	}
	void delEdge(int i, int j) { // Delete edge (i, j)
		if (isEdge(i, j)) {
			vertex[i]->remove(*it);
			numEdge--;
		}
	}
	bool isEdge(int i, int j) { // Is (i,j) an edge?
		Edge eit;
		for (it=vertex[i]->begin();it!=vertex[i]->end();++it) 
		{ // Check whole list
			Edge temp = *it;
			if (temp.vertex() == j) return true;
		}
		return false;
	}
	int weight(int i, int j) { // Return weight of (i, j)
		Edge curr;
		if (isEdge(i, j)) {
			curr = *it;
			return curr.weight();
		}
		else return 0;
	}
	int getMark(int v) { return mark[v]; }
	void setMark(int v, int val) { mark[v] = val; }
};
