#pragma once
#include<sstream>
class Gowg //one out-degree store revered Gowg
{
public:
	Gowg();
	Gowg(int);
	~Gowg();
	int insertEdage(int v, int vo);
	int generateIndexArray();
	int* GetIndexArray();
	int* GetnbrArray();
	int clear();
	int changeEdge(int v, int vo);
	int storeOneWayGraph(int i, std::string filepath);
private:
	int* indexArray;
	int* nbrArray;
	int* GraphArray;
	int* NoDegree;
	int rows;
	int* LogOfEdge;
	std::set<int> LogOfDelete;
	std::set<int> LogOfInsert;
};

Gowg::Gowg()
{
	Gowg(0);
}
Gowg::Gowg(int rows)
{
	this->rows = rows + 1;
	indexArray = new int[this->rows];
	nbrArray = new int[this->rows];
	GraphArray = new int[this->rows];
	NoDegree = new int[this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		indexArray[i] = 0;
		NoDegree[i] = 0;
		nbrArray[i] = 0;
		GraphArray[i] = 0;
	}
	LogOfEdge = new int[this->rows];
	LogOfInsert.clear();
	LogOfDelete.clear();
}

Gowg::~Gowg()
{

}


int Gowg::insertEdage(int v, int vo)
{
	if (GraphArray[v] == 0)
		GraphArray[v] = vo;
	else
		std::cout << " one way Graph insert Error " << std::endl;
	return 0;
}


int Gowg::generateIndexArray()
{
	for (int i = 1; i < rows; i++)
	{
		if (GraphArray[i] != 0)
			NoDegree[GraphArray[i]]++;
	}
	for (int i = 1; i < rows; i++)
	{
		indexArray[i] = indexArray[i - 1] + NoDegree[i];
	}
	int k = 1;
	for (int i = 1; i < rows; i++)
	{
		int temp = indexArray[i] - indexArray[i - 1];
		while (temp)
		{
			for (int j = 1; j < rows; j++)
			{
				if (GraphArray[j] == i)
				{
					nbrArray[k++] = j;
					temp--;
				}
				if (temp&&j == rows)
				{
					std::cout << " Error in generateIndexArray " << std::endl;
				}
			}
		}
	}
	return 0;
}


int* Gowg::GetIndexArray()
{
	return indexArray;
}


int* Gowg::GetnbrArray()
{
	return nbrArray;
}


int Gowg::clear()
{
	memset(indexArray, 0, sizeof(indexArray));
	memset(NoDegree, 0, sizeof(NoDegree));
	memset(nbrArray, 0, sizeof(nbrArray));
	return 0;
}


int Gowg::changeEdge(int v, int vo)
{
	if (GraphArray[v] != 0)
		GraphArray[v] = vo;
	else
		std::cout << " one way Graph Vector  " << v << "  change Edge Error " << std::endl;
	LogOfEdge[v] = vo;
	return 0;
}
int Gowg::storeOneWayGraph(int i,std::string filepath)
{
	std::stringstream sstr;
	sstr << filepath << "oneWayGraph_" << i << ".txt";
	std::string outfile = sstr.str();
	std::ofstream fout(outfile);
	for (int i = 0; i < rows; i++)
	{
		fout << indexArray[i]<<" ";
	}
	fout << std::endl;
	for (int i = 0; i < rows; i++)
	{
		fout << nbrArray[i]<<" ";
	}
	fout.close();
	return 0;
}

