#include <bits/stdc++.h>
#include "params.h"
#define f(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<vvd> v3d;


class Node{
public:
	vector<double> state;	// node State
	vector<double> label;	// node labels
	vector<double> charges;	// node targets
	
	//constructor
	Node(vd &st, vd &lab, vector<double> &targets);
	void Print(int i);
};

class Molecule{
public:
	int numAtoms;
	vector<Node> atoms;		//atomic numbers=node Labels
	v3d edgeLabels;	//distances -> 100 if disconnected

	//constructor
	Molecule(int num,vvd nodeLabels,vvd targets,v3d edgeLabels);
	void Print();
	
};
