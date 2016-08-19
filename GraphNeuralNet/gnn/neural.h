//header file for basic neural network classes
#include <bits/stdc++.h>
#define f(i,a,b) for(int i=a;i<(signed)b;i++)
#define RAND (float)(rand())/(float)(RAND_MAX) - 0.5
#define pb push_back

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;

class Neuron{
  public:
	int numInps;
	double* weights;
	
	// constructor
	Neuron();
	void NeuronSetter(int numInputs,double *w);
	//destructor
	~Neuron();
};

/*	
*	NEURON LAYER
*/
class NeuronLayer{
  public:
	int numNeurons;
	Neuron *neurons;	//pointer to starting neuron neuron

	//constructor
	NeuronLayer();
	void NeuronLayerSetter(int numN, int inpsPerN,double *w);
	//destructor
	~NeuronLayer();
};

/*
*	NEURAL NET
*/
class NeuralNet{
  public:
	int numInps;
	int numOuts;
	int numHiddenLayers;
	int *neuronsInHiddenLayers; 

	NeuronLayer *neuronLayers;

	// Contructor
	NeuralNet();
	// NeuralNet(int numInp,int numOut, int numHiddenLayer,int *neuronsInHiddenLayer);
	NeuralNet(int numInp,int numOut, int numHiddenLayer,int *neuronsInHiddenLayer,double *w);
	//destrutor
	~NeuralNet();
	
	double* ForwardOne(double *inputs,int inpSize);
	double* ForwardMany(double *inputs,int inpSize,int num_inps,int outSize);
	double Sigmoid(double netInp);
	
};




