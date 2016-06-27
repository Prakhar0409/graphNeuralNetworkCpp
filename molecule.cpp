#include <bits/stdc++.h>
#include "molecule.h"
#include "params.h"

using namespace std;

// constructor for node
Node::Node(vd &st,vd &lab,vd &targets){
	state = st;
	label = lab;
	charges = targets;
};

// constructor for molecukle -> a graph
Molecule::Molecule(int num,vvd nodeLabels,vvd targets,vvd edgeLabs){
	numAtoms = num;
	vd state_init (STATEDIM,0);
	f(i,0,numAtoms){
		atoms.push_back(Node(state_init,nodeLabels[i],targets[i]));
	}
	edgeLabels=edgeLabs;
};
