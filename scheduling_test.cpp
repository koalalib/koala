#include <iostream>
#include <string>

#include "Koala/graph/graph.h"
#include "Koala/algorithm/schedule.h"



using namespace std;
using namespace Koala;

void scheduling_brucker()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 12;
    Vertex<string, string>* V[] =
	{
        G.addVert("1 "), G.addVert("2 "), G.addVert("3 "), G.addVert("4 "),
        G.addVert("5 "), G.addVert("6 "), G.addVert("7 "), G.addVert("8 "),
        G.addVert("9 "), G.addVert("10"), G.addVert("11"), G.addVert("12")
    };

	G.addArc(V[0], V[6]), G.addArc(V[1], V[6]), G.addArc(V[2], V[7]), G.addArc(V[3], V[8]);
	G.addArc(V[4], V[8]), G.addArc(V[5], V[9]), G.addArc(V[6], V[9]), G.addArc(V[7], V[10]);
	G.addArc(V[8], V[10]), G.addArc(V[9], V[11]), G.addArc(V[10], V[11]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(1, 0, 4, V[0]),
		Scheduling::Task<GraphType>(1, 0, 6, V[1]),
		Scheduling::Task<GraphType>(1, 0, 4, V[2]),
		Scheduling::Task<GraphType>(1, 0, 2, V[3]),
		Scheduling::Task<GraphType>(1, 0, 3, V[4]),
		Scheduling::Task<GraphType>(1, 0, 1, V[5]),
		Scheduling::Task<GraphType>(1, 0, 5, V[6]),
		Scheduling::Task<GraphType>(1, 0, 3, V[7]),
		Scheduling::Task<GraphType>(1, 0, 1, V[8]),
		Scheduling::Task<GraphType>(1, 0, 6, V[9]),
		Scheduling::Task<GraphType>(1, 0, 3, V[10]),
		Scheduling::Task<GraphType>(1, 0, 7, V[11])
	};
	Scheduling::Schedule S(3);

	cout << "Lmax = " << Scheduling::brucker(T, T + N, G, S) << " ";
	cout << Scheduling::LMax(T, T + N, S) << endl;

	for(int m = 0; m < 3; m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;
}

void scheduling_coffman()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 17;
    Vertex<string, string>* V[] =
	{
		G.addVert("3 "), G.addVert("4 "), G.addVert("5 "), G.addVert("6 "),
        G.addVert("7 "), G.addVert("8 "), G.addVert("9 "), G.addVert("10"),
		G.addVert("11"), G.addVert("12"), G.addVert("13"), G.addVert("14"),
		G.addVert("15"), G.addVert("16"), G.addVert("17"),
		G.addVert("1 "), G.addVert("2 ")
    };

	G.addArc(V[0], V[6]), G.addArc(V[0], V[7]), G.addArc(V[1], V[5]), G.addArc(V[2], V[7]);
	G.addArc(V[3], V[7]), G.addArc(V[4], V[8]), G.addArc(V[4], V[9]), G.addArc(V[5], V[10]);
	G.addArc(V[6], V[10]), G.addArc(V[7], V[11]), G.addArc(V[7], V[12]), G.addArc(V[7], V[14]);
	G.addArc(V[8], V[11]), G.addArc(V[8], V[12]), G.addArc(V[9], V[13]), G.addArc(V[13], V[12]);
	G.addArc(V[15], V[1]), G.addArc(V[15], V[2]), G.addArc(V[16], V[3]), G.addArc(V[16], V[4]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(V[0]), Scheduling::Task<GraphType>(V[1]),
		Scheduling::Task<GraphType>(V[2]), Scheduling::Task<GraphType>(V[3]),
		Scheduling::Task<GraphType>(V[4]), Scheduling::Task<GraphType>(V[5]),
		Scheduling::Task<GraphType>(V[6]), Scheduling::Task<GraphType>(V[7]),
		Scheduling::Task<GraphType>(V[8]), Scheduling::Task<GraphType>(V[9]),
		Scheduling::Task<GraphType>(V[10]), Scheduling::Task<GraphType>(V[11]),
		Scheduling::Task<GraphType>(V[12]), Scheduling::Task<GraphType>(V[13]),
		Scheduling::Task<GraphType>(V[14]), Scheduling::Task<GraphType>(V[15]),
		Scheduling::Task<GraphType>(V[16])
	};
	Scheduling::Schedule S(2);

	cout << "Cmax = " << Scheduling::coffmanGraham(T, T + N, G, S) << " ";
	cout << Scheduling::CMax(T, T + N, S) << endl;

	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;
    assert(Scheduling::test(T, T + N, G, S));
}

