#include <bits/stdc++.h>
#include "neural.h"
// #include "gnn.h"

using namespace std;
// GNN g;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;

/////////////////////////CONSTRUCTOR FOR A SINGLE NEURON////////////////////////
Neuron::Neuron(){
	numInps = 0;
	weights = NULL;
}

void Neuron::NeuronSetter(int numInputs,double *w){		//pointer passed by value
	numInps=numInputs+1;		//number of neurons in prev layer + the bias term
	weights = new double[numInps];
	copy(w,w+numInps,weights);				//weights start pointing to the expected array
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DESTRUCTOR~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
Neuron::~Neuron(){
	// cout<<"Neuron() and then NeuronSetter used so nothing to delete!"<<endl;
	if(weights!=NULL){
		delete[] weights;
	}
}
///////////////////////////////////////////////////////////////////////////////////


////////////////////////////// Constructor for NEURON LAYER ///////////////////////////
NeuronLayer::NeuronLayer(){
	numNeurons =0;
	neurons = NULL;
}


void NeuronLayer::NeuronLayerSetter(int numN,int inpsPerN,double *w){
	// cout<<"NEURAL LAYER cons 333"<<endl;
	numNeurons = numN;
	int inputsPerNeuron = inpsPerN;		// bias is taken care in neuron itself hence this is only number of neurons in previous layers
	neurons = new Neuron[numNeurons];
	double *arr = &w[0];
	f(i,0,numNeurons){
		neurons[i].NeuronSetter(inpsPerN,arr);
		arr = arr + (inputsPerNeuron + 1);	
	}
}
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DESTRUCTOR~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~///
NeuronLayer::~NeuronLayer(){
	if(neurons != NULL){
		delete[] neurons;
	}
	// cout<<"Neuron Layer dies!"<<endl;
	
}
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Constructor for NEURAL NET //////////////////////////////
NeuralNet::NeuralNet(){
	numInps=0;
	numOuts=0;
	numHiddenLayers=0;
	neuronsInHiddenLayers = NULL; 
	neuronLayers = NULL;
}



NeuralNet::NeuralNet(int numInp,int numOut, int numHiddenLayer,int *neuronsInHiddenLayer, double *w){
	cout<<"Creating a neural Net ..."<<endl;
	numInps = numInp;
	numOuts = numOut;
	numHiddenLayers = numHiddenLayer;
	neuronsInHiddenLayers = new int[numHiddenLayers];
	f(i,0,numHiddenLayers){
		neuronsInHiddenLayers[i] = neuronsInHiddenLayer[i];
	}
	
	neuronLayers = new NeuronLayer[numHiddenLayers+1];			//looks okay to allocate memory worth numHiddenLayers+1 neuronLayers;
	
	double *arr = w;

	f(i,0,numHiddenLayers+1){			
		if(i==0){
			int numParams = (numInps+1) * neuronsInHiddenLayers[i];
			neuronLayers[i].NeuronLayerSetter(neuronsInHiddenLayers[i],numInps,arr);
			arr = arr + numParams;

		}else if(i==numHiddenLayers){		//output layer
			int numParams = (neuronsInHiddenLayers[i-1]+1) * numOuts;
			neuronLayers[i].NeuronLayerSetter(numOuts,neuronsInHiddenLayers[i-1],arr);
			arr = arr + numParams;
		}else{
			int inps = neuronsInHiddenLayers[i-1]; //excluding the bias
			int outs = neuronsInHiddenLayers[i];	// number of neurons in layer

			int numParams = (inps+1) * outs;
			neuronLayers[i].NeuronLayerSetter(outs,inps,arr);
			arr = arr + numParams;
		}
	}
	cout<<"Neural net created! Inputs ->"<<numInps<<" and outputs->"<<numOuts<<endl;
	//Neural Net created
}
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DESTRUCTOR~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~///
NeuralNet::~NeuralNet(){
	// cout<<"Destrcutor of NEURAL NET"<<endl;
	if(neuronLayers!=NULL){
		delete[] neuronLayers;	
	}
	if(neuronsInHiddenLayers!=NULL){
		delete[] neuronsInHiddenLayers;
	}
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// calculates output from a set of inputs
double* NeuralNet::ForwardOne(double *inputs,int inpSize){

	double *inps = new double[inpSize];
	copy(inputs,inputs+inpSize,inps);

	double *outputs=NULL;	
	int num_outs=0;
	//check if correct number of inputs
	// if(inpSize != numInps){
	// 	cout<<"Incorrect number of inputs. Expected: "<<numInps<<". Given: "<<inpSize<<endl;
	// 	return outputs;	
	// }
	double netInp=0;
	//for each layer
	f(i,0,numHiddenLayers+1){		
		// add bias term
		outputs = new double[neuronLayers[i].numNeurons];		//start from first hidden layers
		num_outs=0;
		// for each neuron
		f(j,0,neuronLayers[i].numNeurons){

			netInp = 0;	
			int num_inps = neuronLayers[i].neurons[j].numInps;
			// cout<<"for neuron:"<<j<<endl;
			//for each weight
			f(k,0,num_inps-1){
				netInp += neuronLayers[i].neurons[j].weights[k] * inps[k];
				// cout<<neuronLayers[i].neurons[j].weights[k]<<" * "<<inputs[k]<<" <=> ";
			}
			netInp += neuronLayers[i].neurons[j].weights[num_inps-1];

			// cout<<neuronLayers[i].neurons[j].weights[num_inps-1]<<" <=> "<<netInp<<" ::: "<<Sigmoid(netInp);
			// cout<<endl;
	
			//store output for each neuron in the layer
			outputs[j]= Sigmoid(netInp);
			num_outs++;
		}
		
		delete[] inps;
		inps = outputs;
	}
	return outputs;
}

double* NeuralNet::ForwardMany(double *inputs,int inpSize,int num_inps,int outSize){
	 // cout<<"Foward Many"<<endl;
	double *outs=new double[outSize*num_inps];
	// cout<<"after"<<endl;
	f(i,0,num_inps){
		double *tmp = ForwardOne(&inputs[i*inpSize],inpSize);
		f(j,0,outSize){
			outs[i*outSize + j]=tmp[j];
		}
		delete[] tmp;
	}
	return outs;
} 

double NeuralNet::Sigmoid(double netInp){
	double ex = exp(netInp);
	double tmp= (1.0/(1.0+ex));
	return tmp;
}





// int main(){
// 	srand(3);
// 	int numInps = g.STATEDIM + 2*g.NODELABELDIM + g.EDGELABELDIM;
// 	int numOuts = g.STATEDIM;

// 	int numParams = ((numInps+1) * g.FORWARDHLNEURONS) + ((g.FORWARDHLNEURONS+1)*g.FORWARDHLNEURONS * (g.FORWARDHL-1)) + 
// 				((g.STATEDIM+1)* g.FORWARDHLNEURONS);
	
// 	double *params = new double[numParams];
// 	for(int i=0; i<numParams; i++){
// 		params[i] = 2*(RAND);
// 	}
	
// 	int *neuronsInHL = new int[g.FORWARDHL];
// 	f(i,0,g.FORWARDHL){
// 		neuronsInHL[i] = g.FORWARDHLNEURONS;
// 		cout<<neuronsInHL[i]<<endl;
// 	}

// 	cout<<"LOL??"<<endl;
// 	NeuralNet *n =NULL;

// 	n = new NeuralNet(numInps,numOuts,g.FORWARDHL,neuronsInHL,params);
// 	cout<<"Created a neuralnetwork"<<endl;

// 	double *inps = new double[10*numInps];

// 	f(i,0,10*numInps){
// 		inps[i] = ((double)i+1.0)/10.0;
// 	}

// 	double* outs;
// 	// // outs = n.ForwardOne(inps,numInps);

// 	outs = n->ForwardMany(inps,numInps,10,numOuts);

// 	f(j,0,10){
// 		f(i,0,numOuts){
// 			cout<<outs[numOuts*j +i]<<" ";	
// 		}	
// 		cout<<endl;
// 	}
	
// 	delete[] inps;
// 	delete[] outs;
// 	delete[] params;
// 	delete[] neuronsInHL;
// 	delete n;
// 	return 0;
// }
