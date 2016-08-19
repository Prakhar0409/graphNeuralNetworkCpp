#include <bits/stdc++.h>
#include "neural.h"
#include "molecule.h"
#include "read.h"
#include <unistd.h>
#include "gnn.h"
#define pb push_back
#define RAND (float)(rand())/(float)(RAND_MAX) - 0.5

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;

//Global vars
NeuralNet *forwardStateNN=NULL;
NeuralNet *outNetNN=NULL;
vector<Molecule> molecules;
GNN g;

void cpy(double * res,double *src,int len){
	f(i,0,len){
		res[i]=src[i];
	}
}

void forwardStateCompute(vector<Molecule> &molecules,int dataSize){//Molecule *molecules,int dataSize){//vector<Molecule> &molecules){
	//stacked version of STATES and NODELABELS

	double *outs;

	//for each molecule in the set
	f(i,0,dataSize) {
		int numAtoms = molecules[i].atoms.size();
		int numOfInpToF = (numAtoms*numAtoms);//-numAtoms+32);// - numAtoms;
		// cout<<"numOfInpToF: "<<numOfInpToF<<endl;
		
		double *inps = new double[numOfInpToF * g.NUMINPSF];		//number of atoms * input for each atom
		// cout<<"inps start::"<<inps<<"  aend::"<<inps + (numOfInpToF * g.NUMINPSF)<<endl;
		double *res = inps;
		double *states = new double[numAtoms* g.NUMOUTSF];
		// cout<<"You have crossed the check point"<<endl;
		// for each atom in the molecule
		// cout<<"res base add:: "<<res<<endl;
		int counter=0;
		f(j,0,numAtoms){
			// cout<<"hey behdb :: "<<j<<endl;
			double *tmp1 = molecules[i].atoms[j].state.data();
			double *tmp2 = molecules[i].atoms[j].label.data();
			copy(tmp1,tmp1+g.STATEDIM,states+j* g.NUMOUTSF);
			// cout<<"yoyoyoy:: "<<j<<endl;

			f(k,0,numAtoms){
				// cout<<"innternewhjfb :: "<<k<<endl;
				double *tmp3 = molecules[i].atoms[k].label.data();
				double *tmp4 = molecules[i].edgeLabels[j][k].data();
				
				copy(tmp1,tmp1+g.STATEDIM,res);
				// cout<<"copy 1"<<endl;
				copy(tmp2,tmp2+g.NODELABELDIM,res+g.STATEDIM);
				// cout<<"copy 2"<<endl;
				copy(tmp3,tmp3+g.NODELABELDIM,res+g.STATEDIM+g.NODELABELDIM);
				copy(tmp4,tmp4+g.EDGELABELDIM,res+g.STATEDIM+g.NODELABELDIM+g.NODELABELDIM);
				
				res = res + g.NUMINPSF;
				counter++;
			}		
			// cout<<"OUTOTHITNIUTN:: "<<j<<endl;
		}
		// cout<<"res base add:: "<<res<<"  counter: "<<counter<<endl;
		
		f(k,0,g.FORWARDSTATEITER){
		
			//double* NeuralNet::ForwardMany(double *inputs,int inpSize,int num_inps,int outSize){
			outs = forwardStateNN->ForwardMany(inps,g.NUMINPSF,numOfInpToF,g.NUMOUTSF);
			// outs is a array of numAtoms^2 * OutSize=(STATEDIM) size
			double *tmp = new double[numAtoms* g.NUMOUTSF];
			f(l,0,numAtoms){
				f(m,0,numAtoms){
					f(n,0,(g.STATEDIM)){
						// cout<<outs[l*(NUMOUTSF)*numAtoms+m*(NUMOUTSF)+n]<<" ";
						//	if(l==m){continue;}
						if(m==0) {tmp[l* g.NUMOUTSF + n] = outs[l* g.NUMOUTSF *numAtoms + m * g.NUMOUTSF + n];}
						else {tmp[l* g.NUMOUTSF + n] = tmp[l* g.NUMOUTSF + n] + outs[l* g.NUMOUTSF *numAtoms + m* g.NUMOUTSF + n];}
					}
					// cout<<endl;
				}
				// cout<<endl;
			}

			// cout<<"States new::"<<endl;
			// f(x,0,numAtoms){
				// f(y,0,(NUMOUTSF)){
					// cout<<tmp[x*(NUMOUTSF)+y]<<" ";
				// }
				// cout<<endl;
			// }

			//assinging states to next input
			double diff=0;
			f(l,0,numAtoms){
				f(m,0,numAtoms){
					// cout<<"inps base::"<<&inps[l*(NUMINPSF)*numAtoms+m*(NUMINPSF)]<<" ==> ";
					f(n,0,(g.STATEDIM)){
						if(m==0){diff = diff + inps[l* g.NUMINPSF *numAtoms + m* g.NUMINPSF  + n] - tmp[l* g.NUMOUTSF +n];}
						inps[l* g.NUMINPSF *numAtoms + m* g.NUMINPSF  + n] = tmp[l* g.NUMOUTSF +n];
						// cout<<inps[l*(NUMINPSF)*numAtoms + m*(NUMINPSF) + n]<<" ";
					}	
					// cout<<endl;
				}
				// cout<<endl;
			}

			delete[] tmp;
			delete[] outs;
			if(abs(diff)<0.5){
				// cout<<"Difference = "<<diff<<"  and number of iters = "<<k<<endl;
				break;
			}

		}		
		//put the new converged state in atom
		f(x,0,numAtoms){
			f(y,0,g.STATEDIM){
				// cout<<"inps base:: "<<&inps[x*numAtoms*(NUMINPSF)+y]<<"  ==> ";
				molecules[i].atoms[x].state[y] = inps[x*numAtoms*g.NUMINPSF+y];	
				// cout<<molecules[i].atoms[x].state[y]<<" ";	
			}
			// cout<<endl;
		}
		
			// f(x,0,numAtoms){
				// cout<<"Atom "<<x+1<<" converged to state: ";
				// f(y,0,(STATEDIM)){
					// cout<<inps[x*numAtoms*(NUMINPSF)+y]<<"  ";		
				// }
				// cout<<endl;
			// }
		delete[] states;
		delete[] inps;

	}
}


