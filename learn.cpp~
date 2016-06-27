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
			outs = forwadStateNN.ForwardMany(inps);
			inps = outs;
			f(k,0,outs.size()){
				inps[k].insert(inps.end(),labels[k].begin(),labels[k].end());
			}		
			cout<<"reached out states with size:"<<outs.size()<<endl;
		}

	}
}

int main(){
	vvd nodeLabels;
	nodeLabels.push_back(vd (1,1));
	nodeLabels.push_back(vd (1,1));
	vvd targets;
	targets.push_back(vd (1,0));	
	targets.push_back(vd (1,0));

	vvd edgeLabs;
	f(i,0,2){
		f(j,0,2){
			if(i==j)edgeLabs[i][j] = 0;
			else edgeLabs[i][j] = 0.28;
		}
	}
	molecules.push_back(Molecule(2,nodeLabels,targets,edgeLabs));

//NeuralNet(int numInp,int numOut, int numHiddenLayer,vector<int> neuronsInHiddenLayer)
	vector<int> nuInHL (FORWARDHL,FORWARDHLNEURONS);
	forwardStateNN = NeuralNet(STATEDIM+NODELABELDIM,STATEDIM,FORWARDHL,nuInHL);

	forwardStateCompute(molecules);

	return 0;
}
