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

#ifndef TV_V2D_HPP
#define TV_V2D_HPP

/* === v2d meta operations === */
struct v2d_id { inline static double const RET(double const & o) { return o; } };
struct v2d_sum { inline static double const RET(double const & l, double const & r) { return l + r; } };
struct v2d_sub { inline static double const RET(double const & l, double const & r) { return l - r; } };
struct v2d_mul { inline static double const RET(double const & l, double const & r) { return l * r; } };
struct v2d_div { inline static double const RET(double const & l, double const & r) { return l / r; } };

/* === v2d meta expression === */
template<typename OTp_, typename FOp_>
struct v2d_exp {
	OTp_ const & o_;
	inline v2d_exp (OTp_ const & o) : o_(o) {}
	inline double const operator[] (unsigned int const & index) const { return FOp_::RET(o_[index]); }
};
/* === v2d meta binary expression === */
template<typename LTp_, typename RTp_, typename FOp_>
struct v2d_bin_exp {
	LTp_ const & l_;
	RTp_ const & r_;
	inline v2d_bin_exp (LTp_ const & l, RTp_ const & r) : l_(l), r_(r) {}
	inline double const operator[] (unsigned int const & index) const { return FOp_::RET(l_[index], r_[index]); }
};
/* === specialization to write less code === */
template<typename LTp_, typename RTp_, typename FOp_, typename GOp_>
struct v2d_exp<v2d_bin_exp<LTp_, RTp_, FOp_>, GOp_> {
	v2d_bin_exp<LTp_, RTp_, FOp_> const o_;
	inline v2d_exp(LTp_ const & l, RTp_ const & r) : o_(l, r) {}
	inline double const operator[] (unsigned int const & index) const { return GOp_::RET(o_[index]); }
};

class v2d {
	public:
		typedef v2d packet;
	
	public:
		/* === constructor === */
		inline v2d () {
		}
		inline v2d (double const & value) {
			v_[0] = v_[1] = value;
		}
		inline v2d (v2d const & vec) {
			v_[0] = vec[0];
			v_[1] = vec[1];
		}
		template<typename OTp_, typename FOp_>
		inline v2d (v2d_exp<OTp_, FOp_> const & exp) {
			v_[0] = exp[0];
			v_[1] = exp[1];
		}
		
		/* === destructor === */
		inline ~v2d () {
		}
		
		/* === access === */
		inline double & operator[] (unsigned int const & index) {
			return v_[index];
		}
		inline double const & operator[] (unsigned int const & index) const {
			return v_[index];
		}
		
		/* === copy === */
		inline v2d & operator= (double const & value) {
			v_[0] = v_[1] = value;
			return *this;
		}
		inline v2d & operator= (v2d const & vec) {
			v_[0] = vec[0];
			v_[1] = vec[1];
			return *this;
		}
		template<typename OTp_, typename FOp_>
		inline v2d & operator= (v2d_exp<OTp_, FOp_> const & exp) {
			v_[0] = exp[0];
			v_[1] = exp[1];
			return *this;
		}
	
