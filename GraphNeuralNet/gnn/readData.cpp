#include <bits/stdc++.h>
#include "molecule.h"
#include "read.h"
#define f(i,a,b) for(int i=a;i<(signed)b;i++)
#define RAND (float)(rand())/(float)(RAND_MAX) - 0.5
#define ll long long
#define pb push_back 

using namespace std;


 int numAtoms;
 vd atomsInMol;
 vd offset;
 float tmp;
 vvd labels;
 vvd charges;
 vd positionsX;
 vd positionsY;
 vd positionsZ;	

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<vvd> v3d;

vector<Molecule> trainSet;
vector<Molecule> validationSet;
vector<Molecule> testSet;
int counter = 0;

extern "C" {
void foo(){
	cout<<"I AM IN FOO!!!"<<endl;
}

extern void reader(void** data,int* trainSetSize,void** vSet,int* validationSetSize,void** teSet,int* testSetSize,int stateDim){
	//if(data == NULL){
	//	cout<<"DEBUG PRINT IN READ DATA .CPP!!!"<<endl;
	//	return;
	//}
	ifstream file("data-all-charges.bin", ios::binary);

	// file.seekg(0,file.end);
	// int len = file.tellg();
	// file.seekg(0,file.beg);
//	cout<<"fileSize: "<<len<<endl;
	int numRec;
	file.read((char*) &numRec,4);
	//file.seekg (0, file.cur);
	cout<<"numRecords: "<<numRec<<endl;
	//cout<<"pointer position: "<<file.tellg()<<endl;
	numRec = 18;

	*trainSetSize = numRec/3;
	*validationSetSize = numRec/3;
	*testSetSize = numRec - *trainSetSize - *validationSetSize;
	
	Molecule *tmpTrainSet = new Molecule[*trainSetSize];
	Molecule *tmpVSet = new Molecule[*validationSetSize];
	Molecule *tmpTestSet = new Molecule[*testSetSize];

	int tCount=0,vCount=0,teCount=0;
	
	f(i,0,numRec){
		labels.clear();
		charges.clear();
		positionsX.clear();
		positionsY.clear();
		positionsZ.clear();		
			
		// if(i==0) offset.push_back(4);
		// else offset.push_back(offset[i-1]+(numAtoms*20)+4);
			
		file.read((char*) &numAtoms,4);
	//////////////////////DEBUG PRINT///////////////////
	//	if(i<5){
	//		cout<<"pointer position: "<<file.tellg()<<endl;		
	//		cout<<"offset: "<<offset[i]<<endl; 		
	//		cout<<"NUM:"<<numAtoms<<endl;
	//	}
	//////////////////////DEBUG PRINT END///////////////////

		// loop for atomic Numbers
		f(j,0,numAtoms){
			file.read((char*) &tmp,4);
			labels.push_back(vd (1,tmp));
		}
		//loop for positions
		f(j,0,numAtoms){
			file.read((char*) &tmp,4);
			positionsX.push_back(tmp);
			file.read((char*) &tmp,4);
			positionsY.push_back(tmp);
			file.read((char*) &tmp,4);
			positionsZ.push_back(tmp);
		}

		f(j,0,numAtoms){
			file.read((char*) &tmp,4);
//			cout<<"charges:"<<tmp<<endl;
			charges.push_back(vd (1,tmp));
		}
		
		//cout<<"i = "<<i<<endl;
		if(counter==0){			//(r<0.33){
		   counter++; 
		   if(tCount >= *trainSetSize){continue;}
		   tmpTrainSet[tCount++].MoleculeSetter(numAtoms,labels,charges,positionsX,positionsY,positionsZ,stateDim);
		}else if(counter==1){			//(r<0.67){
		   counter++;
		   if(vCount >= *validationSetSize){continue;}
		   tmpVSet[vCount++].MoleculeSetter(numAtoms,labels,charges,positionsX,positionsY,positionsZ,stateDim);
		}else{
		   counter = 0;
		   if(teCount >= *testSetSize){continue;}
		   tmpTestSet[teCount++].MoleculeSetter(numAtoms,labels,charges,positionsX,positionsY,positionsZ,stateDim);
		}
	}	
//	cout<<"pointer position: "<<file.tellg()<<endl;
	*trainSetSize = tCount;
	*validationSetSize = vCount;
	*testSetSize = teCount;

	cout<<"Num of molecules in TrainSet: "<<*trainSetSize<<endl;
	cout<<"Num of molecules in validationSet: "<<*validationSetSize<<endl;
	cout<<"Num of molecules in testSet: "<<*testSetSize<<endl;


	// Molecule *tmp = (Molecule*) malloc(trainSet.size()*sizeof(Molecule));
	// f(i,0,trainSet.size()){
		// cout<<"iter: "<<i<<endl;
		// tmp[i] = trainSet[i];
	// }
	*data = tmpTrainSet;
	*vSet = tmpVSet;
	*teSet = tmpTestSet;

	file.close();
	// *trainSetSize = trainSet.size(); 

	// tmp = (Molecule*) malloc(validationSet.size()*sizeof(Molecule));
	// f(i,0,trainSet.size()){
		// tmp[i] = validationSet[i];
	// }
	// *data = tmp;
	// *vSet = tmp;//validationSet.data();
	// *validationSetSize = validationSet.size();

	// tmp = (Molecule*) malloc(testSet.size()*sizeof(Molecule));
	// f(i,0,trainSet.size()){
		// tmp[i] = trainSet[i];
	// }
	// *teSet = tmp;
	// *teSet = testSet.data();
	// *testSetSize = testSet.size(); 


	// FILE *molF;
	// molF = fopen("molecules.out","w");
	// f(i,0,trainSet.size()){
	// 	trainSet[i].Print(molF);
	// }
	// fclose(molF);
	
}
}


// int main(int argc,const char *argv[]){
// 	void *data,*x,*y;int s1,s2,s3;
// 	reader(&data,&s1,&x,&s2,&y,&s3,4);
// 	Molecule* d= (Molecule*) data;
// 	d[0].Print(stdout);
// 	// d[0].numAtoms -= 1;
// 	cout<<s1<<endl;
// 	// d[0].Print(stdout); 
// 	delete[] d;
// 	d = (Molecule*) x;
// 	delete[] d;
// 	d = (Molecule*) y;
// 	delete[] d;
// 	// free(data);
// 	// free(x);
// 	// free(y);
// 	// delete[] x;
// 	// delete[] y;
// 	return 0;
// }

