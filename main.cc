#include <exception>
#include <iostream>
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_queue.h"

#include <string>
#include <sstream>

extern "C" {
#include "SIM_power.h"
#include "SIM_router_power.h"
#include "SIM_power_router.h"
}

#define cube_x 8
#define cube_y 8
#define cube_z 4
#define cycle 1000

#define rate_start 0.01
#define rate_end  0.01
#define rate_inc  0.04

void create_sub_bench();
void create_uniform_bench(double rate);
void create_reversal_bench(double rate);
void create_hotspot_bench(double rate);


int main(int argc, char *argv[])
{
	ofstream result("./results");
	SRGen random_gen;
	configuration c_par(argc, argv);
	cout<<c_par;
	for(double rate = rate_start; rate<=rate_end; rate+=rate_inc){
		//create_uniform_bench(rate);
		try {
			mess_queue network_mess_queue(0.0);
			sim_foundation sim_net;
			network_mess_queue.simulator();
			double delay=sim_net.simulation_results();
			result<<rate<<"\t"<<delay<<"\n";
		} catch (exception & e) {
			cerr << e.what();
		}
	}
	result.close();

}

void create_sub_bench(){
	system("rm ./random_trace_magic_cube/bench.*");
	ifstream b("./random_trace_magic_cube/bench");
	double time;
	int x,y,z;
	string trace;
	while(getline(b,trace)){
		stringstream s;
		s<<trace;
		s>>time>>x>>y>>z;
		stringstream file;
		file <<"./random_trace_magic_cube/bench."<<x<<"."<<y<<"."<<z;
		ofstream subbench;
		subbench.open(file.str().c_str(),ios::app);
		subbench<<trace<<"\n";
		subbench.close();
	}
	b.close();
}

void create_uniform_bench(double rate){
	ofstream bench("./random_trace_magic_cube/bench");
	double time = 1;
	SRGen r;
	for(; time < cycle;){
		for(int i=0;i<cube_x;i++){
			for(int j=0;j<cube_y;j++){
				for(int k=0;k<cube_z;k++){
					ostringstream trace;
					trace << time <<" " << i <<" " << j <<" " << k <<" " << r.flat_l(0,cube_x) <<" " << r.flat_l(0,cube_y) <<" " << r.flat_l(0,cube_z) <<" " << 5 << "\n";
					bench<<trace.str();
				}
			}
		}
		time += 1/rate;
	}
	bench.close();
	create_sub_bench();
}

void create_reversal_bench(double rate){
	ofstream bench("./random_trace_magic_cube/bench");
	double time = 1;
	SRGen r;
	for(; time < cycle;){
		for(int i=0;i<cube_x;i++){
			for(int j=0;j<cube_y;j++){
				for(int k=0;k<cube_z;k++){
					ostringstream trace;
					trace << time <<" " << i <<" " << j <<" " << k <<" " << (cube_x-1-i) <<" " << (cube_y-1-j) <<" " << (cube_z-1-k) <<" " << 5 << "\n";
					bench<<trace.str();
				}
			}
		}
		time += 1/rate;
	}
	bench.close();
	create_sub_bench();
}
void create_hotspot_bench(double rate){
	ofstream bench("./random_trace_magic_cube/bench");
	double time = 1;
	SRGen r;
	for(; time < cycle;){
		for(int i=0;i<cube_x;i++){
			for(int j=0;j<cube_y;j++){
				for(int k=0;k<cube_z;k++){
					ostringstream trace;
					trace << time <<" " << i <<" " << j <<" " << k <<" " << (cube_x/2) <<" " << (cube_y/2) <<" " << (cube_z/2) <<" " << 5 << "\n";
					bench<<trace.str();
				}
			}
		}
		time += 1/rate;
	}
	bench.close();
	create_sub_bench();
}
