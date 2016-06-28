#include <bits/stdc++.h>
#include "neural.h"
#include "params.h"
#include "molecule.h"

using namespace std;

//Global vars
NeuralNet forwardStateNN;
NeuralNet outNetNN;
vector<Molecule> molecules;

void forwardStateCompute(vector<Molecule> &molecules){
	//stacked version of STATES and NODELABELS

	vvd inps;
	vvd outs;
	f(i,0,molecules.size()){
		f(j,0,molecules[i].atoms.size()){
			inps.clear();
			vd tmp1 = molecules[i].atoms[j].state;
			vd tmp2 = molecules[i].atoms[j].label;
			vd t12=tmp1;
			t12.insert(t12.end(),tmp2.begin(),tmp2.end());
			f(k,0,molecules[i].atoms.size()){
				if(j==k){continue;}
				vd tmp3 = molecules[i].atoms[k].label;
				vd tmp4 = molecules[i].edgeLabels[j][k];
				//stack state, label1->tmp2; label2->tmp3; edgeLabel->tmp4; feed to
				// transitionNet;Sum outp for main outer atom; this is the next state
				vd tmp = t12;	
				//tmp.insert(tmp.end(),tmp2.begin(),tmp2.end());
				tmp.insert(tmp.end(),tmp3.begin(),tmp3.end());
				tmp.insert(tmp.end(),tmp4.begin(),tmp4.end());
				inps.push_back(tmp);
			}
			

			f(k,0,FORWARDSTATEITER){
				vvd inpsTmp = inps;
				outs = forwardStateNN.ForwardMany(inpsTmp);
				f(l,0,outs.size()){
					f(m,0,outs[l].size()){
						if(l==0) tmp1[m] = outs[l][m];
						else tmp1[m] = tmp1[m] + outs[l][m];
					}
				}
				f(l,0,inps.size()){
					f(m,0,STATEDIM){
						inps[l][m] = tmp1[m];
					}	
				}
			}
			
			//put the new converged state in atom
			molecules[i].atoms[j].state = tmp1;
			cout<<"Atom "<<j+1<<" converged to state: ";
			f(k,0,tmp1.size()){
				cout<<tmp1[k]<<" ";	
			}
			cout<<endl;
		}

		
	}
}


void outNetCompute(vector<Molecule> &molecules){
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
		outs = outNetNN.ForwardMany(inps);

		cout<<"Targets: "<<endl;//<<outs.size()<<endl;
		f(k,0,outs.size()){
			f(l,0,outs[k].size()){
				cout<<outs[k][l]<<" ";
			}
			cout<<endl;
		}

	}
}



int main(){
	// adding temporary molecules 
	vvd nodeLabels (2,(vd (1,1)));
	//nodeLabels[0][0]=2;  // to see how they converge to different states
	vvd targets (2, (vd (1,0)));
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
	
	//initialize neural network
	vector<int> forwardNetNuInHL (FORWARDHL,FORWARDHLNEURONS);
	forwardStateNN = NeuralNet(STATEDIM+2*NODELABELDIM+EDGELABELDIM,STATEDIM,FORWARDHL,forwardNetNuInHL);
	//cout<<"got here"<<endl;
	forwardStateCompute(molecules);

	vector<int> outNetNuInHL (OUTHL,OUTHLNEURONS);
	outNetNN = NeuralNet(STATEDIM+NODELABELDIM,TARGETDIM,OUTHL,outNetNuInHL);
	outNetCompute(molecules);
	

	return 0;
}
