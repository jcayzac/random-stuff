#if !defined(UUID_3A349C6703B94C2AB82F82652C82149D)
#define UUID_3A349C6703B94C2AB82F82652C82149D

#include <list>

template<typename T> 
class countable_list: public std::list<T> {
public:
	typedef typename std::list<T>						base_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	
private:
	size_type mSize;
public:
	countable_list(): mSize(0) { }
	countable_list ( const countable_list<T>& x ): base_type(x), mSize(x.mSize) { }
	
	template <class II>
	void assign(II first, II last) { base_type::assign(first,last); mSize=std::distance(first,last); }
	
	size_type size() const										{ return mSize; }
	void resize (size_type sz)									{ base_type::resize(sz,T()); mSize=sz; }
	void assign ( size_type n, const T& u )						{ base_type::assign(n,u); mSize=n; }
	void push_front ( const T& x )								{ base_type::push_front(x); ++mSize; }
	void pop_front ( )											{ base_type::pop_front(); --mSize; }
	void push_back ( const T& x )								{ base_type::push_back(x); ++mSize; }
	void pop_back ( )											{ base_type::pop_back(); --mSize; }
	iterator insert ( iterator position, const T& x )			{ base_type::insert(position,x); ++mSize; }
    void insert ( iterator position, size_type n, const T& x )	{ base_type::insert(position,n,x); mSize+=n; }
	iterator erase ( iterator position )						{ base_type::erase(position); --mSize; }
	void clear()												{ base_type::clear(); mSize=0; }

	template <class InputIterator>
    void insert ( iterator position, InputIterator first, InputIterator last ) {
		base_type::insert(position,first,last);
		mSize+=std::distance(first,last);
	}

	iterator erase ( iterator first, iterator last ) {
		mSize-=std::distance(first,last);
		base_type::erase(first,last);
	}
	
	void swap ( countable_list<T>& lst ) {
		base_type::swap(lst);
		size_type tmp(mSize);
		mSize     = lst.mSize;
		lst.mSize = tmp;
	}

private:
	// Not implemented
	void splice ( iterator position, countable_list<T>& x );
	void splice ( iterator position, countable_list<T>& x, iterator i );
	void splice ( iterator position, countable_list<T>& x, iterator first, iterator last );
	void remove ( const T& value );
	template <class Predicate> void remove_if ( Predicate pred );
	void unique ( );
	template <class BinaryPredicate>
	void unique ( BinaryPredicate binary_pred );
	void merge ( countable_list<T>& x );
	template <class Compare>
	void merge ( countable_list<T>& x, Compare comp );
};

#endif // UUID_3A349C6703B94C2AB82F82652C82149D

