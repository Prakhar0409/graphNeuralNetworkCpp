#include <bits/stdc++.h>
#include "neural.h"
#include "params.h"
#include "molecule.h"

using namespace std;

//Global vars
NeuralNet forwardStateNN;
vector<Molecule> molecules;

void forwardStateCompute(vector<Molecule> &molecules){
	//stacked version of STATES and NODELABELS
	vvd states;
	vvd labels;
	vvd inps;
	vvd outs;
	f(i,0,molecules.size()){
		states.clear();
		labels.clear();
		inps.clear();
		f(j,0,molecules[i].atoms.size()){
			states.push_back(molecules[i].atoms[j].state);
			labels.push_back(molecules[i].atoms[j].label);
			vd tmp1 = molecules[i].atoms[j].state;
			vd tmp2 = molecules[i].atoms[j].label;
			tmp1.insert(tmp1.end(),tmp2.begin(),tmp2.end());
			inps.push_back(tmp1);
		}

		f(j,0,FORWARDSTATEITER){		
			outs = forwardStateNN.ForwardMany(inps);
			inps = outs;
			f(k,0,outs.size()){
				inps[k].insert(inps[k].end(),labels[k].begin(),labels[k].end());
			}		
			cout<<"states in "<<j<<"th iteration: "<<endl;//<<outs.size()<<endl;
			f(k,0,outs.size()){
				f(l,0,outs[k].size()){
					cout<<outs[k][l]<<" ";
				}
				cout<<endl;
			}

		}

	}
}



int main(){
	// adding temporary molecules 
	vvd nodeLabels (2,(vd (1,1)));
	vvd targets (2, (vd (1,0)));
	vvd edgeLabs (2,(vd (2,0)));
	f(i,0,2){
		f(j,0,2){
			if(i==j)edgeLabs[i][j] = 0;
			else edgeLabs[i][j] = 0.28;
		}
	}
	molecules.push_back(Molecule(2,nodeLabels,targets,edgeLabs));
	
	//initialize neural network
	vector<int> nuInHL (FORWARDHL,FORWARDHLNEURONS);
	forwardStateNN = NeuralNet(STATEDIM+NODELABELDIM,STATEDIM,FORWARDHL,nuInHL);
	forwardStateCompute(molecules);

	return 0;
}
