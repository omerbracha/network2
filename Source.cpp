#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <String.h>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

struct packet_t {
	long pId;
	long time;
	string Saddr;
	short Sport;
	string Daddr;
	short Dport;
	int length;
	int weight;
};

typedef struct packet_t* Packet;

struct flow_t {
	deque <Packet> packets; // Packet
	string Saddr;
	short Sport;
	string Daddr;
	short Dport;
	int weight;
	int credits;
};

typedef struct flow_t* Flow;



Packet parsePacket(std::string line, int default_weight) {
	Packet p = new packet_t;

	if (!p)
		return NULL;
	std::istringstream iss(line);
	std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };

	//    some_stream >> p->pId >> p->time >> p->Saddr >> p->Sport >> p->Daddr >> p->Dport >> p->length >> p->weight;
		// use sscanf on line.c_str if this doesn't work
	p->pId = stol(tokens[0]);
	p->time = stol(tokens[1]);
	p->Saddr = tokens[2];
	p->Sport = stoi(tokens[3]);
	p->Daddr = tokens[4];
	p->Dport = stoi(tokens[5]);
	p->length = stoi(tokens[6]);
	p->weight = (tokens.size() == 8) ? stoi(tokens[7]) : default_weight;
	return p;
}

void push_packet(Packet packet, std::vector<Flow> *flows, int quantum) {
	bool no_matching_flow = true;
	for each (Flow flow in *flows) {
		if ((packet->Saddr.compare(flow->Saddr) == 0) && (packet->Sport == flow->Sport) && (packet->Daddr.compare(flow->Daddr) == 0) && (packet->Dport == flow->Dport)) {
			flow->packets.push_back(packet);
			no_matching_flow = false;
		}
	}
	if (no_matching_flow) {
		Flow flow = new flow_t;
		flow->Saddr = packet->Saddr;
		flow->Sport = packet->Sport;
		flow->Daddr = packet->Daddr;
		flow->Dport = packet->Dport;
		flow->weight = packet->weight;
		flow->credits = 0;
		flows->push_back(flow);
		//std::deque<Packet> packs();
		flow->packets.push_back(packet);
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
		printf("not enough parameters, exiting...\n");
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
	output = fopen(output_file, "w");
	if (!output) {
		printf("no output file, exiting...\n");
		return -1;
	}

	packtime = 0;
	Packet p, futurePacket = NULL;
	bool more_packets = false;
	int is_in_progress = 0;
	int curr_weight = -1;
	int curr_flow = 0;
	Packet curr_pack;
	bool read_more = true;


	while (1) {
		if (packtime == 104) {
			int i = 0;

		}
		if (futurePacket != NULL) {
			if (futurePacket->time == packtime) {
				push_packet(futurePacket, &flows, quantum);
				futurePacket = NULL;
				read_more = true;
			}
			else {
				read_more = false;
			}
		}

		std::string line;
		if (read_more) {
			while (std::getline(infile, line)) {
				p = parsePacket(line, default_weight);
				if (flows.empty()) {
					packtime = p->time;
				}
				if (p->time > packtime) {
					futurePacket = p;
					break;
				}
				push_packet(p, &flows, quantum);
			}
		}

		if ((--is_in_progress) > 0) { // if current packet is still sending (size hasnt reduced to 0)
			packtime++;
			continue;
		}

		if (deficit == false) { // WRR

			more_packets = false;
			if ((curr_weight > 0) && (!flows[curr_flow]->packets.empty())) { // take next packet from same flow
				more_packets = true;
			}
			else if (curr_weight == -1) {
				curr_flow = 0;
				more_packets = true;
				curr_weight = flows[curr_flow]->weight;
			}
			else {//take packet from next flow for equality
				for (int i = 0; i < flows.size(); i++) {
					curr_flow = (++curr_flow) % flows.size();
					if (!flows[curr_flow]->packets.empty()) {
						more_packets = true;
						curr_weight = flows[curr_flow]->weight;
						break;
					}
				}
			}

			if (!more_packets) {
				curr_weight = 0;
				if (infile.eof())
					break;
				packtime++;
				continue;
			}

			// now we have a flow to work from
			curr_pack = flows[curr_flow]->packets.front();
			flows[curr_flow]->packets.pop_front();
			is_in_progress = curr_pack->length;
			curr_weight--;

			fprintf(output, "%ld: %ld\n", packtime, curr_pack->pId);
			// incrementing time is important for getting old
			packtime++;
		}

		else { // DRR
			more_packets = false;
			if ((curr_weight > 0) && (!flows[curr_flow]->packets.empty())) { // take next packet from same flow
				more_packets = true;
			}
			else if (curr_weight == -1) {
				curr_flow = 0;
				more_packets = true;
				curr_weight = flows[curr_flow]->weight;
				flows[curr_flow]->credits += curr_weight*quantum;
			}
			else {//take packet from next flow for equality
				for (int i = 0; i < flows.size(); i++) {
					curr_flow = (++curr_flow) % flows.size();
					if (!flows[curr_flow]->packets.empty()) {
						more_packets = true;
						curr_weight = flows[curr_flow]->weight;
						flows[curr_flow]->credits += curr_weight*quantum;
						break;
					}
					else { // flow is empty
						flows[curr_flow]->credits = 0;
					}
				}
			}

			if (!more_packets) {
				curr_weight = 0;
				curr_flow = flows.size() - 1;
				if (infile.eof())
					break;
				packtime++;
				continue;
			}

			// now we have a flow to work from
			curr_pack = flows[curr_flow]->packets.front();
			if (curr_pack->length <= flows[curr_flow]->credits) {
				flows[curr_flow]->packets.pop_front();
				is_in_progress = curr_pack->length;
				//curr_weight--;
				flows[curr_flow]->credits -= curr_pack->length;
				fprintf(output, "%ld: %ld\n", packtime, curr_pack->pId);
				packtime++;
			}
			else {
				curr_weight = 0;
			}

			// incrementing time is important for getting old
		}
	}

	return 0;
}
