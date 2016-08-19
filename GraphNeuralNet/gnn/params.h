#define STATEDIM 4
#define NODELABELDIM 1
#define EDGELABELDIM 1
#define TARGETDIM 1

#define FORWARDHL 1
#define FORWARDHLNEURONS 5
#define FORWARDSTATEITER 30

#define OUTHL 1
#define OUTHLNEURONS 5

#define NUMINPSF (STATEDIM) + 2*(NODELABELDIM) + (EDGELABELDIM)
#define NUMOUTSF (STATEDIM)
#define NUMPARAMSF (((NUMINPSF)+1) * (FORWARDHLNEURONS))+(((FORWARDHLNEURONS)+1)*(FORWARDHLNEURONS) * ((FORWARDHL)-1))+(((FORWARDHLNEURONS)+1)* (NUMOUTSF))

#define NUMINPSO (STATEDIM) + (NODELABELDIM)
#define NUMOUTSO (TARGETDIM)
#define NUMPARAMSO ((NUMINPSO+1)*(OUTHLNEURONS))+(((OUTHLNEURONS)+1)*(OUTHLNEURONS)*((OUTHL)-1))+(((OUTHLNEURONS)+1)*(NUMOUTSO))

