#include "sim_ring.h"
#include <math.h>


Ring::Ring(ring_add_type address,int node_number, int virtual_link_number):
	address_(address),
	node_number_(node_number),
	virtual_link_number_(virtual_link_number)
{
	buffer_count_ = 0;
	link_usage_.resize(virtual_link_number_);
	for(int i=0;i<virtual_link_number_;++i){
		link_usage_[i].resize(node_number_, false);
	}

	local_time_ = -1;
}


void Ring::add_flit_(mess_event event)
{
	++buffer_count_;
	buffer_.insert(event);
}

void Ring::remove_flit_()
{
	--buffer_count_;
	buffer_.erase(buffer_.begin());
}

void Ring::ring_travel_()
{
	if(buffer_count_){
		mess_event m = *buffer_.begin();
		add_type des=sim_foundation::wsf().ring_to_3d_(m.des());

		sim_foundation::wsf().router(des).receive_flit(0,0,m.get_flit());

	}

}

