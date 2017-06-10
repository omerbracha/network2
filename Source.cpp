#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <String.h>
#include <fstream>
#include <sstream>
#include <iterator>


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
	char* Saddr;
	short Sport;
	char* Daddr;
	short Dport;
};

typedef struct flow_t* Flow;



Packet parsePacket(std::string line, int default_weight){
    Packet p = (Packet)malloc(sizeof(packet_t));
    
    if(!p)
        return NULL;
    std::istringstream iss(line);
	std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };
	
//    some_stream >> p->pId >> p->time >> p->Saddr >> p->Sport >> p->Daddr >> p->Dport >> p->length >> p->weight;
    // use sscanf on line.c_str if this doesn't work
	if (tokens.size() == 8) {
		sscanf(line.c_str(), "%ld %ld %s %hu %s %hu %d %d", p->pId, p->time, p->Saddr, p->Sport, p->Daddr, p->Dport, p->length, p->weight);
	}
	else {
		sscanf(line.c_str(), "%ld %ld %s %hu %s %hu %d", p->pId, p->time, p->Saddr, p->Sport, p->Daddr, p->Dport, p->length);
		p->weight = default_weight;
	}
	return p;
}

void push_packet(Packet packet, std::vector<Flow> flows) {
	for each (Flow flow in flows) {
		if ( (strcmp(packet->Saddr,flow->Saddr) == 0) && (packet->Sport == flow->Sport) && (strcmp(packet->Daddr,flow->Daddr) == 0) && (packet->Dport == flow->Dport) ){
			flow->packets.push_back(packet);
		}
	}
}


int main(int argc, char* argv[]) {
	long pktID, time, packtime = 0;
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
    deficit = strcmp("RR", type) != 0;
    
	std::ifstream infile(input_file);
	input = fopen(input_file, "r");
	
	if (deficit == false) {

		std::string line;
		std::getline(infile, line);
		Packet p = parsePacket(line, default_weight);
		
		while (std::getline(infile, line)) {
			push_packet(p, flows);
			packtime = p->time;
		}

	}





	return 0;
}
