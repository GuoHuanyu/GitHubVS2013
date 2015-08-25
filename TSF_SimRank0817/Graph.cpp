#include"Graph.h"
Graph::Graph()
{
}
Graph::Graph(int rows)
{
	this->rows = rows;
	graphArray = new int*[rows];
	for (int i = 0; i <= rows; i++)
		graphArray[i] = new int[rows];
	Rg = 0;
	Rq = 0;
	T = 0;
	sArray = new double[rows];


}

Graph::~Graph()
{
	delete[]graphArray;
	delete[]sArray;
	delete[]tsList;
}
int Graph::initialize(int Rg, int Rq, int T)
{
	this->Rg = Rg;
	this->Rq = Rq;
	this->T = T;
	oneWayGraph = new Gowg[Rg];
	for (int i = 0; i < Rg; i++)
		oneWayGraph[i] = Gowg(rows);
	return 0;
}
int Graph::initialize(int** graph)
{
	graphArray = graph;
	return 0;
}
int Graph::insertEdge(int v, int vo)
{
	if (graphArray[v][vo] == 0)
	{

		graphArray[v][vo] = 1;
		UpdateVector(v);
	}
	else
		std::cout << "Graph insertEdge" << v << "->" << vo << "Error in  Graph::insertEdge" << std::endl;
	return 0;
}
int Graph::deleteEdge(int v, int vo)
{
	if (graphArray[v][vo])
	{
		graphArray[v][vo] = 0;
		UpdateVector(v);
	}
	else
		std::cout << "Graph deleteEdge" << v << "->" << vo << "Error in  Graph::deleteEdge" << std::endl;
	return 0;
}int Graph::deleteVertex(int v)
{
	for (int i = 1; i <= this->rows; i++)
	{
		oneWayGraph[i - 1].deleteVertex(v);
		graphArray[v][i] = 0;
		if (graphArray[i][v])
		{
			graphArray[i][v] = 0;
			UpdateVector(i);
		}
	}
	return 0;
}
int Graph::GenerateithOneWaygraph(int i)
{
	std::vector<int> Noj;
	for (int j = 1; j < rows; j++)
	{
		int NoSize = findNbr(j, Noj);
		std::cout << j << "Vector neighbours：" << NoSize << "  ";

		int randomKey = randomSelect(NoSize);
		std::cout << j << "Vector randomKey：" << randomKey << "  ";
		int Ue = Noj[randomKey];
		oneWayGraph[i].insertEdage(j, Ue);
		std::cout << "insert  " << j << "->" << Ue << std::endl;
		Noj.clear();
	}
	oneWayGraph[i].generateIndexArray();
	return 0;
}
int Graph::findNbr(int ver, std::vector<int>& nbro)
{
	int size = 0;
	nbro.clear();
	for (int j = 1; j < rows; j++)
	{
		if (graphArray[ver][j] == 1)
		{
			nbro.push_back(j);
			size++;
		}
	}
	return size;
}
int Graph::randomSelect(int NoSize)
{
	if (NoSize == 0)
		return 0;
	else
	{
		std::uniform_int_distribution<unsigned>u(0, NoSize - 1);
		static std::default_random_engine e(0000000); //static保证随机下去 而不是每次取随机数第一个值
		return u(e);
	}
}
int Graph::UpdateVector(int v)
{
	std::vector<int> Nov;
	int NoSize = findNbr(v, Nov);
	std::cout << v << "Vector neighbours：" << NoSize << "  ";
	for (int i = 0; i < Rg; i++)
	{
		int randomKey = randomSelect(NoSize);
		std::cout << v << "Vector randomKey：" << randomKey << "  ";
		int Ue = Nov[randomKey];
		oneWayGraph[i].changeEdge(v, Ue);
		oneWayGraph[i].generateIndexArray();
	}
	return 0;
}
int Graph::RandomSample(int v)
{
	//清空上次随机游走结果 
	delete[]tsList;
	tsList = new std::set<int>[rows];
	tsMap.clear();
	//开始新一次随机游走
	std::vector<int> Nov;
	for (int j = 0; j <= Rq - 1; j++)
	{
		int Ue = v;
		for (int i = 1; i <= T; i++)
		{

			//random select a vertex form  No(Ue)
			int NoSize = findNbr(Ue, Nov);
			if (!NoSize)
				break;
			else
			{
				int randomKey = randomSelect(NoSize);
				std::cout << Ue << "Vector neighbours：" << NoSize << "  ";
				std::cout << Ue << "Vector randomKey：" << randomKey << "  ";
				std::cout << "sample random walks " << Ue << "->" << Nov[randomKey] << std::endl;
				Ue = Nov[randomKey];
				//records possible meeting vertices in tsMap
				tsList[Ue].insert(i);
				//records possible meetibng vertices and a list of meeting times
				std::map<int, std::set<int>>::iterator it;
				it = tsMap.find(Ue);
				if (it == tsMap.end())
				{

					tsMap.insert(std::make_pair(Ue, tsList[Ue]));
				}
				else
				{
					it->second = tsList[Ue];
				}
				Nov.clear();
			}
		}
	}
	return 0;
}
//traverse Girowg starting from w
int Graph::updateSimRank(int i, int w)
{
	Gowg oneWayGra = oneWayGraph[i];
	std::set<int>tsw = tsList[w];
	int t = 0;
	//set<int> queset;
	std::set<int> oneStepSet;
	std::set<int> newset;
	//queset.insert(w);
	oneStepSet.insert(w);
	std::set<int>::iterator it; //定义前向迭代器 
	int* indexArray = oneWayGra.GetIndexArray();
	int* nbrArray = oneWayGra.GetNorArray();
	//sort(tsw.begin(), tsw.end());
	while (t < T)
	{
		t++;
		for (it = oneStepSet.begin(); it != oneStepSet.end(); it++)
		{
			int k = *it;
			int temp = indexArray[k] - indexArray[k - 1];
			if (temp>0)
			{

				for (int i = 1; i <= temp; i++)
					newset.insert(nbrArray[indexArray[k - 1] + i]);
			}

		}

		if (find(tsw.begin(), tsw.end(), t) != tsw.end())
		{
			for (it = newset.begin(); it != newset.end(); it++)
			{
				int u = *it;
				sArray[u] = sArray[u] + pow(C, t);
				std::cout << "sArray[" << u << "] +=C^" << t << "   ";
			}
		}
		oneStepSet.clear();
		oneStepSet = newset;
		newset.clear();
	}
	std::cout << std::endl;
	return 0;
}
std::map<int, std::set<int>> Graph::gettsMap()
{
	return tsMap;
}
double* Graph::getsArray()
{
	return sArray;
}