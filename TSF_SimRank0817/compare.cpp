#include"TSF.h"
//read graph from txt file
int readfile(Graphl &gra, std::string filename)
{
	std::ifstream fin(filename);
	if (!fin.good())
		return 0;
	std::string line;
	int a, b;
	while (!fin.eof())
	{
		getline(fin, line);
		auto it = line.begin();
		if (*it != '#')
			break;
	}
	std::stringstream strstrm(line);
	strstrm >> a;
	strstrm >> b;
	gra.setEdge(a, b, 1);
	while (!fin.eof())
	{
		fin >> a;
		fin >> b;
		gra.setEdge(a, b, 1);

	}
	fin.close();
	return 0;
}
//display Graph's Adjacency list
int display(Graphl &gra)
{
	for (int i = 1; i < gra.n(); i++)
	{
		int vertex;
		vertex = gra.first(i);
		if (DBUG)
			std::cout << vertex << " ";
		while (1)
		{
			vertex = gra.next(i, vertex);
			if (vertex != gra.n())
			if (DBUG)
				std::cout << vertex << " ";
			else
			{
				if (DBUG)
					std::cout << std::endl;
				break;
			}
		}
	}
	return 0;
}
int getMaxNodesFormfile(std::string filename)
{
	std::ifstream fin(filename);
	if (!fin.good())
		return 0;
	std::string line;
	int a, b, Max;
	while (!fin.eof())
	{
		getline(fin, line);
		auto it = line.begin();
		if (*it != '#')
			break;
	}
	std::stringstream strstrm(line);
	strstrm >> a;
	strstrm >> b;
	Max = a > b ? a : b;
	while (!fin.eof())
	{
		fin >> a;
		fin >> b;
		int temp = a > b ? a : b;
		Max = Max > temp ? Max : temp;
	}
	fin.close();
	return Max;
}
int main()
{
	int Rg = 100;
	int Rq = 20;
	int T = 10;
	int Qk = 20;
	int startv = 4;
	std::string filepath = "D:\\tsfTest\\";
	int N = getMaxNodesFormfile("D:\\Wiki-Vote.txt");
	TSF tsf(N, Rg, Rq, T, filepath);
	Graphl gral(N);
	readfile(gral, "D:\\Wiki-Vote.txt");//get Graphr
	std::string outfile = "D:\\out-wiki-test.txt";
	tsf.SimRank(outfile, gral, startv, Qk);
	return 0;
}