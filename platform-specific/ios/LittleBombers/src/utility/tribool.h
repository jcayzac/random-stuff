#if !defined(UUID_436F424AEE3B4513886525842F9AD37B)
#define UUID_436F424AEE3B4513886525842F9AD37B

struct tribool {
	enum value_t { _maybe=-1, _false, _true };
	value_t value;
	tribool(value_t v): value(v) { }
	tribool(const tribool& o): value(o.value) { }
	tribool(bool v): value(v?_true:_false) { }
};

inline bool operator==(bool lhs, const tribool& rhs) {
	return lhs?(rhs.value==tribool::_true):(rhs.value==tribool::_false);
}
inline bool operator==(const tribool& lhs, bool rhs) { return (rhs==lhs); }
inline bool operator!=(bool lhs, const tribool& rhs) { return !(lhs==rhs); }
inline bool operator!=(const tribool& lhs, bool rhs) { return (rhs!=lhs); }
#define maybe tribool::_maybe

#endif // UUID_436F424AEE3B4513886525842F9AD37B

