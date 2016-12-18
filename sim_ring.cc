#include "sim_ring.h"
#include "SStd.h"
#include "mess_queue.h"

#include <math.h>


Ring::Ring(ring_add_type address,int node_number, int virtual_link_number):
	address_(address),
	node_number_(node_number),
	virtual_link_number_(virtual_link_number)
{
	buffer_count_ = 0;
	link_usage_.resize(virtual_link_number_);
	for(int i=0;i<virtual_link_number_;++i){
		link_usage_[i].resize(node_number_, -1);
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

// for new, link_usage_ just using as a link.
void Ring::ring_travel_()
{
	if(buffer_count_){
		mess_event m(*buffer_.begin());
		add_type des=m.des();
		add_type src=m.src();
		flit_template flit_t(m.get_flit());
		time_type event_time = m.event_start();

		if(event_time>=local_time_){
			for(int i=0;i<virtual_link_number_;++i){
				link_usage_[i][0]=-1;
			}

			local_time_=event_time+RING_DELAY_;
			link_usage_[0][0]=local_time_;

			mess_queue::wm_pointer().add_message(mess_event(local_time_,RING_,src,des,0,0,flit_t));
			remove_flit_();
		}
		else{
			time_type min_time=link_usage_[0][0];
			int min_index=0;
			for(int i=1;i<virtual_link_number_;++i){
				if(min_time>link_usage_[i][0]){
					min_time=link_usage_[i][0];
					min_index=i;
				}
			}
			// get the max value.
			if(min_time<event_time){
				min_time=event_time;
			}

			Sassert(static_cast<bool>(local_time_ <= (min_time + RING_DELAY_)));

			local_time_ = min_time + RING_DELAY_;
			link_usage_[min_index][0] = local_time_;

			mess_queue::wm_pointer().add_message(mess_event(local_time_,RING_,src,des,0,0,flit_t));
			remove_flit_();
		}
	}

}

