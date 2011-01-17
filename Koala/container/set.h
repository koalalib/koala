#ifndef KOALA_SET
#define KOALA_SET

#include <vector>

namespace Koala {
/*template<class Date>
class Set {
public:
	virtual bool add(const Date &) = 0;
	virtual bool is_element(const Date &) = 0;
	virtual bool is_empty() = 0;
	virtual int size() = 0;
	virtual bool del(const Date &) = 0;
	virtual ~Set(){};
	virtual bool operator*(const Set<Date>&) = 0;//intersection
	virtual bool operator+(const Set<Date>&) = 0;//sum(union)
	virtual bool operator-(const Set<Date>&) = 0;//difference
};*/

template<class Data>
class Set {// : public Set<Date> {
public:
	Set() {};
//	Set(int n) : tab(n){}
	Set(std::vector<Data> t): tab(t) {}
	~Set() {};
	bool add(const Data &);
	bool del(const Data &);
	bool isElement(const Data &) const;
	bool isEmpty() const;
	int size() const;
	Data at(int) const;
	void add(const Data*, const Data*); //add table
	Set operator+(Set&);
	Set operator*(Set&);
	Set operator-(Set&);
//private:
	std::vector<Data> tab;
};

#include "set.hpp"

}
#endif
