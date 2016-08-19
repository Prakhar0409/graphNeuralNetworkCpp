#include <bits/stdc++.h>

#define f(i,a,b) for(int i=a;i<(signed)b;i++)

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<vvd> v3d;

class Node{
public:
	vd state;	// node State
	vd label;	// node labels
	vd charges;	// node targets
	vd outChrg;	// output charges
	
	//constructor
	Node(vd &st, vd &lab, vector<double> &targets);
	void Print(int i,FILE* f);
};

class Molecule{
public:
	int numAtoms;
	vector<Node> atoms;		//atomic numbers=node Labels
	vd positionsX;
	vd positionsY;
	vd positionsZ;	
	v3d edgeLabels;	//distances -> 100 if disconnected

	//constructor
	Molecule();
	Molecule(int num,vvd nodeLabels,vvd targets,v3d edgeLabels,int StateDim);
	void MoleculeSetter(int num,vvd nodeLabels,vvd targets,vd px,vd py,vd pz,int StateDim);
	void Print(FILE* f);
	
	
};
