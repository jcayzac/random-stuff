/*
 * Copyright (c) 2009, Penati Mattia
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Politecnico di Milano nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TV_DYNAMIC_HPP
#define TV_DYNAMIC_HPP

/* === main class === */
template<typename Tp_>
class dynamic {
	public:
		typedef typename type_selector<Tp_>::packet packet;
		
	private:
		enum { packet_size = type_selector<Tp_>::size };
	
	public:
	
	public:
		/* === constructor === */
		inline dynamic (unsigned int const & n): Ne_(n) {
			s_ = new packet[size()];
		}
		inline dynamic (unsigned int const & n, Tp_ const & value) : Ne_(n) {
			s_ = new(value) packet[size()];
		}
		inline dynamic (dynamic<Tp_> const & store) : Ne_(store.n) {
			s_ = new packet[size()];
			for (unsigned int i = begin(); i < end(); ++i)
				s_[i] = store[i];
		}
		
		/* === destructor === */
		inline ~dynamic () {
			delete[] s_;
		}
		
		/* === packet access === */
		inline packet & operator[] (unsigned int const & index) {
			return s_[index];
		}
		inline packet const & operator[] (unsigned int const & index) const {
			return s_[index];
		}
		
		/* === value access === */
		inline Tp_ & operator() (unsigned int const & index) {
			return s_[index / packet_size][index % packet_size];
		}
		inline Tp_ const & operator() (unsigned int const & index) const {
			return s_[index / packet_size][index % packet_size];
		}
		
		/* === copy === */
		inline dynamic<Tp_> & operator= (Tp_ const & value) {
			for (unsigned int i = begin(); i < end(); ++i)
				s_[i] = value;
			return *this;
		}
		inline dynamic<Tp_> & operator= (dynamic<Tp_> const & store) {
			if (Ne_ != store.Ne_)
				resize(store.Ne_);
			for (unsigned int i = begin(); i < end(); ++i)
				s_[i] = store[i];
			return *this;
		}
		
		/* === resize === */
		inline void resize(unsigned int const & n) {
			if (n != Ne_) {
				packet *new_s_ = new packet[n];
				Ne_ = (n < Ne_ ? n : Ne_);
				for (unsigned int i = begin(); i < end(); ++i)
					new_s_[i] = s_[i];
				Ne_ = (n < Ne_ ? Ne_ : n);
				delete[] s_;
				s_ = new_s_;
			}
		} 
		
	private:
		packet *s_;
		unsigned int Ne_;
		
		/* === calculate the size === */
		inline unsigned int size() {
			return ((Ne_ - 1)  / packet_size) + 1;
		}
	public:
		/* === begin and end for loop === */
		inline unsigned int begin() { return 0; }
		inline unsigned int end() { return size-1; } 
		
};

#endif // TV_DYNAMIC_HPP
