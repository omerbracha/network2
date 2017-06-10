#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
	long pktID, time;
	char *sAdd, *dAdd, *ptr, *type, *input_file, *output_file;
	int sPort, dPort, length, weight, default_weight, quantum;

	if (argc < 5) {
		printf("not enough parameters, exiting...");
		return -1;
	}
	type = argv[0];
	input_file = argv[1];
	output_file = argv[2];
	default_weight = atoi(argv[3]);
	quantum = atoi(argv[4]);



	return 1;
}