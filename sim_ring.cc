#include "sim_ring.h"
#include "SStd.h"
#include "mess_queue.h"

#include <math.h>


Ring::Ring(ring_add_type address,int physical_port, int node_number, int virtual_link_number):
	address_(address),
	physical_port_(physical_port),
	node_number_(node_number),
	virtual_link_number_(virtual_link_number)
{
	link_usage_.resize(virtual_link_number_);
	for(int i=0;i<virtual_link_number_;++i){
		link_usage_[i].resize(node_number_, -1);
	}
	buffer_count_.resize(node_number_, 0);
	buffer_.resize(node_number_);
	for(int i=0; i<node_number_; ++i){
		buffer_[i].resize(physical_port_);
	}
	wheel_.resize(node_number_, 0);
	wheel_lock_.resize(node_number_, false);
	receive_who_.resize(node_number_, -1);
	local_time_.resize(node_number_, -1);
	use_which_link_.resize(node_number_, -1);
}


void Ring::add_flit_(mess_event event, int node_id, int port)
{
	++buffer_count_[node_id];
	buffer_[node_id][port].push_back(event);
}

void Ring::remove_flit_(int node_id, int port)
{
	--buffer_count_[node_id];
	buffer_[node_id][port].erase(buffer_[node_id][port].begin());
}


// for new, link_usage_ just using as a link.
void Ring::ring_travel_()
{
	time_type current_time = mess_queue::m_pointer().current_time();
	for(int i=0; i<node_number_; ++i){
		if(buffer_count_[i]){
			while(wheel_lock_[i] == false){
				if(buffer_[i][wheel_[i]].size() != 0){
					wheel_lock_[i] = true;
					break;
				}
				wheel_[i] = (wheel_[i] +1) % physical_port_;
			}

			if(buffer_[i][wheel_[i]].size() > 0){
				mess_event m(*(buffer_[i][wheel_[i]].begin()));
				add_type des = m.des();
				add_type src = m.src();
				flit_template flit_t(m.get_flit());
				time_type event_time = m.event_start();

				if(event_time <= current_time){
					ring_node_add_type des_add = sim_foundation::wsf().three_d_to_ring_(des);
					int node_id = des_add[2];
					if(receive_who_[node_id] == i || receive_who_[node_id] == -1){
						if(check_and_set_link(i, node_id)){
							mess_queue::wm_pointer().add_message(mess_event(current_time+RING_DELAY_, RING_, src, des, 7, 0, flit_t));
							remove_flit_(i, wheel_[i]);
							if(flit_t.type() == HEADER_){
								receive_who_[node_id] = i;
							}else {
								if(flit_t.type() == TAIL_){
									receive_who_[node_id] = -1;
									free_link(i, node_id);
									wheel_[i] = (wheel_[i] +1) % physical_port_;
									wheel_lock_[i] = false;
								}
							}
						}
					}
				}
			}

		}
	}
}

bool Ring::check_and_set_link(int src, int des)
{
	if(use_which_link_[src] == -1){
		int min = src;
		int max = des;
		if(min > des){
			min = des;
			max = src;
		}
		int start = min;
		int end = max;
		if((max - min) > node_number_/2){
			start = max;
			end = min;
		}
		int s,e;
		bool find_link;
		for(int i=0; i < virtual_link_number_; ++i){
			s = start;
			e = end;
			find_link = false;
			while(s != e){
				if(link_usage_[i][s] != -1 && link_usage_[i][s] != src) break;
				s = (s+1)%node_number_;
				if(s == e){
					find_link = true;
				}
			}
			if(find_link){
				while(start != end){
					link_usage_[i][start] = src;
					start = (start + 1) % node_number_;
				}
				use_which_link_[src] = i;
				return true;
			}
		}
		return false;
	}
	return true;
}

void Ring::free_link(int src,int des)
{
	int min = src;
	int max = des;
	if(min > des){
		min = des;
		max = src;
	}
	int start = min;
	int end = max;
	if((max - min) > node_number_/2){
		start = max;
		end = min;
	}
	int l = use_which_link_[src];
	while(start != end){
		Sassert(link_usage_[l][start] == src);
		link_usage_[l][start] = -1;
		start = (start + 1) % node_number_;
	}
	use_which_link_[src] = -1;
}

