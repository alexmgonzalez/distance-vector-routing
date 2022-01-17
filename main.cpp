
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

struct rTable{
    int dest;
    int cost;
    int nH;
    
};

struct Node{
    int name;
    
    vector<rTable> rT;
    vector<int> neighbors;
};

struct dvPacket{
    int src;
    int dest;
    
    vector<rTable> compare;
};

void bothExist(Node &node1, Node &node2, int cost){
    rTable table1;
    
    table1.cost = cost;
    table1.dest = node2.name;
    table1.nH = node2.name;
    
    node1.rT.push_back(table1);
    
    rTable table2;
    
    table2.cost = cost;
    table2.dest = node1.name;
    table2.nH = node1.name;
    
    node2.rT.push_back(table2);
    
}

Node newNode(int node1, int node2, int cost){
    Node n1;
    n1.name = node1;
    Node n2;
    n2.name = node2;
    
    rTable table1;
    table1.cost = cost;
    table1.dest = node2;
    table1.nH = node2;
    
    n1.rT.push_back(table1);
    
    return n1;
}

void updateTable(Node &n1, int neighbor, int cost){
    rTable table1;
    
    table1.cost = cost;
    table1.dest = neighbor;
    table1.nH = neighbor;
    
    n1.rT.push_back(table1);
}

vector<Node> nodeList(0);
int rounds = 0;
int lastConverged = 0;

vector<Node> readFile(const string& file){
    int name;
    int cost;
    int neighbor;
    bool both;
    
    
    ifstream fin;                       //text file
    fin.open(file.c_str());
    if(!fin){
        cout << "ERROR: file not found" << endl;
        exit(0);
    }
    
    vector<Node> nodes(0);
    
    while(fin >> name >> neighbor >> cost){
        both = false;
        if(nodes.size() == 0){
            nodes.push_back(newNode(name, neighbor, cost));
            nodes.push_back(newNode(neighbor, name, cost));
        }
        else{
            for(int i = 0; i < nodes.size(); i++){
                if(nodes[i].name == name){
                    for(int j = 0; j < nodes.size(); j++){
                        if(nodes[j].name == neighbor){
                            bothExist(nodes[i], nodes[j], cost);
                            both = true;
                            break;
                        }
                    }
                    if(both){
                        break;
                    }
                    nodes.push_back(newNode(neighbor, name, cost));
                    updateTable(nodes[i],neighbor, cost);
                    break;
                }
                else if(nodes[i].name == neighbor){
                    for(int j = 0; j < nodes.size(); j++){
                        if(nodes[j].name == name){
                            bothExist(nodes[j], nodes[i], cost);
                            both = true;
                            break;
                        }
                    }
                    if(both){
                        break;
                    }
                    nodes.push_back(newNode(name, neighbor, cost));
                    updateTable(nodes[i], name, cost);
                    break;
                }
            }
        }
    }
    
    fin.close();
    return nodes;
}

void printInitial(){
    ofstream data("results.txt", ios::out | ios::trunc);
    data << "Initital Routing Tables for all nodes: " << endl << endl;
    for(int i = 0; i < nodeList.size(); i++){
        data << "Routing Table for " << nodeList[i].name << endl;
        for(int j = 0; j < nodeList[i].rT.size(); j++){
            if(j == 0){
                data << "------------------------------------" << endl
                     << "Destination \tCost \tNext Hop" << endl
                     << "------------------------------------" << endl;
                
            }
            data << "     " << nodeList[i].rT[j].dest << "          "
                 << nodeList[i].rT[j].cost << "          "
                 << nodeList[i].rT[j].nH << endl <<
                    "------------------------------------" << endl;
        }
        data << endl;
    }
}

void print(){
    ofstream data("results.txt", ios::out | ios::app);
    data << endl << "Final Routing Tables for all nodes: " << endl << endl;
    for(int i = 0; i < nodeList.size(); i++){
        data << "Routing Table for " << nodeList[i].name << endl;
        for(int j = 0; j < nodeList[i].rT.size(); j++){
            if(j == 0){
                data << "------------------------------------" << endl
                     << "Destination \tCost \tNext Hop" << endl
                     << "------------------------------------" << endl;
                
            }
            data << "     " << nodeList[i].rT[j].dest << "          "
                 << nodeList[i].rT[j].cost << "          "
                 << nodeList[i].rT[j].nH << endl <<
                    "------------------------------------" << endl;
        }
        data << endl;
    }
}

