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

#ifndef TV_V4F_HPP
#define TV_V4F_HPP

/* === v4f meta operations === */
struct v4f_id { static inline float RET(float const & o) { return o; } };
struct v4f_sum { static inline float RET(float const & l, float const & r) { return l + r; } };
struct v4f_sub { static inline float RET(float const & l, float const & r) { return l - r; } };
struct v4f_mul { static inline float RET(float const & l, float const & r) { return l * r; } };
struct v4f_div { static inline float RET(float const & l, float const & r) { return l / r; } };

/* === v4f meta expression === */
template<typename OTp_, typename FOp_>
struct v4f_exp {
	OTp_ const & o_;
	inline v4f_exp (OTp_ const & o) : o_(o) {}
	inline float operator[] (unsigned int const & index) const { return FOp_::RET(o_[index]); }
};
/* === v4f meta binary expression === */
template<typename LTp_, typename RTp_, typename FOp_>
struct v4f_bin_exp {
	LTp_ const & l_;
	RTp_ const & r_;
	inline v4f_bin_exp (LTp_ const & l, RTp_ const & r) : l_(l), r_(r) {}
	inline float operator[] (unsigned int const & index) const { return FOp_::RET(l_[index], r_[index]); }
};
/* === specialization to write less code === */
template<typename LTp_, typename RTp_, typename FOp_, typename GOp_>
struct v4f_exp<v4f_bin_exp<LTp_, RTp_, FOp_>, GOp_> {
	v4f_bin_exp<LTp_, RTp_, FOp_> const o_;
	inline v4f_exp(LTp_ const & l, RTp_ const & r) : o_(l, r) {}
	inline float operator[] (unsigned int const & index) const { return GOp_::RET(o_[index]); }
};

/* === main class === */
class v4f {
	public:
		typedef v4f packet;
	
	public:
		/* === constructor === */
		inline v4f () {
		}
		inline v4f (float const & value) {
			v_[0] = v_[1] = v_[2] = v_[3] = value;
		}
		inline v4f (v4f const & vec) {
			v_[0] = vec[0];
			v_[1] = vec[1];
			v_[2] = vec[2];
			v_[3] = vec[3];
		}
		template<typename OTp_, typename FOp_>
		inline v4f (v4f_exp<OTp_, FOp_> const & exp) {
			v_[0] = exp[0];
			v_[1] = exp[1];
			v_[2] = exp[2];
			v_[3] = exp[3];
		}
		
		/* === destructor === */
		inline ~v4f () {
		}
		
		/* === access === */
		inline float & operator[] (unsigned int const & index) {
			return v_[index];
		}
		inline float const & operator[] (unsigned int const & index) const {
			return v_[index];
		}
		
		/* === copy === */
		inline v4f & operator= (float const & value) {
			v_[0] = v_[1] = v_[2] = v_[3] = value;
			return *this;
		}
		inline v4f & operator= (v4f const & vec) {
			v_[0] = vec[0];
			v_[1] = vec[1];
			v_[2] = vec[2];
			v_[3] = vec[3];
			return *this;
		}
		template<typename OTp_, typename FOp_>
		inline v4f & operator= (v4f_exp<OTp_, FOp_> const & exp) {
			v_[0] = exp[0];
			v_[1] = exp[1];
			v_[2] = exp[2];
			v_[3] = exp[3];
			return *this;
		}
	