void outNetCompute(vector<Molecule> &molecules,int dataSize){//Molecule* molecules,int dataSize){//vector<Molecule> &molecules){
	//stacked version of STATES and NODELABELS
	double Qnet=0;
	double *outs;
	// vvd outs;
	// for each molecule in the set
	f(i,0,dataSize){
		Qnet = 0;
		int numAtoms = molecules[i].atoms.size();
		// states.clear();
		// labels.clear();
		double *inps = new double[(numAtoms* (g.NUMINPSO))];
		double *res = inps;	
		// for each atom in the molecule
		f(j,0,numAtoms){
			// states.push_back(molecules[i].atoms[j].state);
			// labels.push_back(molecules[i].atoms[j].label);
			double *tmp1 = molecules[i].atoms[j].state.data();
			double *tmp2 = molecules[i].atoms[j].label.data();
			copy(tmp1,tmp1+ g.STATEDIM,res);
			copy(tmp2,tmp2+ g.NODELABELDIM,res+ g.STATEDIM);
			res = res + g.NUMINPSO;
		}
		// double* NeuralNet::ForwardMany(double *inputs,int inpSize,int num_inps,int outSize){
		// cout<<"AM I HERE?"<<endl;
		outs = outNetNN->ForwardMany(inps,g.NUMINPSO,numAtoms,g.TARGETDIM);

		

		f(j,0,numAtoms){
			f(k,0,g.TARGETDIM){
				Qnet += outs[j* g.TARGETDIM +k];
			}
		}
		Qnet = (Qnet/ (double)(numAtoms*g.TARGETDIM) );
		

		f(j,0,numAtoms){
			f(k,0, g.TARGETDIM ){
				molecules[i].atoms[j].outChrg[k] = outs[j* g.TARGETDIM +k] - Qnet;
//				cout<<"Qnet : "<<molecules[i].atoms[j].outChrg[k] <<endl;
			}			
		}
		delete[] outs;
		delete[] inps;

	}
}

double computeCost(vector<Molecule> &m,int dataSize){//Molecule* m,int dataSize){//vector<Molecule> m){
	double cost=0;	
	f(i,0,dataSize){
		f(j,0,m[i].atoms.size()){
			f(k,0,m[i].atoms[j].outChrg.size()){
				double tmp = (m[i].atoms[j].outChrg[k] - m[i].atoms[j].charges[k]);
//				cout<<"Charges : " <<m[i].atoms[j].charges[k]<<"    ";
//				cout<<"DIFF : "<<tmp<<endl;
				tmp = tmp*tmp;
				cost -= tmp; 
			}
		}
	}
	return cost;
}

