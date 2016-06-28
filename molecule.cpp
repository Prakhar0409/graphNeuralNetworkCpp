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

//Print out the Node
void Node::Print(int i){
	cout<<"Node Label of "<<i<<"th atom: ";
	f(j,0,label.size()){
		cout<<label[j]<<" ";		
	}
	cout<<endl;

	cout<<"State of "<<i<<"th atom: ";
	f(j,0,state.size()){
		cout<<state[j]<<" ";		
	}
	cout<<endl;

	cout<<"Target Label of "<<i<<"th atom: ";
	f(j,0,charges.size()){
		cout<<charges[j]<<" ";		
	}
	cout<<endl;
}

// constructor for molecukle -> a graph
Molecule::Molecule(int num,vvd nodeLabels,vvd targets,v3d edgeLabs){
	numAtoms = num;
	vd state_init (STATEDIM,0);
	f(i,0,numAtoms){
		atoms.push_back(Node(state_init,nodeLabels[i],targets[i]));
	}
	edgeLabels=edgeLabs;
};

void Molecule::Print(){
	cout<<"Num of atoms:"<<numAtoms<<endl;
	f(i,0,atoms.size()){
		atoms[i].Print(i);
	}
	f(i,0,edgeLabels.size()){
		f(j,0,edgeLabels[i].size()){
			f(k,0,edgeLabels[i][j].size()){
				cout<<edgeLabels[i][j][k]<<" ";
			}
			cout<<" ";
		}
		cout<<endl;
	}
}



/*************************TEST FOR MOLECULE CLASS ******************/
/*
vector<Molecule> molecules;
int main(){
	vvd nodeLabels (2,(vd (1,1)));
//	nodeLabels.push_back(vd (1,1)); nodeLabels.push_back(vd (1,1));
	vvd targets (2, (vd (1,0)));
//	targets.push_back(vd (1,0)); targets.push_back(vd (1,0));
	cout<<"node Labels and targets added!"<<endl;
	v3d edgeLabs (2,(vvd (2,(vd (1,0)))));

	f(i,0,2){
		f(j,0,2){
			f(k,0,1){
				if(i==j)edgeLabs[i][j][k] = 0;
				else edgeLabs[i][j][k] = 0.28;
			}
		}
	}
	molecules.push_back(Molecule(2,nodeLabels,targets,edgeLabs));
	
	f(i,0,molecules.size()){
		molecules[i].Print();
	}

	return 0;
}
*/
