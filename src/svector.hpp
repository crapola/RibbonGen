#ifndef SVECTOR_HPP_INCLUDED
#define SVECTOR_HPP_INCLUDED

#include <limits>
#include <vector>

#include <iostream>

//#define PNAME std::cout<<"Call to "<<__PRETTY_FUNCTION__<<'\n';
#define PNAME

// Disable warning about virtual destructor.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

template<class T,class Ta=std::allocator<T>>
class Svector: protected std::vector<T,Ta>
{
	typedef std::vector<T,Ta> Base;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reference reference;
	typedef typename Base::const_reference const_reference;
public:
	Svector():Base(),_from(MAXSIZE),_to(0) {}
	Svector(size_t s, const T& v,const Ta& a=Ta()):Base(s,v,a),
		_from(MAXSIZE),_to(0) {}
	Svector(size_t s):Base(s),_from(MAXSIZE),_to(0) {}

	Svector(const Svector&)=delete;
	Svector(Svector&&)=delete;

	using Base::at;
	using Base::back;
	using Base::begin;
	using Base::empty;
	using Base::end;
	using Base::front;
	using Base::reserve;
	using Base::resize;
	using Base::size;
	using Base::swap;

	// Access

	reference operator[](size_t n)
	{
		if (n<_from) _from=n;
		if (n>_to) _to=n;
		return Base::operator[](n);
	}

	const_reference operator[](size_t n) const
	{
		return Base::operator[](n);
	}

	// Erase

	iterator erase(iterator pos)
	{
		PNAME
		iterator it=Base::erase(pos);
		SetRange(pos-begin(),size()-1);
		return it;
	}

	iterator erase(iterator first, iterator last)
	{
		PNAME
		iterator it=Base::erase(first,last);
		SetRange(first-begin(),size()-1);
		return it;
	}

	// Insert

	iterator insert(iterator pos, const T& x)
	{
		PNAME
		iterator it=Base::insert(pos,x);
		SetRange(pos-begin(),size()-1);
		return it;
	}

	void insert(iterator pos,std::initializer_list<T> l)
	{
		PNAME
		Base::insert(pos,l);
		SetRange(pos-begin(),size()-1);
	}

	void insert(iterator pos, size_t n, const T& x)
	{
		PNAME
		Base::insert(pos,n,x);
		SetRange(pos-begin(),size()-1);
	}

	template<typename InputIterator>
	void insert(iterator pos, InputIterator first,
				InputIterator last)
	{
		PNAME
		Base::insert(pos,first,last);
		SetRange(pos-begin(),size()-1);
	}

	// Push/Pop

	void pop_back()
	{
		PNAME
		Base::pop_back();
		SetRange(_from,size()-1);

	}

	void push_back(const T& x)
	{
		PNAME
		Base::push_back(x);
		SetRange(size()-1,size()-1);
	}

	void push_back(T&& x)
	{
		PNAME
		Base::push_back(x);
		SetRange(size()-1,size()-1);
	}

	/* Resize

	void resize(size_t ns)
	{
		SetRange(size()-1,size()-1);
		Base::resize(ns);
	}*/
public:
	size_t From() const
	{
		return _from;
	}

	size_t RangeSize() const
	{
		if (!Pending()) return 0;
		return _to-_from+1;
	}

	bool Pending() const
	{
		return (_from<=_to);
	}

	void Reset()
	{
		PNAME
		_from=MAXSIZE;
		_to=0;
	}

	size_t To() const
	{
		return _to;
	}

	void Info()
	{
		if (Pending())
		{
			std::cout<<"Pending changes\n";
		}
		else
		{
			std::cout<<"No changes pending\n";
		}
		std::cout<<"From="<<_from<<" "<<"To="<<_to<<'\n';
	}
private:


	void SetRange(size_t f,size_t t)
	{
		//std::cout<<"SET RANGE from "<<f<<" to "<<t<<"\n";
		if (f<_from) _from=f;
		if (t>_to) _to=t;
		if (_to>=size()) _to=size()-1;// force

		//std::cout<<"RANGE is now "<<_from<<" to "<<_to<<"\n";
	}

	size_t _from,_to;	// Changed indices since last reset.
	static const size_t MAXSIZE=std::numeric_limits<size_t>::max();
};

#pragma GCC diagnostic pop

#endif // SVECTOR_HPP_INCLUDED