extern "C" {

void printTestCase(vector<Molecule> molecules,int dataSize){
	cout<<"PRINTING TEST RESULTS"<<endl;
	FILE* outF;
	outF = fopen("testResults.out","w");
	//	Molecule* data = (Molecule*) d;

	//	vector<Molecule> molecules;
	//	int l = dataSize;
	//	f(i,0,l){
	//		molecules.pb(data[i]);
	//	}
	
	f(i,0,molecules.size()){
		fprintf(outF,"********************For %dth molecule****************\n",i);		
		Molecule t = molecules[i];
		f(j,0,molecules[i].atoms.size()){
			Node tmp = t.atoms[j];
			fprintf(outF,"Z: %.1lf | positon: (%.2lf,%.2lf,%.2lf) | Expt.Chrg: %.3lf | Predicted: %.3lf | Diff: %.3lf\n",tmp.label[0],t.positionsX[j],t.positionsY[j],t.positionsZ[j],tmp.charges[0], tmp.outChrg[0], tmp.charges[0]-tmp.outChrg[0]);
		}
		fprintf(outF,"******************************************************\n");
	}
	fclose(outF);
	
}

double eval(void* d,int dataSize,double* params,int paramSize,int printOut){
	if(d==NULL || params ==NULL){
		cout<<"DEBUG PRINT"<<endl;
		return 0;
	}

	Molecule *data;
	data = (Molecule*) d;

	vector<Molecule> molecules;
	int l = dataSize;   
	cout<<"dataSize::"<<dataSize<<endl;
	// data[0].Print(stdout);
	f(i,0,l){
		molecules.push_back(data[i]);
	}

	if(data == NULL){
		cout<<"DATA PASSED TO EVALUATE IN LEARN.CPP IS NULL."<<endl;
	}	

	
	//cout<<"Length of params:"<<paramSize<<endl;
	double *forwardWeights = new double[g.NUMPARAMSF];
	double *outWeights = new double[g.NUMPARAMSO];

	f(i,0,paramSize){
		if(i<g.NUMPARAMSF){	
			forwardWeights[i] = params[i];
		}else{
			outWeights[i-g.NUMPARAMSF] = params[i];
		}
	}


	int *forwardNetNuInHL = new int[g.FORWARDHL];
	f(i,0,g.FORWARDHL){forwardNetNuInHL[i] = g.FORWARDHLNEURONS;}
	int *outNetNuInHL = new int[g.OUTHL];
	f(i,0,g.OUTHL){	outNetNuInHL[i] = g.OUTHLNEURONS;}
	
	// NeuralNet(int numInp,int numOut, int numHiddenLayer,int *neuronsInHiddenLayer, double *w){
	forwardStateNN = new NeuralNet(g.NUMINPSF,g.NUMOUTSF,g.FORWARDHL,forwardNetNuInHL,forwardWeights);
	cout<<"Forward Net Initialised"<<endl<<endl<<endl<<endl;
	
	outNetNN = new NeuralNet(g.NUMINPSO,g.NUMOUTSO,g.OUTHL,outNetNuInHL,outWeights);
	cout<<"Forward Net and Out Net Initialised"<<endl<<endl<<endl<<endl;
	

	forwardStateCompute(molecules,molecules.size());
	cout<<"Forward State Computation Done"<<endl<<endl<<endl<<endl;

	outNetCompute(molecules,molecules.size());
	cout<<"Output Target Computation Done"<<endl<<endl<<endl<<endl;
	
	double fitness = computeCost(molecules,molecules.size());
	
	delete forwardStateNN;
	delete outNetNN;
	delete[] forwardNetNuInHL;
	delete[] outNetNuInHL;
	delete[] forwardWeights;
	delete[] outWeights;

	
	// cout<<"Yo I am here and this is done!!"<<endl;
	return fitness;
}


}



int main(){
	// adding temporary molecules 
	void* data,*x,*y;int s1=0,s2=0,s3=0;
	reader(&data,&s1,&x,&s2,&y,&s3,g.STATEDIM);

	
	int numParams = g.NUMPARAMSF + g.NUMPARAMSO; 
	double *w = new double[numParams];
	for(int i=0;i<numParams;i++){
		w[i] =  2*RAND;
	}
	double fitness = eval(data,s1,w,numParams,0);

	cout<<"Printing fitness: "<<fitness<<endl;
	
	delete[] w;
	delete[] (Molecule*) data;
	delete[] (Molecule*) x;
	delete[] (Molecule*) y;

	return 0;
}
