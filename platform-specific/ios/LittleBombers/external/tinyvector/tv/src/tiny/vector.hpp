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

#ifndef TV_TINY_VECTOR_HPP
#define TV_TINY_VECTOR_HPP

/* === all classes === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_> struct vector_exp;
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int FOp_> struct vector_bin_exp;
template<typename LTp_, typename RTp_, unsigned int BIn_, unsigned int EIn_> struct meta_assign;
template<typename LTp_, typename RTp_, unsigned int In_, unsigned int IEn_, bool Fin_> struct meta_unroll_dot;
template<typename ITp_, unsigned int INe_, typename LTp_, typename RTp_> struct meta_long_dot;
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int Size_> struct meta_short_dot;
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int Size_, bool Small_> struct meta_dot;

/* === vector expression: identity === */
template<typename Tp_, unsigned int Ne_, typename OTp_>
struct vector_exp<Tp_, Ne_, OTp_, TV::packet::ID> {
	typedef typename TV::packet::exp_ret<typename OTp_::packet, TV::packet::ID>::RET packet;
	
	OTp_ const & o_;
	
	inline vector_exp (OTp_ const & o): o_(o) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return TV::packet::id(o_[index]);
	}
};
/* === specialization to write less code === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int FOp_>
struct vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, LTp_, RTp_, FOp_>, TV::packet::ID> {
	typedef typename TV::packet::exp_ret<typename vector_bin_exp<Tp_, Ne_, LTp_, RTp_, FOp_>::packet, TV::packet::ID>::RET packet;
	
	vector_bin_exp<Tp_, Ne_, LTp_, RTp_, FOp_> const o_;
	
	inline vector_exp (LTp_ const & l, RTp_ const & r): o_(l, r) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return TV::packet::id(o_[index]);
	}
};

/* === vector binary expression: sum === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct vector_bin_exp<Tp_, Ne_, LTp_, RTp_, TV::packet::SUM> {
	typedef typename TV::packet::bin_exp_ret<typename LTp_::packet, typename RTp_::packet, TV::packet::SUM>::RET packet;
	
	LTp_ const & l_;
	RTp_ const & r_;
	
	inline vector_bin_exp (LTp_ const & l, RTp_ const & r) : l_(l), r_(r) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return l_[index] + r_[index];
	}
};

/* === vector binary expression: sub === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct vector_bin_exp<Tp_, Ne_, LTp_, RTp_, TV::packet::SUB> {
	typedef typename TV::packet::bin_exp_ret<typename LTp_::packet, typename RTp_::packet, TV::packet::SUB>::RET packet;
	
	LTp_ const & l_;
	RTp_ const & r_;
	
	inline vector_bin_exp (LTp_ const & l, RTp_ const & r) : l_(l), r_(r) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return l_[index] - r_[index];
	}
};

/* === vector binary expression: mul === */
template<typename Tp_, unsigned int Ne_, typename LTp_>
struct vector_bin_exp<Tp_, Ne_, LTp_, Tp_, TV::packet::MUL> {
	typedef typename TV::storage::fixed<Tp_, Ne_>::packet RTp_;
	typedef typename TV::packet::bin_exp_ret<typename LTp_::packet, typename RTp_::packet, TV::packet::MUL>::RET packet;
	
	LTp_ const & l_;
	RTp_ const r_;
	
	inline vector_bin_exp (LTp_ const & l, Tp_ const & r) : l_(l), r_(r) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return l_[index] * r_;
	}
};

/* === vector binary expression: div === */
template<typename Tp_, unsigned int Ne_, typename LTp_>
struct vector_bin_exp<Tp_, Ne_, LTp_, Tp_, TV::packet::DIV> {
	typedef typename TV::storage::fixed<Tp_, Ne_>::packet RTp_;
	typedef typename TV::packet::bin_exp_ret<typename LTp_::packet, typename RTp_::packet, TV::packet::DIV>::RET packet;
	
	LTp_ const & l_;
	RTp_ const r_;
	
	inline vector_bin_exp (LTp_ const & l, Tp_ const & r) : l_(l), r_(r) {
	}
	inline packet const operator[] (unsigned int const & index) const {
		return l_[index] * r_;
	}
};

/* === meta unrolling === */
template<typename LTp_, typename RTp_, unsigned int BIn_, unsigned int EIn_>
struct meta_assign {
	static inline void RET(LTp_ & l, RTp_ const & r) {
		l[BIn_] = r[BIn_];
		meta_assign<LTp_, RTp_, BIn_+1, EIn_>::RET(l, r);
	}
};
template<typename LTp_, typename RTp_, unsigned int EIn_>
struct meta_assign<LTp_, RTp_, EIn_, EIn_> {
	static inline void RET(LTp_ & l, RTp_ const & r) {
		l[EIn_] = r[EIn_];
	}
};

