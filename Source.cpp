#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <String.h>



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


int main(int argc, char* argv[]) {
	long pktID, time;
	char *sAdd, *dAdd, *ptr, *type, *input_file, *output_file;
	int sPort, dPort, length, weight, default_weight, quantum;
	std::vector<Flow> flows;
	bool deficit;
	File input, output;

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
    input = fopen(input_file, RD_ONLY);






	return 1;
}