	private:
		double v_[2];
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
inline v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sum>, v2d_id> const
operator+ (v2d const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sum>, v2d_id>(l, r);
}
template<>
struct bin_exp_ret<v2d, v2d, SUM> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sum>, v2d_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sum>, v2d_id> const
operator+ (v2d const & l, v2d_exp<OTp_, FOp_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sum>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d, v2d_exp<OTp_, FOp_>, SUM> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sum>, v2d_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sum>, v2d_id> const
operator+ (v2d_exp<OTp_, FOp_> const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sum>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d_exp<OTp_, FOp_>, v2d, SUM> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sum>, v2d_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sum>, v2d_id> const
operator+ (v2d_exp<O1Tp_, F1Op_> const & l, v2d_exp<O2Tp_, F2Op_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sum>, v2d_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, SUM> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sum>, v2d_id> RET;
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
inline v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sub>, v2d_id> const
operator- (v2d const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sub>, v2d_id>(l, r);
}
template<>
struct bin_exp_ret<v2d, v2d, SUB> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d, v2d_sub>, v2d_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sub>, v2d_id> const
operator- (v2d const & l, v2d_exp<OTp_, FOp_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sub>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d, v2d_exp<OTp_, FOp_>, SUB> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_sub>, v2d_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sub>, v2d_id> const
operator- (v2d_exp<OTp_, FOp_> const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sub>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d_exp<OTp_, FOp_>, v2d, SUB> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_sub>, v2d_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sub>, v2d_id> const
operator- (v2d_exp<O1Tp_, F1Op_> const & l, v2d_exp<O2Tp_, F2Op_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sub>, v2d_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, SUB> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_sub>, v2d_id> RET;
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
inline v2d_exp<v2d_bin_exp<v2d, v2d, v2d_mul>, v2d_id> const
operator* (v2d const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d, v2d_mul>, v2d_id>(l, r);
}
template<>
struct bin_exp_ret<v2d, v2d, MUL> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d, v2d_mul>, v2d_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_mul>, v2d_id> const
operator* (v2d const & l, v2d_exp<OTp_, FOp_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_mul>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d, v2d_exp<OTp_, FOp_>, MUL> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_mul>, v2d_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_mul>, v2d_id> const
operator* (v2d_exp<OTp_, FOp_> const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_mul>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d_exp<OTp_, FOp_>, v2d, MUL> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_mul>, v2d_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_mul>, v2d_id> const
operator* (v2d_exp<O1Tp_, F1Op_> const & l, v2d_exp<O2Tp_, F2Op_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_mul>, v2d_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, MUL> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_mul>, v2d_id> RET;
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
inline v2d_exp<v2d_bin_exp<v2d, v2d, v2d_div>, v2d_id> const
operator/ (v2d const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d, v2d_div>, v2d_id>(l, r);
}
template<>
struct bin_exp_ret<v2d, v2d, DIV> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d, v2d_div>, v2d_id> RET;
};
/* === 2 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_div>, v2d_id> const
operator/ (v2d const & l, v2d_exp<OTp_, FOp_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_div>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d, v2d_exp<OTp_, FOp_>, DIV> {
	typedef v2d_exp<v2d_bin_exp<v2d, v2d_exp<OTp_, FOp_>, v2d_div>, v2d_id> RET;
};
/* === 3 === */
template<typename OTp_, typename FOp_>
inline v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_div>, v2d_id> const
operator/ (v2d_exp<OTp_, FOp_> const & l, v2d const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_div>, v2d_id>(l, r);
}
template<>
template<typename OTp_, typename FOp_>
struct bin_exp_ret<v2d_exp<OTp_, FOp_>, v2d, DIV> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<OTp_, FOp_>, v2d, v2d_div>, v2d_id> RET;
};
/* === 4 === */
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
inline v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_div>, v2d_id> const
operator/ (v2d_exp<O1Tp_, F1Op_> const & l, v2d_exp<O2Tp_, F2Op_> const & r) {
	return v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_div>, v2d_id>(l, r);
}
template<>
template<typename O1Tp_, typename O2Tp_, typename F1Op_, typename F2Op_>
struct bin_exp_ret<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, DIV> {
	typedef v2d_exp<v2d_bin_exp<v2d_exp<O1Tp_, F1Op_>, v2d_exp<O2Tp_, F2Op_>, v2d_div>, v2d_id> RET;
};

/* === functions === */
/* === id function === */
inline v2d id (v2d const & o) {
	return o;
}
template<>
struct exp_ret<v2d, ID> {
	typedef v2d RET;
};
template<typename OTp_, typename FOp_>
inline v2d_exp<OTp_, FOp_> id (v2d_exp<OTp_, FOp_> const & o) {
	return o;
}
template<>
template<typename OTp_, typename FOp_>
struct exp_ret<v2d_exp<OTp_, FOp_>, ID> {
	typedef v2d_exp<OTp_, FOp_> RET;
};

#endif // TV_V2D_HPP
