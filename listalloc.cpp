#include<stdio.h>
#include<stdlib.h>
#include<utility>
#include<iostream>

#include"Koala/graph/graph.h"
#include"Koala/algorithm/search.h"



using namespace Koala;
using namespace std;

Graph<int, int> g;



//template <class T>
//class ListBlockListAllocator {
//    private:
//        BlockListMemPool<T>* pool;
//        template <class U,class W> class TestEqTypes;
//        template <class U> class TestEqTypes<U,U> {};
//    public:
//       ListBlockListAllocator(BlockListMemPool<T>* apool) : pool(apool) { assert(apool); }
//
//       template<class U> U *allocate()
//       { TestEqTypes<U,T> x; return (U*)pool->alloc(); };
//       template<class U> void deallocate(U *p)
//       { TestEqTypes<U,T> x; pool->dealloc((U*)p); };
//};


template <class T>
class AllocTest {
    public:

    typedef typename Privates::List<T,AllocTest<T> >::Node NodeType;

    NodeType x;

};



int main() {


//    BlockListMemPool<int> pool(7);
    //ListBlockListAllocator<int> all1(&pool),all2(&pool);
//    int* p1=all1.allocate<int>(),*p2=all1.allocate<int>(),*p3=all2.allocate<int>();

//    *p3=3;
//    all2.allocate<int>();
//    cout << *p3;
//    all1.deallocate(p1);all2.deallocate(p3);
//    cout << *p3;

    Privates::ListDefaultCPPAllocator all;


    Privates::ListBlockListAllocator<int> ball(40);

//    ListBlockListAllocator<int> ball2(100);
//    ball2=ball;

 {
	int i, j;
//	List<int> l(all);

    ball.allocate<Privates::ListNode<int> >();
    Privates::List<int,Privates::ListBlockListAllocator<int> > l,l2;
    l.init(ball);
	Privates::List<int,Privates::ListBlockListAllocator<int> >::iterator it;
	for(j = 0; j < 19; j++) {
		l.clear();
		for(i = 0; i < j; i++) l.push_back(17 - i);
		l.sort();
		for(it = l.begin(); it != l.end(); ++it) {
			printf("%d ", *it);
			};
		printf("\n");
		Privates::List<int,Privates::ListBlockListAllocator<int> > l2;
		l2=l;
        cout << l2;

		Privates::List<int> l3(all);
		l3.copy(l2);
		};

	};



	return 0;
};