/* === main class === */
template<typename Tp_, unsigned int Ne_>
class vector {
	private:
		/* === packet type to store scalar type === */
		typedef TV::storage::fixed<Tp_, Ne_> store;
		
	public:
		typedef typename store::packet packet;
		
		/* === begin and end for loop === */
		enum {
			begin = store::begin,
			end   = store::end
		};
	
	private:
		/* === access packet === */
		inline packet & operator[] (unsigned int const & index) {
			return s_[index];
		}
		inline packet const & operator[] (unsigned int const & index) const {
			return s_[index];
		}
	
		/* === friend classes === */
		template<typename ITp_, unsigned int INe_, typename OTp_, unsigned int FOp_> friend struct vector_exp;
		template<typename ITp_, unsigned int INe_, typename LTp_, typename RTp_, unsigned int FOp_> friend struct vector_bin_exp;
		template<typename LTp_, typename RTp_, unsigned int BIn_, unsigned int EIn_> friend struct meta_assign;
		template<typename LTp_, typename RTp_, unsigned int In_, unsigned int IEn_, bool Fin_> friend struct meta_unroll_dot;
		template<typename ITp_, unsigned int INe_, typename LTp_, typename RTp_> friend struct meta_long_dot;
		template<typename ITp_, unsigned int INe_, typename LTp_, typename RTp_, unsigned int Size_> friend struct meta_short_dot;
		
		/* === friend function === */
		template<typename ITp_, unsigned int INe_> friend inline ITp_ dot (vector<ITp_, INe_> const &, vector<ITp_, INe_> const &); 
	
	public:
		/* === constructor === */
		inline vector (Tp_ const & value = Tp_()) : s_(value) {
		}
		inline vector (vector<Tp_, Ne_> const & vect): s_(vect.s_) {
		}
		template<typename OTp_, unsigned int FOp_>
		inline vector (vector_exp<Tp_, Ne_, OTp_, FOp_> const & exp) {
			meta_assign<store, vector_exp<Tp_, Ne_, OTp_, FOp_>, begin, end>::RET(s_, exp);
		}
		
		/* === destructor === */
		inline ~vector () {
		}
		
		/* === access value === */
		inline Tp_ & operator() (unsigned int const & index) {
			return s_(index);
		}
		inline Tp_ const & operator() (unsigned int const & index) const {
			return s_(index);
		}
		
		/* === copy === */
		inline vector<Tp_, Ne_> & operator= (Tp_ const & value) {
			s_ = value;
			return *this;
		}
		inline vector<Tp_, Ne_> & operator= (vector<Tp_, Ne_> const & vect) {
			s_ = vect.s_;
			return *this;
		}
		template<typename OTp_, unsigned int FOp_>
		inline vector<Tp_, Ne_> & operator= (vector_exp<Tp_, Ne_, OTp_, FOp_> const & exp) {
			meta_assign<store, vector_exp<Tp_, Ne_, OTp_, FOp_>, begin, end>::RET(s_, exp);
			return *this;
		}
		
	private:
		store  s_;
};


/* === operator+ === 
     +---+---+---+ 
     |L\R| V | E |
     +---+---+---+ 
     | V | 1 | 2 | 
     +---+---+---+ 
     | E | 3 | 4 | 
     +---+---+---+ */
/* === 1 === */
template<typename Tp_, unsigned int Ne_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, TV::packet::SUM>, TV::packet::ID> const
operator+ (vector<Tp_, Ne_> const & l, vector<Tp_, Ne_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, TV::packet::SUM>, TV::packet::ID>(l, r);
}
/* === 2 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector_exp<Tp_, Ne_, OTp_, FOp_>, TV::packet::SUM>, TV::packet::ID> const
operator+ (vector<Tp_, Ne_> const & l, vector_exp<Tp_, Ne_, OTp_, FOp_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector_exp<Tp_, Ne_, OTp_, FOp_>, TV::packet::SUM>, TV::packet::ID>(l, r);
}
/* === 3 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, vector<Tp_, Ne_>, TV::packet::SUM>, TV::packet::ID> const
operator+ (vector_exp<Tp_, Ne_, OTp_, FOp_> const & l, vector<Tp_, Ne_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, vector<Tp_, Ne_>, TV::packet::SUM>, TV::packet::ID>(l, r);
}
/* === 4 === */
template<typename Tp_, unsigned int Ne_, typename O1Tp_, typename O2Tp_, unsigned int F1Op_, unsigned int F2Op_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, O1Tp_, F1Op_>, vector_exp<Tp_, Ne_, O2Tp_, F2Op_>, TV::packet::SUM>, TV::packet::ID> const
operator+ (vector_exp<Tp_, Ne_, O1Tp_, F1Op_> const & l, vector_exp<Tp_, Ne_, O2Tp_, F2Op_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, O1Tp_, F1Op_>, vector_exp<Tp_, Ne_, O2Tp_, F2Op_>, TV::packet::SUM>, TV::packet::ID>(l, r);
}

