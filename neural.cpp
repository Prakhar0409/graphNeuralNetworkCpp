#include <bits/stdc++.h>
#include "neural.h"


Neuron::Neuron(int numInputs){
	numInps=numInputs+1;		//including the bias term
	//init random weights
	f(i,0,numInps){
		float r= (float)(rand())/(float)(RAND_MAX) - 1;
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

NeuralNet::NeuralNet(){

}

//Constructor for Neural Net
NeuralNet::NeuralNet(int numInp,int numOut, int numHiddenLayer,vector<int> neuronsInHiddenLayer){
	numInps = numInp;
	numOuts = numOut;
	numHiddenLayers = numHiddenLayer;
	neuronsInHiddenLayers = neuronsInHiddenLayer;
	f(i,0,numHiddenLayers+1){			//+1 for the output layer
		if(i==0){
			neuronLayers.push_back(NeuronLayer(neuronsInHiddenLayers[i],numInps));

		}else if(i==numHiddenLayers){		//output layer
			neuronLayers.push_back(NeuronLayer(numOuts,neuronsInHiddenLayers[i-1]));
		}else{
			int inps = neuronsInHiddenLayers[i-1]; //excluding the bias
			int outs = neuronsInHiddenLayers[i];	// number of neurons in layer
			neuronLayers.push_back(NeuronLayer(outs,inps));
		}
	}
	cout<<"Neural net created!"<<endl;
	//Neural Net created
}

// calculates output from a set of inputs
vector<double> NeuralNet::ForwardOne(vd &inputs){
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
//#			cout<<"layer number:"<<i<<endl;
		if(i>0){inputs = outputs;}
		// add bias term
		inputs.push_back(1.0);			//last input of the neuron is the bias input from prev layer		

		outputs.clear();
		// for each neuron
		f(j,0,neuronLayers[i].numNeurons){
			double netInp = 0;	
			int num_inps = neuronLayers[i].neurons[j].numInps;

			//for each weight
			f(k,0,num_inps){
				netInp += neuronLayers[i].neurons[j].weights[k] * inputs[k];
			}
//#				cout<<"neuron number:"<<j<<endl;
			//store output for each neuron in the layer
			outputs.push_back(Sigmoid(netInp));		
		}
	}
	return outputs;
}

vvd NeuralNet::ForwardMany(vvd &inputs){
	vvd outs;
	f(i,0,inputs.size()){
		outs.push_back(ForwardOne(inputs[i]));
	}
	return outs;
} 

double NeuralNet::Sigmoid(double netInp){
	return (1/(1+exp(-netInp)));
}


/*
int main(){
	vector<int> neuronsInHL;
	neuronsInHL.push_back(4); neuronsInHL.push_back(4);
	NeuralNet n = NeuralNet(5,3,2,neuronsInHL);
	vector<double> inps;
	f(i,0,5){
		inps.push_back((float)(rand())/(float)(RAND_MAX));
	}
	vector<double> outs;
	outs=n.ForwardOne(inps);
	cout<<"size of output vector:"<<outs.size()<<endl;
	f(i,0,outs.size()){
		cout<<outs[i]<<endl;	
	}	
	//cout<<"NeuronCreated"<<endl;
	return 0;
}
*/
