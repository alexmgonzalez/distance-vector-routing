//
//  header.h
//  Computer_Networks_Project_1
//
//  Created by Alex Gonzalez on 10/25/20.
//

#ifndef header_h
#define header_h

void bothExist(struct Node &node1, struct Node &node2, int cost);
struct Node newNode(int node1, int node2, int cost);
void updateTable(struct Node &n1, int neighbor, int cost);
void newNH(struct Node *dest, int nH, int cost);
vector<struct Node> readFile(const string& file);
void printInitial();
void print();
void dvPacketRounds(int);
int totalDVPackets();
void simPacket();

#endif /* header_h */