void scheduling_hu()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 12;
    Vertex<string, string>* V[] =
	{
        G.addVert("1 "), G.addVert("2 "), G.addVert("3 "), G.addVert("4 "),
        G.addVert("5 "), G.addVert("6 "), G.addVert("7 "), G.addVert("8 "),
        G.addVert("9 "), G.addVert("10"), G.addVert("11"), G.addVert("12")
    };

	G.addArc(V[0], V[6]), G.addArc(V[1], V[6]), G.addArc(V[2], V[7]), G.addArc(V[3], V[8]);
	G.addArc(V[4], V[8]), G.addArc(V[5], V[9]), G.addArc(V[6], V[9]), G.addArc(V[7], V[10]);
	G.addArc(V[8], V[10]), G.addArc(V[9], V[11]), G.addArc(V[10], V[11]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(V[0]), Scheduling::Task<GraphType>(V[1]),
		Scheduling::Task<GraphType>(V[2]), Scheduling::Task<GraphType>(V[3]),
		Scheduling::Task<GraphType>(V[4]), Scheduling::Task<GraphType>(V[5]),
		Scheduling::Task<GraphType>(V[6]), Scheduling::Task<GraphType>(V[7]),
		Scheduling::Task<GraphType>(V[8]), Scheduling::Task<GraphType>(V[9]),
		Scheduling::Task<GraphType>(V[10]), Scheduling::Task<GraphType>(V[11])
	};
	Scheduling::Schedule S(3);

	cout << "Cmax = " << Scheduling::hu(T, T + N, G, S) << " ";
	cout << Scheduling::CMax(T, T + N, S) << endl;

	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;
    assert(Scheduling::test(T, T + N, G, S));
}

void scheduling_liu()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 7;
    Vertex<string, string>* V[] =
	{
        G.addVert("1"), G.addVert("2"), G.addVert("3"), G.addVert("4"),
        G.addVert("5"), G.addVert("6"), G.addVert("7")
    };
	for(int i = 0; i < N - 2; i++)
		G.addArc(V[i], V[i + 2]);
	G.addArc(V[3], V[4]), G.addArc(V[5], V[6]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(3, 0, 4, V[0]),
		Scheduling::Task<GraphType>(2, 4, 6, V[1]),
		Scheduling::Task<GraphType>(2, 2, 8, V[2]),
		Scheduling::Task<GraphType>(1, 5, 15, V[3]),
		Scheduling::Task<GraphType>(4, 6, 10, V[4]),
		Scheduling::Task<GraphType>(1, 15, 20, V[5]),
		Scheduling::Task<GraphType>(2, 13, 25, V[6])
	};
	Scheduling::Schedule S(1);

	cout << "Lmax = " << Scheduling::precLiu(T, T + N, G, S) << " ";
	cout << Scheduling::LMax(T, T + N, S) << endl;


	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->part << " " << i->start << " " << i->end << endl;
    assert(Scheduling::test(T, T + N, G, S,false));
}

void scheduling_spt()
{
	typedef Graph<string, string> GraphType;
	GraphType G;

	int N = 5;
    Vertex<string, string>* V[] =
	{
        G.addVert("1"), G.addVert("2"), G.addVert("3"), G.addVert("4"), G.addVert("5")
    };

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(4, 0, 0, V[0]),
		Scheduling::Task<GraphType>(5, 0, 0, V[1]),
		Scheduling::Task<GraphType>(2, 0, 0, V[2]),
		Scheduling::Task<GraphType>(1, 0, 0, V[3]),
		Scheduling::Task<GraphType>(2, 0, 0, V[4])
	};
	Scheduling::Schedule S(3);

	cout << "SigmaCi = " << Scheduling::spt(T, T + N, S) << " ";
	cout << Scheduling::SigmaCi(T, T + N, S) << endl;

	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;
    assert(Scheduling::test(T, T + N, G, S, false));
}