/* === operator- === 
     +---+---+---+ 
     |L\R| V | E |
     +---+---+---+ 
     | V | 1 | 2 | 
     +---+---+---+ 
     | E | 3 | 4 | 
     +---+---+---+ */
/* === 1 === */
template<typename Tp_, unsigned int Ne_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, TV::packet::SUB>, TV::packet::ID> const
operator- (vector<Tp_, Ne_> const & l, vector<Tp_, Ne_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, TV::packet::SUB>, TV::packet::ID>(l, r);
}
/* === 2 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector_exp<Tp_, Ne_, OTp_, FOp_>, TV::packet::SUB>, TV::packet::ID> const
operator- (vector<Tp_, Ne_> const & l, vector_exp<Tp_, Ne_, OTp_, FOp_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, vector_exp<Tp_, Ne_, OTp_, FOp_>, TV::packet::SUB>, TV::packet::ID>(l, r);
}
/* === 3 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, vector<Tp_, Ne_>, TV::packet::SUB>, TV::packet::ID> const
operator- (vector_exp<Tp_, Ne_, OTp_, FOp_> const & l, vector<Tp_, Ne_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, vector<Tp_, Ne_>, TV::packet::SUB>, TV::packet::ID>(l, r);
}
/* === 4 === */
template<typename Tp_, unsigned int Ne_, typename O1Tp_, typename O2Tp_, unsigned int F1Op_, unsigned int F2Op_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, O1Tp_, F1Op_>, vector_exp<Tp_, Ne_, O2Tp_, F2Op_>, TV::packet::SUB>, TV::packet::ID> const
operator- (vector_exp<Tp_, Ne_, O1Tp_, F1Op_> const & l, vector_exp<Tp_, Ne_, O2Tp_, F2Op_> const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, O1Tp_, F1Op_>, vector_exp<Tp_, Ne_, O2Tp_, F2Op_>, TV::packet::SUB>, TV::packet::ID>(l, r);
}

/* === operator* === 
     +---+---+---+ 
     |R\L| V | E |
     +---+---+---+ 
     | T | 1 | 2 | 
     +---+---+---+ */
/* === 1 === */
template<typename Tp_, unsigned int Ne_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, Tp_, TV::packet::MUL>, TV::packet::ID> const
operator* (vector<Tp_, Ne_> const & l, Tp_ const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, Tp_, TV::packet::MUL>, TV::packet::ID>(l, r);
}
/* === 2 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, Tp_, TV::packet::MUL>, TV::packet::ID> const
operator* (vector_exp<Tp_, Ne_, OTp_, FOp_> const & l, Tp_ const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, Tp_, TV::packet::MUL>, TV::packet::ID>(l, r);
}

/* === operator/ === 
     +---+---+---+ 
     |R\L| V | E |
     +---+---+---+ 
     | T | 1 | 2 | 
     +---+---+---+ */
/* === 1 === */
template<typename Tp_, unsigned int Ne_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, Tp_, TV::packet::MUL>, TV::packet::ID> const
operator/ (vector<Tp_, Ne_> const & l, Tp_ const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector<Tp_, Ne_>, Tp_, TV::packet::MUL>, TV::packet::ID>(l, 1./r);
}
/* === 2 === */
template<typename Tp_, unsigned int Ne_, typename OTp_, unsigned int FOp_>
inline vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, Tp_, TV::packet::MUL>, TV::packet::ID> const
operator/ (vector_exp<Tp_, Ne_, OTp_, FOp_> const & l, Tp_ const & r) {
	return vector_exp<Tp_, Ne_, vector_bin_exp<Tp_, Ne_, vector_exp<Tp_, Ne_, OTp_, FOp_>, Tp_, TV::packet::MUL>, TV::packet::ID>(l, 1./r);
}

