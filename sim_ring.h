#ifndef SIM_RING_H_
#define SIM_RING_H_


#include "flit.h"
#include "index.h"
#include "mess_event.h"
#include "configuration.h"

#include <vector>
#include <utility>
#include <map>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <set>

/**
 * @Author		karel
 */

class Ring{

private:
	// (cube, ring)
	ring_add_type address_;
	// the number of the node in a ring.
	int node_number_;

	// buffer
	vector<int> buffer_count_;
	// for each router's port alocate a buffer.
	vector<vector<vector<mess_event> > > buffer_;
	// for each router equal to check each port buffer.
	int physical_port_;
	vector<int> wheel_;
	vector<int> use_which_link_;
	vector<int> receive_who_;

	// status of the link.
	// the number of the link
	int virtual_link_number_;
	// (virtual_link(vl),node),the occupying time of the link (can it be use to traversal.)
	vector<vector<int> > link_usage_;

	// each node save a local time.
	vector<time_type> local_time_;



public:

	Ring(ring_add_type address, int physical_port, int node_number, int virtual_link_number);
	// insert a flit to a buffer.
	void add_flit_(mess_event event, int node_id, int port);
	// remove a flit from buffer.
	void remove_flit_(int node_id, int port);


	// // using the magicCube algorithm get the next destination.
	// static add_type get_next_traversal_node_(flit_template flit);

	// // for deciding traversal in the ring or in the normal popnet.
	// static bool is_ring_traversal(flit_template flit);

	// set a flit to the destination in the same ring.
	void ring_travel_();
	bool check_and_set_link(int src, int des);
	void free_link(int src, int des);

};




#endif