void scheduling_hodgson()
{
	typedef Graph<string, string> GraphType;
	GraphType G;

	int N = 6;
    Vertex<string, string>* V[] =
	{
        G.addVert("1"), G.addVert("2"), G.addVert("3"), G.addVert("4"), G.addVert("5"), G.addVert("6")
    };

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(4, 0, 9, V[0]),
		Scheduling::Task<GraphType>(1, 0, 9, V[1]),
		Scheduling::Task<GraphType>(9, 0, 8, V[2]),
		Scheduling::Task<GraphType>(1, 0, 8, V[3]),
		Scheduling::Task<GraphType>(4, 0, 4, V[4]),
		Scheduling::Task<GraphType>(2, 0, 18, V[5])
	};
	Scheduling::Schedule S(1);

	cout << "SigmaUi = " << Scheduling::hodgson(T, T + N, S) << " ";
	cout << Scheduling::SigmaUi(T, T + N, S) << endl;

	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;
    assert(Scheduling::test(T, T + N, G, S, true));
}

void scheduling_list()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 7;
    Vertex<string, string>* V[] =
	{
        G.addVert("1"), G.addVert("2"), G.addVert("3"), G.addVert("4"),
        G.addVert("5"), G.addVert("6"), G.addVert("7")
    };
	G.addArc(V[0], V[1]), G.addArc(V[1], V[2]), G.addArc(V[2], V[3]);
	G.addArc(V[4], V[5]), G.addArc(V[5], V[6]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(3, 5, 0, V[0]),
		Scheduling::Task<GraphType>(2, 1, 0, V[1]),
		Scheduling::Task<GraphType>(2, 12, 0, V[2]),
		Scheduling::Task<GraphType>(1, 3, 0, V[3]),
		Scheduling::Task<GraphType>(4, 1, 0, V[4]),
		Scheduling::Task<GraphType>(1, 0, 0, V[5]),
		Scheduling::Task<GraphType>(2, 9, 0, V[6])
	};
	Scheduling::Schedule S(2);

    int out[7];
    Scheduling::sortLPT(T,T+N,out);
    for(int i=0;i<N-1;i++)
    {   assert(T[out[i]].length>=T[out[i+1]].length); }

    Scheduling::sortSPT(T,T+N,out);
    for(int i=0;i<N-1;i++)
    {   assert(T[out[i]].length<=T[out[i+1]].length); }

    Scheduling::sortEDD(T,T+N,out);
    for(int i=0;i<N-1;i++)
    {   assert(T[out[i]].duedate<=T[out[i+1]].duedate); }

	cout << "Cmax = " << Scheduling::ls(T, T + N, G, S) << " ";
	cout << Scheduling::CMax(T, T + N, S) << endl;

	for(int m = 0; m < S.getMachNo(); m++)
		for(std::vector<Scheduling::TaskPart>::iterator i = S.machines[m].begin(); i != S.machines[m].end(); ++i)
			cout << "Z" << T[i->task].vertex->info << " " << i->start << " " << i->end << endl;

    assert(Scheduling::test(T, T + N, G, S));
}

void scheduling_critical()
{
	typedef Graph<string, string> GraphType;

	GraphType G;
	int N = 7;
    Vertex<string, string>* V[] =
	{
        G.addVert("1"), G.addVert("2"), G.addVert("3"), G.addVert("4"),
        G.addVert("5"), G.addVert("6"), G.addVert("7")
    };
	for(int i = 0; i < N - 2; i++)
		G.addArc(V[i], V[i + 2]);
	G.addArc(V[3], V[4]), G.addArc(V[5], V[6]);

	Scheduling::Task<GraphType> T[] =
	{
		Scheduling::Task<GraphType>(3, 0, 4, V[0]),
		Scheduling::Task<GraphType>(2, 4, 6, V[1]),
		Scheduling::Task<GraphType>(2, 2, 8, V[2]),
		Scheduling::Task<GraphType>(1, 5, 15, V[3]),
		Scheduling::Task<GraphType>(4, 6, 10, V[4]),
		Scheduling::Task<GraphType>(1, 15, 20, V[5]),
		Scheduling::Task<GraphType>(2, 13, 25, V[6])
	};

	vector<Scheduling::TaskWindow> tasks;
	tasks.resize(N);
	cout << "Total time = " << Scheduling::critPath(T, T + N, G, tasks.begin()) << endl;

	for(std::vector<Scheduling::TaskWindow>::iterator i = tasks.begin(); i != tasks.end(); ++i)
		cout << " " << i->earliestStart << " " << i->latestStart << " " << i->earliestFinish << " " << i->latestFinish << endl;
}

#include "main.hpp"
	scheduling_brucker();
	scheduling_coffman();
	scheduling_hu();
	scheduling_liu();
	scheduling_spt();
	scheduling_hodgson();

	scheduling_list();
	scheduling_critical();

	return 0;
}
