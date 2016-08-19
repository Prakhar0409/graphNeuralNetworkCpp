#include <bits/stdc++.h>
#define RAND (float)(rand())/(float)(RAND_MAX) - 0.5

using namespace std;


int main(){
	int a=3;
	double tmp = exp(a);
	cout<<"tmp::";
	cout<<tmp<<endl;
	return 0;
}


// int* foo(){
// 	int *x;
// 	x= new int[10];
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		x[i]=i+1;
// 	}
// 	return x;
// }

// int main(){

// 	int *tmp;
// 	tmp = foo();
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		cout<<tmp[i]<<" ";
// 	}
// 	cout<<endl;
// 	delete[] tmp;
// 	return  0;
// }

// void foo(int *&x){
// 	int c=3;
// 	x[0] = c;
// }

// void printVec(std::vector<int> vec){
// 	for (int i = 0; i < vec.size(); ++i)
// 	{
// 		/* code */
// 		cout<<vec[i]<<" ";
// 	}
// 	cout<<endl;
// }

// int main(){
// 	int a[4] = {1,2,3,4};
// 	int *b=a;
// 	printVec(vector<int> (&b[0],&b[0]+4));
// 	foo(b);
// 	printVec(vector<int> (&b[0],&b[0]+4));

// }

// void printVec(std::vector<int> vec){
// 	for (int i = 0; i < vec.size(); ++i)
// 	{
// 		/* code */
// 		cout<<vec[i]<<" ";
// 	}
// 	cout<<endl;
// }

// int main(){
// 	int arr[5]={5,4,3,2,1};
// 	std::vector<int> v(&arr[0],&arr[0]+5);

// 	std::vector<int>::const_iterator first = v.begin() + 0;		
// 	std::vector<int>::const_iterator last = v.begin() + 5;// + numParams;
// 	//vd tmpWeights(first, last);	

// 	std::vector<int> c(first,last);
// 	c[0]=99;c[1]=25;
// 	printVec(c);

// 	cout<<"Initial Array is :"<<endl;
// 	printVec(v);


// }


// int main(){
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		/* code */
// 		cout<<RAND<<endl;
// 	}
// }

// void foo1(int *arr) { cout << sizeof(arr) << '\n'; }
// void foo2(int arr[]) { cout << sizeof(arr) << '\n'; }
// void foo3(int arr[10]) { cout << sizeof(arr) << '\n'; }
// void foo4(int (&arr)[10]) { cout << sizeof(arr) << '\n'; }

// int main()
// {
//     int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     foo1(arr);
//     foo2(arr);
//     foo3(arr);
//     foo4(arr);
// }

// void printVec(std::vector<int> vec){
// 	for (int i = 0; i < vec.size(); ++i)
// 	{
// 		/* code */
// 		cout<<vec[i]<<" ";
// 	}
// 	cout<<endl;
// }


// void foo(std::vector<int> myVec){
// 	for (int i = 0; i < myVec.size(); ++i)
// 	{
// 		/* code */
// 		myVec[i]=myVec.size() - i;
// 	}
// 	cout<<"Printing vector inside FOO!!!"<<endl;
// 	printVec(myVec);
// }

// int main(){

// 	std::vector<int> v(4,0);

// 	for (int i = 0; i < 4; ++i)
// 	{
// 		/* code */
// 		v[i]=i;
// 	}

// 	cout<<"Printing after initializing"<<endl;
// 	printVec(v);

// 	foo(v);

// 	cout<<"Printing vector after foo in Main"<<endl;
// 	printVec(v);

	

// 	return 0;
// }