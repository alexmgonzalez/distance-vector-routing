*READ ME*

This project simulates distance vector routing for a given network


With this simulator, you may input any topology.txt file and the simulator will provide the initial routing tables for each node, the final routing tables for each node, the number of rounds it took to complete each nodes routing table, the ID of the last node converged and the path used by data packets to get to its destination.. 


Build/Run Instructions:

To build this simulator, you will need access to a Linux command line. Use the cd command to get into the MIPS_SIMULATOR folder. Use the line below to compile the simulator.

	g++ main.cpp -o sim

And use this line to run the simulator (make sure to add the .txt file as the argument):
	
	./sim topology#.txt

Be sure to include the .txt file in the Computer_Networks_Project_1 folder or the program will not find the file to read.
The topology1-3.txt files are included as test with variable sized topologies.