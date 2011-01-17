
template<class Data>
bool
Set<Data>::add(const Data &d) {
	int l = 0;
	int r = tab.size();
	while(l<tab.size() && l<=r) {
		int m = (l+r) >> 1;
		if(tab[m]>d)
			r = m - 1;
		else if(tab[m]<d)
			l = m + 1;
		else
			return false;
	}
	tab.insert(tab.begin()+l, d);
	return true;
}

template<class Data>
bool
Set<Data>::del(const Data &d) {
	int l = 0;
	int r = tab.size();
	while(l<tab.size() && l<=r) {
		int m = (l+r) >> 1;
		if(tab[m]>d)
			r = m - 1;
		else if(tab[m]<d)
			l = m + 1;
		else {
			tab.erase(tab.begin() + m);
			return true;
		}
	}
	return false;
}

template<class Data>
bool
Set<Data>::isElement(const Data &d) const {
	int l = 0;
	int r = tab.size();

	while(l<tab.size() && l<=r) {
		int m = (l+r) >> 1;
		if(tab[m]>d)
			r = m - 1;
		else if(tab[m]<d)
			l = m + 1;
		else
			return true;
	}
	return false;
}

template<class Data>
bool
Set<Data>::isEmpty() const{
	return tab.empty();
}

template<class Data>
int
Set<Data>::size() const {
	return tab.size();
}

template<class Data>
Data
Set<Data>::at(int i) const {
	return tab[i];
}

template<class Data>
void
Set<Data>::add(const Data *beg, const Data *end) {
	while(beg!=end) {
		add(*beg);
		++beg;
	}
}

template<class Data>
Set<Data>
Set<Data>::operator+(Set<Data> &op2) {
	typename std::vector<Data>::iterator iter1, iter2, iter1_end, iter2_end;
	std::vector<Data> odp;
	iter1 = tab.begin();
	iter2 = op2.tab.begin();
	iter1_end = tab.end();
	iter2_end = op2.tab.end();
	while(iter1!=iter1_end&&iter2!=iter2_end) {
		if(*iter1<*iter2) {
			odp.push_back(*iter1);
			iter1++;
		} else {
			if(*iter1==*iter2) {
				odp.push_back(*iter1);
				iter1++;
				iter2++;
			} else {
				odp.push_back(*iter2);
				iter2++;
			}
		}
	}
	while(iter1!=iter1_end) {
		odp.push_back(*iter1);
		iter1++;
	}
	while(iter2!=iter2_end) {
		odp.push_back(*iter2);
		iter2++;
	}
	return Set<Data>(odp);
}

template<class Data>
Set<Data>
Set<Data>::operator*(Set<Data> &op2) {
	typename std::vector<Data>::iterator iter1, iter2, iter1_end, iter2_end;
	std::vector<Data> odp;
	iter1 = tab.begin();
	iter2 = op2.tab.begin();
	iter1_end = tab.end();
	iter2_end = op2.tab.end();
	while(iter1!=iter1_end&&iter2!=iter2_end) {
		if(*iter1<*iter2) {
			iter1++;
		} else {
			if(*iter1==*iter2) {
				odp.push_back(*iter1);
				iter1++;
				iter2++;
			} else {
				iter2++;
			}
		}
	}
	return Set<Data>(odp);
}

template<class Data>
Set<Data>
Set<Data>::operator-(Set<Data> &op2) {
	typename std::vector<Data>::iterator iter1, iter2, iter1_end, iter2_end;
	std::vector<Data> odp;
	iter1 = tab.begin();
	iter2 = op2.tab.begin();
	iter1_end = tab.end();
	iter2_end = op2.tab.end();
	while(iter1!=iter1_end&&iter2!=iter2_end) {
		if(*iter1<*iter2) {
			odp.push_back(*iter1);
			iter1++;
		} else {
			if(*iter1==*iter2) {
				iter1++;
				iter2++;
			} else {
				iter2++;
			}
		}
	}
	while(iter1!=iter1_end) {
		odp.push_back(*iter1);
		iter1++;
	}
	return Set<Data>(odp);
}

