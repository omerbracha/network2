#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <String.h>
#include <fstream>
#include <sstream>



struct packet_t{
  long pId;
  long time;
  char* Saddr;
  short Sport;
  char* Daddr;
  short Dport;
  int length;
  int weight;
};

typedef struct packet_t* Packet;

struct flow_t{
    std::deque <Packet> packets; // Packet
	short Saddr[4];
	short Sport;
	short Daddr[4];
	short Dport;
};

typedef struct flow_t* Flow;



Packet parsePacket(std::string line){
    Packet p = (Packet)malloc(sizeof(*Packet));
    
    if(!p)
        return NULL;
    std::istringstream some_stream(line);
    some_stream >> p->pId >> time >> p->Saddr >> p->Sport >>;
    
    vector<string> strings;
    std::istringstream in(line);
    string s;
    sscanf(line.c_str)
    
    
}

void push_packet(Packet packet, std::vector<Flow> flows) {
	for each (Flow flow in flows) {
		if ( (packet->Saddr == flow->Saddr) && (packet->Sport == flow->Sport) && (packet->Daddr == flow->Daddr) && (packet->Dport == flow->Dport) ){
			flow->packets.push_back(packet);
		}
	}
}


int main(int argc, char* argv[]) {
	long pktID, time;
	char *sAdd, *dAdd, *ptr, *type, *input_file, *output_file;
	int sPort, dPort, length, weight, default_weight, quantum;
	std::vector<Flow> flows;
	bool deficit;
	FILE *input, *output;

	if (argc < 5) {
		printf("not enough parameters, exiting...");
		return -1;
	}
	type = argv[1];
	input_file = argv[2];
	output_file = argv[3];
	default_weight = atoi(argv[4]);
	quantum = atoi(argv[5]);

	// init scheduler
    deficit = strcmp("DRR", type) != 0;
    
	std::ifstream infile(input_file);
	//input = fopen(input_file, "r");

	std::string line;
	while (std::getline(infile, line)) {
		Packet p = parsePacket(line);
		push_packet(p, flows);
	}







	return 1;
}
