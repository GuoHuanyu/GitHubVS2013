#include"Gowg.h"
#include "TSF.h"
#pragma once
class Graph
{
public:
	Graph();
	Graph(int rows);
	~Graph();
	int initialize(int Rg, int Rq, int T);
	int initialize(int **graph);
	int	deleteVertex(int v);
	int	deleteEdge(int v, int vo);
	int	insertEdge(int v, int vo);
	int GenerateithOneWaygraph(int i);
	int findNbr(int ver, std::vector<int>& nbro);
	int randomSelect(int NoSize);
	int UpdateVector(int v);
	int updateSimRank(int i, int w);
	int RandomSample(int v);
	std::map<int, std::set<int>> gettsMap();
	double* getsArray();
private:
	int** graphArray;
	int rows;
	int Rg;
	int T;
	int Rq;
	Gowg* oneWayGraph;
	double* sArray;
	std::set<int>* tsList;//tsList which store meeting times
	std::map<int, std::set<int>>tsMap;//tsMap
};

