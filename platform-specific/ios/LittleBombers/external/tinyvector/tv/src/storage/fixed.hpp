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

#ifndef TV_FIXED_HPP
#define TV_FIXED_HPP

/* === loop unrolling to copy a vector === */
template<typename LTp_, typename RTp_, unsigned int In_, unsigned int En_>
struct fixed_meta_assign_vector {
	static inline void RET (LTp_ & l, RTp_ const & r) {
		l[In_] = r[In_];
		fixed_meta_assign_vector<LTp_, RTp_, In_+1, En_>::RET(l, r);
	}
};
template<typename LTp_, typename RTp_, unsigned int In_>
struct fixed_meta_assign_vector<LTp_, RTp_, In_, In_> {
	static inline void RET (LTp_ & l, RTp_ const & r) {
		l[In_] = r[In_];
	}
};

/* === loop unrolling to copy a value === */
template<typename LTp_, typename RTp_, unsigned int In_, unsigned int En_>
struct fixed_meta_assign_value {
	static inline void RET (LTp_ & l, RTp_ const & r) {
		l[In_] = r;
		fixed_meta_assign_value<LTp_, RTp_, In_+1, En_>::RET(l, r);
	}
};
template<typename LTp_, typename RTp_, unsigned int In_>
struct fixed_meta_assign_value<LTp_, RTp_, In_, In_> {
	static inline void RET (LTp_ & l, RTp_ const & r) {
		l[In_] = r;
	}
};

/* === main class === */
template<typename Tp_, unsigned int Ne_>
class fixed {
	public:
		typedef typename type_selector<Tp_>::packet packet;
		
	public:
		enum { packet_size = type_selector<Tp_>::size };
		enum { size = ((Ne_ - 1)  / packet_size) + 1 };
	
	public:
		/* === begin and end for loop === */
		enum {
			begin = 0, 
			end   = size-1
		};
	
	public:
		/* === constructor === */
		inline fixed () {
		}
		inline fixed (Tp_ const & value) {
			fixed_meta_assign_value<packet[size], Tp_, begin, end>::RET(s_, value);
		}
		inline fixed (fixed<Tp_, Ne_> const & store) {
			fixed_meta_assign_vector<packet[size], packet[size], begin, end>::RET(s_, store.s_);
		}
		
		/* === destructor === */
		inline ~fixed () {
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
		inline fixed<Tp_, Ne_> & operator= (Tp_ const & value) {
			fixed_meta_assign_value<packet[size], Tp_, begin, end>::RET(s_, value);
			return *this;
		}
		inline fixed<Tp_, Ne_> & operator= (fixed<Tp_, Ne_> const & store) {
			fixed_meta_assign_vector<packet[size], packet[size], begin, end>::RET(s_, store.s_);
			return *this;
		}
		
	private:
		packet s_[size];
};

#endif // TV_FIXED_HPP
