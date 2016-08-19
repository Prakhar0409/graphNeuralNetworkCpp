#include <bits/stdc++.h>

using namespace std;


class GNN{	
public:
	static const int STATEDIM = 4;
	static const int NODELABELDIM = 1;
	static const int EDGELABELDIM = 1;
	static const int TARGETDIM = 1;
	static const int FORWARDHL = 1;
	static const int FORWARDHLNEURONS = 5;
	static const int FORWARDSTATEITER = 30;
	static const int OUTHL = 1;
	static const int OUTHLNEURONS = 5;
	static const int NUMINPSF = (STATEDIM) + 2*(NODELABELDIM) + (EDGELABELDIM);
	static const int NUMOUTSF = (STATEDIM);
	static const int NUMPARAMSF = (((NUMINPSF)+1) * (FORWARDHLNEURONS))+(((FORWARDHLNEURONS)+1)*(FORWARDHLNEURONS) * ((FORWARDHL)-1))+(((FORWARDHLNEURONS)+1)* (NUMOUTSF));
	static const int NUMINPSO = (STATEDIM) + (NODELABELDIM);
	static const int NUMOUTSO = (TARGETDIM);
	static const int NUMPARAMSO = ((NUMINPSO+1)*(OUTHLNEURONS))+(((OUTHLNEURONS)+1)*(OUTHLNEURONS)*((OUTHL)-1))+(((OUTHLNEURONS)+1)*(NUMOUTSO));
};



// int main(){
// 	GNN a;
// 	cout<<a.STATEDIM<<endl;
// 	return 0;
// }