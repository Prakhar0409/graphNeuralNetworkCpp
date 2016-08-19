#include <bits/stdc++.h>
#include "molecule.h"


using namespace std;

// constructor for node
Node::Node(vd &st,vd &lab,vd &targets){
	state = st;
	label = lab;
	charges = targets;
	outChrg = vd (1,0);
};

//Print out the Node
void Node::Print(int i,FILE* f){
	fprintf(f,"Node Label of %dth atom: ",i);
	f(j,0,label.size()){
		fprintf(f,"%.2lf",label[j]);
	}
	fprintf(f,"\n");

	fprintf(f,"State of %dth atom: ",i);
	f(j,0,state.size()){
		fprintf(f,"%.2lf ",state[j]);
	}
	fprintf(f,"\n");

	fprintf(f,"Target label of %dth atom: ",i);
	f(j,0,charges.size()){
		fprintf(f,"%.2lf ",charges[j]);
	}
	fprintf(f,"\n");

	fprintf(f,"Output charges of %dth atom: ",i);
	f(j,0,outChrg.size()){
		fprintf(f,"%.2lf ",outChrg[j]);
	}
	fprintf(f,"\n");
}

// constructor for molecukle -> a graph
Molecule::Molecule(){
	// cout<<"A New molecule with non of the variables defined!"<<endl;
}

Molecule::Molecule(int num,vvd nodeLabels,vvd targets,v3d edgeLabs,int stateDim){
	numAtoms = num;
	vd state_init (stateDim,0);
	f(i,0,numAtoms){
		atoms.push_back(Node(state_init,nodeLabels[i],targets[i]));
	}
	edgeLabels=edgeLabs;
}

double sqr(double x){return x*x;}

void Molecule::MoleculeSetter(int num,vvd nodeLabels,vvd targets,vd px,vd py,vd pz,int stateDim){
	numAtoms = num;
	vd state_init (stateDim,0);
	f(i,0,numAtoms){
		atoms.push_back(Node(state_init,nodeLabels[i],targets[i]));
	}

	positionsX=px;
	positionsY=py;
	positionsZ=pz;
	vvd tmp;
	f(i,0,numAtoms){
		tmp.clear();		
		f(j,0,numAtoms){
			double dist = sqrt(sqr(px[i]-px[j])+sqr(py[i]-py[j])
						+ sqr(pz[i]-pz[j]));
			if(dist < 0.0001){
				tmp.push_back(vd (1,(0)));			
			}else{
				tmp.push_back(vd (1,(((atoms[i].label[0]*atoms[j].label[0])/dist))));			
			}
		}
		edgeLabels.push_back(tmp);
	}

}


void Molecule::Print(FILE* f){
//	FILE* tmp = stdout;
//	stdout = f;
	if(f==NULL){
		cout<<"File could not be opened"<<endl;
		f=stdout;
	}
	// f=stdout;
	// cout<<"LOL"<<endl;
	fprintf(f,"Num of atoms: %d\n",numAtoms);
	f(i,0,atoms.size()){
		atoms[i].Print(i,f);
		// fprintf(f,"Position of %dth atom(Z=%.1lf): (%.2lf,%.2lf,%.2lf)\n",i,atoms[i].label[0],positionsX[i],positionsY[i],positionsZ[i]);
	}
	f(i,0,edgeLabels.size()){
		f(j,0,edgeLabels[i].size()){
			f(k,0,edgeLabels[i][j].size()){
				fprintf(f,"%.2lf",edgeLabels[i][j][k]);
			}
			fprintf(f," ");
		}
		fprintf(f,"\n");
	}
	fprintf(f,"**************************************************************************************\n");

}



/*************************TEST FOR MOLECULE CLASS ******************/

// int main(){

// 	cout<<"trying size of on molecule"<<endl;
// 	cout<<sizeof(Molecule)<<endl;
// 	return 0;
// }

// vector<Molecule> molecules;
// int main(){
// 	vvd nodeLabels (2,(vd (1,1)));
// //	nodeLabels.push_back(vd (1,1)); nodeLabels.push_back(vd (1,1));
// 	vvd targets (2, (vd (1,0)));
// //	targets.push_back(vd (1,0)); targets.push_back(vd (1,0));
// 	cout<<"node Labels and targets added!"<<endl;
// 	v3d edgeLabs (2,(vvd (2,(vd (1,0)))));

// 	f(i,0,2){
// 		f(j,0,2){
// 			f(k,0,1){
// 				if(i==j)edgeLabs[i][j][k] = 0;
// 				else edgeLabs[i][j][k] = 0.28;
// 			}
// 		}
// 	}
// 	vd tp (2,2);
// 	tp[1] = 3.4;
// 	molecules.push_back(Molecule(2,nodeLabels,targets,vd (2,1),tp,vd (2,3),4));
	
// 	// cout<<"Size of mole:: ";
// 	// cout<<sizeof(molecules[0])<<endl;

// 	FILE* f=stdout;
// 	f(i,0,molecules.size()){
// 		molecules[i].Print(f);
// 	}

// 	return 0;
// }