void dvPacketRounds(int duration){
    //fills the vector called "neighbors" for each node
    for(int i = 0; i < nodeList.size(); i++){
        for(int j = 0; j < nodeList[i].rT.size(); j++){
            nodeList[i].neighbors.push_back(nodeList[i].rT[j].nH);
        }
    }
    
    int cost = 0;
    bool updated;
    bool found;
    do{
        rounds++;
        updated = false;
        for(int i = 0; i < nodeList.size(); i++){
            for(int j = 0; j < nodeList[i].neighbors.size(); j++){
                dvPacket dvP;
                dvP.src = nodeList[i].name;
                dvP.dest = nodeList[i].neighbors[j];
                dvP.compare = nodeList[i].rT;
                
                for(int k = 0; k < nodeList.size(); k++){
                    if(nodeList[k].name == dvP.dest){
                        for(int v = 0; v < dvP.compare.size(); v++){
                            for(int c = 0; c < nodeList[k].rT.size(); c++){
                                if(dvP.src == nodeList[k].rT[c].dest){
                                    cost = nodeList[k].rT[c].cost;
                                }
                            }
                        }
                        for(int v = 0; v < dvP.compare.size(); v++){
                            found = false;
                            for(int c = 0; c < nodeList[k].rT.size(); c++){
                                if(dvP.compare[v].dest == nodeList[k].rT[c].dest){
                                    found = true;
                                    if((dvP.compare[v].cost + cost) < nodeList[k].rT[c].cost){
                                        nodeList[k].rT[c].cost = dvP.compare[v].cost + cost;
                                        updated = true;
                                        lastConverged = nodeList[k].name;
                                        for(int n = 0; n < nodeList[k].rT.size(); n++){
                                            if(nodeList[k].rT[n].dest == dvP.src && nodeList[k].rT[n].nH != dvP.src){
                                                nodeList[k].rT[c].nH = nodeList[k].rT[n].nH;
                                                break;
                                            }
                                            else{
                                                nodeList[k].rT[c].nH = dvP.src;
                                            }
                                        }
                                    }
                                }
                                else if(c == nodeList[k].rT.size() - 1 && found == false && dvP.compare[v].dest != nodeList[k].name){
                                    rTable newT;
                                    newT.dest = dvP.compare[v].dest;
                                    newT.cost = dvP.compare[v].cost + cost;
                                    newT.nH = dvP.src;
                                    for(int n = 0; n < nodeList[k].rT.size(); n++){
                                        if(nodeList[k].rT[n].dest == dvP.src && nodeList[k].rT[n].nH != dvP.src){
                                            newT.nH = nodeList[k].rT[n].nH;
                                            break;
                                        }
                                        else{
                                            newT.nH = dvP.src;
                                        }
                                    }
                                    
                                    nodeList[k].rT.push_back(newT);
                                    found = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }while(updated && duration > rounds-1);
    rounds--;
    
}

int totalDVPackets(){
    int total = 0;
    for(int i = 0; i < nodeList.size(); i++){
        for(int j = 0; j < nodeList[i].neighbors.size(); j++){
            total++;
        }
    }
    return total * rounds;
}


void simPacket(){
    ofstream data("results.txt", ios::out | ios::app);
    string response;
    int source;
    int destination;
    cout << "Would you like to simulate a packet being sent? (Y/N): ";
    cin >> response;
    
    if(response == "Y" || response == "y" || response == "Yes" || response == "yes"){
        cout << endl;
        bool foundSource = false;
        do{
            cout << "Please enter the node you would like to be your source: ";
            cin >> source;
            for(int i = 0; i < nodeList.size(); i++){
                if(nodeList[i].name == source){
                    foundSource = true;
                }
            }
            if(foundSource == false){
                cout << endl << "Node does not exist, please enter a valid node." << endl;
            }
        }while(foundSource == false);
            
        cout << endl;
        bool foundDest = false;
        do{
            cout << "Please enter the node you would like to be your destination: ";
            cin >> destination;
            for(int i = 0; i < nodeList.size(); i++){
                if(nodeList[i].name == destination){
                    foundDest = true;
                }
            }
            if(foundDest == false){
                cout << endl << "Node does not exist, please enter a valid node." << endl;
            }
        }while(foundDest == false);
        
        cout << endl << "Node " << source << " receives a data packet destined to node " << destination << endl
             << "The path taken is : " << endl;
        
        data << endl << "Node " << source << " receives a data packet destined to node " << destination << endl
             << "The path taken is : " << endl;
        
        int nextNode = source;
        STEP:
        for(int i = 0; i < nodeList.size(); i++){
            if(nodeList[i].name == nextNode){
                for(int j = 0; nodeList[i].rT.size(); j++){
                    if(nextNode == destination){
                        break;
                    }
                    if(nodeList[i].rT[j].dest == destination){
                        cout << "\tNode " << nextNode << " to node " << nodeList[i].rT[j].nH << endl;
                        data << "\tNode " << nextNode << " to node " << nodeList[i].rT[j].nH << endl;
                        nextNode = nodeList[i].rT[j].nH;
                        goto STEP;
                    }
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    ofstream data("results.txt", ios::out | ios::app);
    if(argv[1] == NULL || argv[2] == NULL){
        cout << "ERROR: argument not passed" << endl;
        return 0;
    }
    string file(argv[1]);
    string duration(argv[2]);
    nodeList = readFile(file);
    int durationNum = atoi(duration.c_str());
    
    
    printInitial();
    dvPacketRounds(durationNum);
    print();
    int totalpsent = totalDVPackets();
    data << "Last Node to converge: " << lastConverged << endl;
    data << "Total number of DV packets sent: " << totalpsent << endl;
    data << "Number of Rounds: " << rounds << endl << endl;
    simPacket();
    cout << endl <<  "Simulator Complete, you can find all routing tables and additional information in results.txt." << endl << endl;
    cout << "Thank you for using my simulator." << endl;
}
