/*
	Implementation of the Banker's Algorithm
	for CS33211 Homework 2
	Konnor Duncan kdunca23@kent.edu
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
	const int P = 5; //number of processes
	const int R = 3; //number of resources



	//finds how many of each resource each process needs
	void findNeed(int need[P][R], int max_allot[P][R], int allot[P][R]){
		for (int i = 0; i < P; i++){ //for each process
			for (int j = 0; j < R; j++){ //for each resource
				need[i][j] = max_allot[i][j] - allot[i][j];
		//difference between maximum resource allocation and current allocation
			}
		}
	}

	bool isDeadlock(int idle[R], int max_allot[P][R], int allot[P][R]){

		int need[P][R]; //representation of which processes need which resources
		findNeed(need, max_allot, allot);
		bool finished[P]= {0};

		int safe_sequence[P]; //will store the safe order of execution to avoide deadlock

		//represent current idle resources
		int work[R];
		for(int i = 0; i < R; i++)
			work[i] = idle[i];


		int count = 0;//number of processes in finished state
		while(count < P){
			bool deadlock = true;

			for (int i = 0; i < P; i++){ //for each process
				if (finished[i] == 0){ //if process not finished
					int j;
					for(j = 0; j < R; j++){ //for each resource type
						if (need[i][j] > work[j])
							break;
					}
					if (j==R){ // process has recieved all necessary resources
						for(int k = 0; k < R; k++)
							work[k] += allot[i][k];
							//reclaim allocated resources
						safe_sequence[count] = i;
						count++;
						finished[i] = 1;
						deadlock = false;
					}
				}
			}
			if (deadlock){
				std::cout << "System in unsafe state." <<std::endl;
				return false;
			}
		}
		std::cout << "System in safe state. Safe execution sequence is: " << std::endl;
		for(int i = 0; i < P; i++){
			std::cout << safe_sequence[i] << " ";
		}
		return true;
	}

	int main(){
		std::ifstream snapshot("snapshot.txt");
		if(!snapshot.is_open()){
			std::cout << "Error opening file" << std::endl;
			return -1;
		}

		int idle[R];
		for (int i = 0; i<R; i++){
			snapshot >> idle[i];
		}
		int max_allot[P][R];
		for (int i = 0; i<P; i++){
			for (int j = 0; i<R; j++){
				snapshot >> max_allot[i][j];
			}
		}
		int allot[P][R];
		for (int i = 0; i<P; i++){
			for (int j = 0; i<R; j++){
				snapshot >> allot[i][j];
			}
		}

		isDeadlock(idle, max_allot, allot);
	}
