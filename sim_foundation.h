#ifndef SIM_FOUNDATION_
#define SIM_FOUNDATION_

#include "sim_router.h"
// karel: start.
#include "sim_ring.h"
// karel: end.
#include "flit.h"
#include "mess_event.h"
#include <vector>
#include "configuration.h"
#include "SRGen.h"
#include <exception>
#include <utility>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>

class sim_foundation {

	friend ostream& operator<<(ostream&, const sim_foundation&);

	private:

		vector<sim_router_template> inter_network_;
		long ary_size_;
		long cube_size_;
		long router_counter_;
		long packet_counter_;

		ifstream inFile_;

		static string file_name_;
		static sim_foundation * s_f_;

		// karel: start.
		vector<Ring> inter_ring_;
		int ring_node_;
		// karel: end.

	public:
		class file_error: public exception {
			public:
				file_error(const string & err) : what_(err) {}
				virtual const char * what() const throw() {return what_.c_str();}
				virtual ~file_error() throw();

			private:
				string what_;
		};

		// karel: start.
		// translate a 3D address to a ring address.
		ring_node_add_type three_d_to_ring_(add_type threed);
		add_type ring_to_3d_(ring_node_add_type rd);

		Ring & ring(const ring_node_add_type &a){
			return inter_ring_[a[0]*ary_size_+a[1]];
		}
		const Ring & ring(const ring_node_add_type &a) const{
			return inter_ring_[a[0]*ary_size_+a[1]];
		}
		// check src,and des whether in the same ring ,if not, assign th des to the next node in the same ring.
		bool is_inthesame_ring(const add_type & src,const add_type & nex);
		add_type find_next_node(const add_type& src, const add_type& des);

		void receive_RING_message(mess_event mesg);
		// karel: end.

		static const sim_foundation & sf() {return *s_f_;}
		static sim_foundation & wsf() {return *s_f_;}
		sim_foundation();
		~sim_foundation(){s_f_ = 0;}
		long ary_size() const {return ary_size_;}
		long cube_size() const {return cube_size_;}
		long packet_counter() {return packet_counter_;}
		long packet_counter() const {return packet_counter_;}

		vector<sim_router_template> & inter_network() {return inter_network_;}
		const vector<sim_router_template> & inter_network() const
	   												{return inter_network_;}
		sim_router_template & router(const add_type& a);
		const sim_router_template & router(const add_type& a) const;
		bool valid_address(const add_type & a) const;

		void receive_EVG_message(mess_event mesg);
		void receive_ROUTER_message(mess_event mesg);
		void receive_WIRE_message(mess_event mesg);
		void receive_CREDIT_message(mess_event mesg);

		void router_power_exchange();

		void simulation_results();
		void simulation_check();

		void init_file();

};
#endif