/* === meta dot unrolling === */
template<typename LTp_, typename RTp_, unsigned int In_, unsigned int En_, bool Fin_>
struct meta_unroll_dot {
	typedef typename LTp_::packet lpacket;
	typedef typename RTp_::packet rpacket;
	typedef typename TV::packet::bin_exp_ret<lpacket, rpacket, TV::packet::MUL>::RET mret;
	typedef typename meta_unroll_dot<LTp_, RTp_, In_+4, En_, (In_+8<En_)>::packet opacket;
	typedef typename TV::packet::bin_exp_ret<mret, opacket, TV::packet::SUM>::RET packet;
	static inline packet const RET(LTp_ const & l, RTp_ const & r) {
		return (l[In_] * r[In_]) + meta_unroll_dot<LTp_, RTp_, In_+4, En_, (In_+8<En_)>::RET(l, r);
	}
};
template<typename LTp_, typename RTp_, unsigned int In_ , unsigned int En_>
struct meta_unroll_dot<LTp_, RTp_, In_, En_, false> {
	typedef typename LTp_::packet lpacket;
	typedef typename RTp_::packet rpacket;
	typedef typename TV::packet::bin_exp_ret<lpacket, rpacket, TV::packet::MUL>::RET packet;
	static inline packet const RET(LTp_ const & l, RTp_ const & r) {
		return (l[In_] * r[In_]);
	}
};

/* === meta long dot === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct meta_long_dot {
	static inline Tp_ RET (LTp_ const & l, RTp_ const & r) {
		typename vector<Tp_, Ne_>::packet t, t0, t1, t2, t3;
		t0 = meta_unroll_dot<LTp_, RTp_, vector<Tp_, Ne_>::begin+0u, vector<Tp_, Ne_>::end, vector<Tp_, Ne_>::begin+4u<vector<Tp_, Ne_>::end>::RET(l, r);
		t1 = meta_unroll_dot<LTp_, RTp_, vector<Tp_, Ne_>::begin+1u, vector<Tp_, Ne_>::end, vector<Tp_, Ne_>::begin+5u<vector<Tp_, Ne_>::end>::RET(l, r);
		t2 = meta_unroll_dot<LTp_, RTp_, vector<Tp_, Ne_>::begin+2u, vector<Tp_, Ne_>::end, vector<Tp_, Ne_>::begin+6u<vector<Tp_, Ne_>::end>::RET(l, r);
		t3 = meta_unroll_dot<LTp_, RTp_, vector<Tp_, Ne_>::begin+3u, vector<Tp_, Ne_>::end, vector<Tp_, Ne_>::begin+7u<vector<Tp_, Ne_>::end>::RET(l, r);
		t = t0 + t1 + t2 + t3;
		return t[0] + t[1] + t[2] + t[3];
	}
};

/* === meta short dot === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int Size_> struct meta_short_dot;

template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct meta_short_dot<Tp_, Ne_, LTp_, RTp_, 1u> {
	static inline Tp_ RET (LTp_ const & l, RTp_ const & r) {
		typename vector<Tp_, Ne_>::packet t;
		t = l[0u] * r[0u];
		return t[0] + t[1] + t[2] + t[3];
	}
};
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct meta_short_dot<Tp_, Ne_, LTp_, RTp_, 2u> {
	static inline Tp_ RET (LTp_ const & l, RTp_ const & r) {
		typename vector<Tp_, Ne_>::packet t;
		t = (l[0u] * r[0u]) + (l[1u] * r[1u]);
		return t[0] + t[1] + t[2] + t[3];
	}
};
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_>
struct meta_short_dot<Tp_, Ne_, LTp_, RTp_, 3u> {
	static inline Tp_ RET (LTp_ const & l, RTp_ const & r) {
		typename vector<Tp_, Ne_>::packet t;
		t = (l[0u] * r[0u]) + (l[1u] * r[1u]) + (l[2u] * r[2u]);
		return t[0] + t[1] + t[2] + t[3];
	}
};

/* === meta dot === */
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int Size_, bool Small_>
struct meta_dot {
	static inline Tp_ RET(LTp_ const & l, RTp_ const & r) {
		return meta_long_dot<Tp_ ,Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_> >::RET(l, r);
	}
};
template<typename Tp_, unsigned int Ne_, typename LTp_, typename RTp_, unsigned int Size_>
struct meta_dot<Tp_, Ne_, LTp_, RTp_, Size_, true> {
	static inline Tp_ RET(LTp_ const & l, RTp_ const & r) {
		return meta_short_dot<Tp_ ,Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, Size_>::RET(l, r);
	}
};

/* === dot === 
  +---+---+---+ 
  |L\R| V | E |
  +---+---+---+ 
  | V | 1 | 2 | 
  +---+---+---+ 
  | E | 3 | 4 | 
  +---+---+---+ */
template<typename Tp_, unsigned int Ne_>
inline Tp_
dot (vector<Tp_, Ne_> const & l, vector<Tp_, Ne_> const & r) {
	enum { size = vector<Tp_, Ne_>::store::size };
	return meta_dot<Tp_, Ne_, vector<Tp_, Ne_>, vector<Tp_, Ne_>, size, (size < 4u)>::RET(l, r);
}

#endif // TV_TINY_VECTOR_HPP
