#include <bits/stdc++.h>
#include "neural.h"

Neuron::Neuron(int numInputs){
	numInps=numInputs+1;		//including the bias term
	//init random weights
	f(i,0,numInps){
		float r= (float)(rand())/(float)(RAND_MAX);
		weights.push_back(r);
	}
}

// Constructor for neuron Layer
NeuronLayer::NeuronLayer(int numN,int inpsPerN){
	numNeurons = numN;
	f(i,0,numNeurons){
		neurons.push_back(Neuron(inpsPerN));
	}
}

//Constructor for Neural Net
NeuralNet::NeuralNet(int numInp,int numOut, int numHiddenLayer,vector<int> neuronsInHiddenLayer){
	numInps = numInp;
	numOuts = numOut;
	numHiddenLayers = numHiddenLayer;
	neuronsInHiddenLayers = neuronInHiddenLayer
	f(i,0,numHiddenLayers){
		if(i==0){
			neuronLayers.push_back(NeuronLayer(neuronsInHiddenLayers[i],numInps));

		}else if(i==numHiddenLayers-1){
			neuronLayers.push_back(NeuronLayer(numOuts,neuronsInHiddenLayers[i-1]));
		}else{
			int inps = neuronsInHiddenLayers[i-1]; //excluding the bias
			int outs = neuronsInHiddenLayers[i];	// number of neurons in layer
			neuronLayers.push_back(NeuronLayer(outs,inps));
		}
	}
	//Neural Net created
}

// calculates output from a set of inputs
vector<double> NeuralNet::Update(vector<double> &inputs){
	//store resultant outputs from each layer
	vector<double> outputs;

	int cWeight=0;

	//check if correct number of inputs
	if(inputs.size() != numInps){
		cout<<"Incorrect number of inputs"<<endl;
		return outputs;	
	}

	//for each layer
	f(i,0,numHiddenLayers+1){
		if(i>0){inputs = outputs;}

		outputs.clear();
		f(j,0,neuronLayers[i].numNeurons){
			
			int inpsNum = 
		}
	}

}


int main(){
	Neuron n = Neuron(10);
	cout<<"NeuronCreated"<<endl;
	return 0;
}
