//header file for basic neural network classes
#include <bits/stdc++.h>
#define f(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;

class Neuron{
  public:
	int numInps;
	vector<double> weights;
	
	// constructor
	Neuron(int numInputs);
};

/*	
*	NEURON LAYER
*/
class NeuronLayer{
  public:
	int numNeurons;
	vector<Neuron> neurons;

	//constructor
	NeuronLayer(int numN, int inpsPerN);
};

/*
*	NEURAL NET
*/
class NeuralNet{
  public:
	int numInps;
	int numOuts;
	int numHiddenLayers;
	vector<int> neuronsInHiddenLayers; 

	vector<NeuronLayer> neuronLayers;

	// Contructor
	NeuralNet(int numInp,int numOut, int numHiddenLayer,vector<int> neuronsInHiddenLayer);
	vd ForwardOne(vd &inputs);
	vvd ForwardMany(vvd &inputs);
	double Sigmoid(double netInp);
	
};




