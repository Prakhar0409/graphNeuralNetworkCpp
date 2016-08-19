#Estimation of partial molecular charges using Graph Neural Network

##Overview

As suggested by the title the project aims to learn a mapping between a molecule and the atomic charges the atoms in it posses. Since the vanilla version of Neural networks does not support variable number of inputs/outputs as may be required for a molecule with different number of atoms, we follow a recently discovered and not so popular approach here, of the Graph Neural Network. 

Please follow on to the **GNNreport** to understand the project and the implementation details. Also the references given below in the report sever as quite extensive readins for the graph neural network.

##Getting it up and running

The **Makefile** in the main/root directory of the project compiles the GA Library that the subsequesnt GNN uses for learning. Please see learning for more details on this. You can then run the tests in the **Tests** directory to check if the library is correctly compiled. Then switch onto the **GraphNeuralNetwork** directory and run the makefile again. To run program in parallel please look up to documentation of your system specific mpi version. 


##Learning

The learning in this model of graph neural network proceeds using a vanilla version of the widely used **Genetic Algorithm** (coded up in C). Though it is very expensive and explores the complete search space, it has been used for the ease of implementation and get things up and running in a short while. The GA implementation uses **mpi** to parallelize learning so as to get things faster. 

The implementation of GA is generic. The code is credited to **Federici Canova Filippo**, who is a researcher at **Aalto School of Science, Finland**.Please visit **https://people.aalto.fi/new/filippo.federici.canova** for more details about him.
######Using the GA code requires his prior permission
Feel free to reach him at **filippo.federici@aalto.fi**


##Contribution

Feel free to fork the repo and send pull requests in order to improve it. 

##License

MIT License
Please see LICENSE file for more details
