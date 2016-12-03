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
	int buffer_count_;
	// using multiset sort by the time of mess_event.
	multiset<mess_event > buffer_;

	// status of the link.
	// the number of the link
	int virtual_link_number_;
	// (virtual_link(vl),node),the status of the link (can it be use to traversal.) 
	vector<vector<bool> > link_usage_;

	// time
	time_type local_time_;



public:
	
	Ring(ring_add_type address, int node_number, int virtual_link_number);
	// insert a flit to a buffer.
	void add_flit_(mess_event event);
	// remove a flit from buffer.
	void remove_flit_();


	// using the magicCube algorithm get the next destination.
	static add_type get_next_traversal_node_(flit_template flit);

	// for deciding traversal in the ring or in the normal popnet.
	static bool is_ring_traversal(flit_template flit);

	// set a flit to the destination in the same ring.
	void ring_travel_();


};




#endif