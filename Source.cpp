#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <String.h>


struct packet_t{
  long pId;
  long time;
  short Saddr[4];
  short Sport;
  short Daddr[4];
  short Dport;
  int length;
  int weight;
};

typedef struct packet_t* Packet;

struct flow_t{
    std::deque <Packet> packets; // Packet
};

typedef struct flow_t* Flow;


Flow* init_scheduler(int type, char* input_file, char* output_file, int default_weight, int quantum){

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