	private:
		float v_[4];
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
inline v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sum>, v4f_id> const
operator+ (v4f const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sum>, v4f_id>(l, r);
}
template<>
struct bin_exp_ret<v4f, v4f, SUM> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sum>, v4f_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sum>, v4f_id> const
operator+ (v4f const & l, v4f_exp<OTp_, FOp_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sum>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f, v4f_exp<OTp_, FOp_>, SUM> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sum>, v4f_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sum>, v4f_id> const
operator+ (v4f_exp<OTp_, FOp_> const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sum>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f_exp<OTp_, FOp_>, v4f, SUM> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sum>, v4f_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sum>, v4f_id> const
operator+ (v4f_exp<O1Tp_, F1Op_> const & l, v4f_exp<O2Tp_, F2Op_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sum>, v4f_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, SUM> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sum>, v4f_id> RET;
};


/* === operator- ===
     +---+---+---+
     |L\R| V | E |
     +---+---+---+
     | V | 1 | 2 |
     +---+---+---+
     | E | 3 | 4 |
     +---+---+---+ */

/* === 1 === */
inline v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sub>, v4f_id> const
operator- (v4f const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sub>, v4f_id>(l, r);
}
template<>
struct bin_exp_ret<v4f, v4f, SUB> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f, v4f_sub>, v4f_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sub>, v4f_id> const
operator- (v4f const & l, v4f_exp<OTp_, FOp_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sub>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f, v4f_exp<OTp_, FOp_>, SUB> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_sub>, v4f_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sub>, v4f_id> const
operator- (v4f_exp<OTp_, FOp_> const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sub>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f_exp<OTp_, FOp_>, v4f, SUB> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_sub>, v4f_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sub>, v4f_id> const
operator- (v4f_exp<O1Tp_, F1Op_> const & l, v4f_exp<O2Tp_, F2Op_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sub>, v4f_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, SUB> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_sub>, v4f_id> RET;
};


/* === operator* ===
     +---+---+---+
     |L\R| V | E |
     +---+---+---+
     | V | 1 | 2 |
     +---+---+---+
     | E | 3 | 4 |
     +---+---+---+ */

/* === 1 === */
inline v4f_exp<v4f_bin_exp<v4f, v4f, v4f_mul>, v4f_id> const
operator* (v4f const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f, v4f_mul>, v4f_id>(l, r);
}
template<>
struct bin_exp_ret<v4f, v4f, MUL> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f, v4f_mul>, v4f_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_mul>, v4f_id> const
operator* (v4f const & l, v4f_exp<OTp_, FOp_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_mul>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f, v4f_exp<OTp_, FOp_>, MUL> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_mul>, v4f_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_mul>, v4f_id> const
operator* (v4f_exp<OTp_, FOp_> const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_mul>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f_exp<OTp_, FOp_>, v4f, MUL> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_mul>, v4f_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_mul>, v4f_id> const
operator* (v4f_exp<O1Tp_, F1Op_> const & l, v4f_exp<O2Tp_, F2Op_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_mul>, v4f_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, MUL> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_mul>, v4f_id> RET;
};


/* === operator/ ===
     +---+---+---+
     |L\R| V | E |
     +---+---+---+
     | V | 1 | 2 |
     +---+---+---+
     | E | 3 | 4 |
     +---+---+---+ */

/* === 1 === */
inline v4f_exp<v4f_bin_exp<v4f, v4f, v4f_div>, v4f_id> const
operator/ (v4f const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f, v4f_div>, v4f_id>(l, r);
}
template<>
struct bin_exp_ret<v4f, v4f, DIV> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f, v4f_div>, v4f_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_div>, v4f_id> const
operator/ (v4f const & l, v4f_exp<OTp_, FOp_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_div>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f, v4f_exp<OTp_, FOp_>, DIV> {
	typedef v4f_exp<v4f_bin_exp<v4f, v4f_exp<OTp_, FOp_>, v4f_div>, v4f_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_div>, v4f_id> const
operator/ (v4f_exp<OTp_, FOp_> const & l, v4f const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_div>, v4f_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v4f_exp<OTp_, FOp_>, v4f, DIV> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<OTp_, FOp_>, v4f, v4f_div>, v4f_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_div>, v4f_id> const
operator/ (v4f_exp<O1Tp_, F1Op_> const & l, v4f_exp<O2Tp_, F2Op_> const & r) {
	return v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_div>, v4f_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, DIV> {
	typedef v4f_exp<v4f_bin_exp<v4f_exp<O1Tp_, F1Op_>, v4f_exp<O2Tp_, F2Op_>, v4f_div>, v4f_id> RET;
};


/* === functions === */

/* === id function === */
inline v4f id (v4f const & o) {
	return o;
}
template<>
struct exp_ret<v4f, ID> {
	typedef v4f RET;
};
template<typename OTp_, typename FOp_>
inline v4f_exp<OTp_, FOp_> id (v4f_exp<OTp_, FOp_> const & o) {
	return o;
}
template<>
template<typename OTp_, typename FOp_>
struct exp_ret<v4f_exp<OTp_, FOp_>, ID> {
	typedef v4f_exp<OTp_, FOp_> RET;
};

#endif // TV_V4F_HPP
