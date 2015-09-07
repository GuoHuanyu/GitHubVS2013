#pragma once
#ifndef TSF_H
#define TSF_H
#include<iostream>
#include<map>
#include<utility>
#include<set>
#include<algorithm>
#include<random>
#include<vector>
#include<ctime>
#include<fstream>
#include<string>
#include<sstream>
#define SEED 1
#define DBUG 0
#define C 0.6
#endif
#include "Gowg.h" 
#include "Graphl.h" 
typedef std::pair<int, double> PAIR;
int cmp(const PAIR& x, const PAIR& y)
{
	return x.second > y.second;
}
int randomSelect(int NoSize)
{
	if (NoSize == 0)
		return 0;
	else
	{
		std::uniform_int_distribution<unsigned>u(0, NoSize - 1);
		static std::default_random_engine e(SEED); //static保证随机下去 而不是每次取随机数第一个值
		return u(e);
	}
}
class TSF
{
private:
	int findNbr(int ver, std::vector<int>& nbro, Graphl& gral);
	int GenerateithOneWaygraph(int i, Graphl& gral, Gowg& gowg);
	std::map<int, std::set<int>> RandomSample(int v, int Rq, int T, Graphl& gral, std::set<int>*tsList);
	//traverse Girowg starting from w
	int updateSimRank(int i, int w, int T, std::set<int>tsw, std::map<int, double> &sArrayMap, Gowg& gowg);
	int UpdateVector(int v, int Rg, Graphl& gral, Gowg* oneWayGraph);
	int N;
	int Rg;
	int Rq;
	int T;
	std::string filepath;
	std::set<int>* tsList;//tsList which store meeting times
	std::map<int, std::set<int>>tsMap;//tsMap
	std::map<int, double>sArrayMap;
	std::map<int, int> topk;
public:

	TSF()
	{
		TSF(0, 0, 0, 0, "D:\\TSFtest");
	}
	TSF(int MaxNodes, int rg, int rq, int t,std::string filph)
	{
		N = MaxNodes;
		Rg = rg;
		Rq = rq;
		T = t;
		filepath = filph;
	}
	~TSF()
	{
		delete[]tsList;
	}
	int deleteEdge(int v, int vo, int Rg, Graphl& gral, Gowg* oneWayGraph);
	int insertEdge(int v, int vo, int Rg, Graphl& gral, Gowg* oneWayGraph);
	int deleteVertex(int v, int Rg, Graphl& gral, Gowg* oneWayGraph);
	int SimRank(std::string outfile, Graphl& gral,int Ue,int Qk)
	{

		std::ofstream fout(outfile);
		std::streambuf *oldbuf = std::cout.rdbuf(fout.rdbuf());
		int startV = Ue;
		for (int i = 0; i < Rg; i++)
		{
			std::vector<int> Nou;
			Gowg oneWayGra = Gowg(N);
			GenerateithOneWaygraph(i, gral, oneWayGra);
			tsList = new std::set<int>[N + 1];
			if (!tsMap.empty())
				tsMap.clear();
			tsMap = RandomSample(startV, Rq, T, gral, tsList);

			std::map<int, std::set<int>>::iterator it = tsMap.begin();
			for (; it != tsMap.end(); ++it)
			{
				if (DBUG)
					std::cout << " Traverse From Vector  " << it->first << "    ";
				updateSimRank(i, it->first, T, tsList[it->first], sArrayMap, oneWayGra);

			}
			std::vector<PAIR> pair_vec(sArrayMap.begin(), sArrayMap.end());
			std::sort(pair_vec.begin(), pair_vec.end(), cmp);
			int s_it_count = 1;

			std::cout << "topk search: " ;
			for (std::vector<PAIR>::iterator s_it = pair_vec.begin(); s_it != pair_vec.end(); ++s_it)
			{
				std::cout << s_it->first << " ";
				std::cout << s_it->second << " ";
				if (s_it_count == Qk)
				{
					std::cout << std::endl;
					break;
				}
				s_it_count++;
			}
			delete[]tsList;
			oneWayGra.storeOneWayGraph(i, filepath);
		}
		fout.close();
		return 0;
	}
	std::map<int, double> getsAmap()
	{
		return sArrayMap;
	}
};
int TSF::findNbr(int ver, std::vector<int>& nbro, Graphl& gral)
{
	int size = 0;
	if (!nbro.empty())
		nbro.clear();
	int vertex;
	vertex = gral.first(ver);
	if (vertex != gral.n())
	{
		nbro.push_back(vertex);
		if (DBUG)
			std::cout << ver << " neighbour: " << vertex << " ";
		size++;
		while (1)
		{
			vertex = gral.next(ver, vertex);
			if (vertex != gral.n())
			{
				if (DBUG)
					std::cout << vertex << " ";
				nbro.push_back(vertex);
				size++;
			}
			else
			{
				if (DBUG)
					std::cout << std::endl;
				break;
			}
		}
	}
	return size;
}
int TSF::GenerateithOneWaygraph(int i, Graphl& gral, Gowg& gowg)
{
	std::vector<int> Noj;
	for (int j = 1; j < gral.n(); j++)
	{
		int NoSize = findNbr(j, Noj, gral);
		if (NoSize == 0)
		{
			if (DBUG)
				std::cout << j << " Vector No neighbours " << std::endl;
			continue;
		}
		if (DBUG)
			std::cout << j << " Vector neighbours is  " << NoSize << "    ";
		int randomKey = randomSelect(NoSize);
		if (DBUG)
			std::cout << j << " Vector randomKey is  " << randomKey << "    ";
		int Ue = Noj[randomKey];
		gowg.insertEdage(j, Ue);
		if (DBUG)
			std::cout << " insert   " << j << " -> " << Ue << std::endl;
		if (!Noj.empty())
			Noj.clear();
	}
	gowg.generateIndexArray();
	return 0;
}
std::map<int, std::set<int>> TSF::RandomSample(int v, int Rq, int T, Graphl& gral, std::set<int>*tsList)
{
	std::map<int, std::set<int>>tsMap;
	//清空上次随机游走结果 
	//delete[]tsList;
	//开始新一次随机游走
	std::vector<int> Nov;
	for (int m = 1; m < Rq; m++)
	{
		int Ue = v;
		for (int n = 1; n <= T; n++)
		{
			//random select a vertex form  No(Ue)
			int NoSize = findNbr(Ue, Nov, gral);
			if (!NoSize)
			{
				if (DBUG)
					std::cout << " random sample  " << n << "  step end in vertex  " << Ue << std::endl;
				break;
			}
			else
			{
				int randomKey = randomSelect(NoSize);
				while (Nov[randomKey] == v)
					randomKey = randomSelect(NoSize);
				if (DBUG)
				{
					std::cout << Ue << " Vector neighbours is  " << NoSize << "    ";
					std::cout << Ue << " Vector randomKey is  " << randomKey << "    ";
					std::cout << " sample random walks  " << Ue << " -> " << Nov[randomKey] << " in " << n << " steps " << std::endl;
				}
				Ue = Nov[randomKey];
				//records possible meeting vertices in tsMap
				tsList[Ue].insert(n);
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
				if (!Nov.empty())
					Nov.clear();
			}
		}
	}
	return tsMap;
}
//traverse Girowg starting from w
int TSF::updateSimRank(int i, int w, int T, std::set<int>tsw, std::map<int, double>& sArrayMap, Gowg& gowg)
{
	int t = 0;
	//set<int> queset;
	std::set<int> oneStepSet;
	std::set<int> newset;
	//queset.insert(w);
	oneStepSet.insert(w);
	std::set<int>::iterator it; //定义前向迭代器 
	int* indexArray = gowg.GetIndexArray();
	int* nbrArray = gowg.GetnbrArray();
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
			it = newset.begin();
			if (it == newset.end())
			{
				if (DBUG)
					std::cout << w << " in step " << t << " no vertex";
			}
			for (; it != newset.end(); it++)
			{
				int u = *it;
				std::map<int, double>::iterator s_it;
				s_it = sArrayMap.find(u);
				if (s_it == sArrayMap.end())
				{
					double sArray = pow(C, t);
					sArrayMap.insert(std::make_pair(u, sArray));

				}
				else
				{
					double sArray = s_it->second + pow(C, t);
					s_it->second = sArray;
				}
				if (DBUG)
					std::cout << " sArray[ " << u << " ] +=C^ " << t << "     ";
			}
		}
		if (!oneStepSet.empty())
			oneStepSet.clear();
		oneStepSet = newset;
		newset.clear();
	}
	if (DBUG)
		std::cout << std::endl;
	return 0;
}
int TSF::deleteEdge(int v, int vo, int Rg, Graphl& gral, Gowg* oneWayGraph)
{
	gral.delEdge(v, vo);
	UpdateVector(v, Rg, gral, oneWayGraph);
	return 0;
}
int TSF::insertEdge(int v, int vo, int Rg, Graphl& gral, Gowg* oneWayGraph)
{
	gral.setEdge(v, vo, 1);
	UpdateVector(v, Rg, gral, oneWayGraph);
	return 0;
}
int TSF::UpdateVector(int v, int Rg, Graphl& gral, Gowg* oneWayGraph)
{
	std::vector<int> Nov;
	int NoSize = findNbr(v, Nov, gral);
	if (DBUG)
		std::cout << v << " Vector neighbours is  " << NoSize << "    ";
	for (int i = 0; i < Rg; i++)
	{
		int randomKey = randomSelect(NoSize);
		if (DBUG)
			std::cout << v << " Vector randomKey is  " << randomKey << "    ";
		int Ue = Nov[randomKey];
		oneWayGraph[i].changeEdge(v, Ue);
		oneWayGraph[i].generateIndexArray();
	}
	return 0;
}
int TSF::deleteVertex(int v, int Rg, Graphl& gral, Gowg* oneWayGraph)
{
	int vertex = gral.first(v);
	gral.delEdge(v, vertex);
	while (1)
	{
		vertex = gral.next(v, vertex);
		if (vertex != gral.n())
		{
			gral.delEdge(v, vertex);
		}
		else
		{
			break;
		}
	}
	for (int i = 1; i < gral.n(); i++)
	{
		gral.delEdge(i, v);
	}
	return 0;
}