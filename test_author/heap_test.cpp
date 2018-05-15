#include <iostream>
#include <string>

#include "Koala/container/heap.h"

using namespace std;
using namespace Koala;


struct Cos {
    int a;
    Cos() {std::cout << "\nCos\n";}
};

void FibonTest1()
{
    SimplArrPool<FibonHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<FibonHeapNode<int> > > heap(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);

    FibonHeap<int> heap, heap2;

	heap.push(10), heap.push(12), heap.push(20);
	FibonHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	FibonHeap<int>::Repr test2 = heap2.push(35);

    FibonHeap<int> heap3(heap);
//	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;
    FibonHeap<int> heap4;
	heap4=heap;
}

void FibonTest1Loc()
{
    SimplArrPool<FibonHeapNode<int> > alloc(100);
//    Privates::DefaultCPPAllocator stdAlloc;
    FibonHeap<int,std::less<int> > heap(&alloc),heap2(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);


	heap.push(10), heap.push(12), heap.push(20);
	FibonHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	FibonHeap<int>::Repr test2 = heap2.push(35);

    FibonHeap<int,std::less<int> > heap3(heap),heap4,heap5;
//	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;

	heap4.push(102);
	heap5.push(103);
//	heap2.merge(heap5);
}


void FibonTest2()
{
    FibonHeap<int> heap;
	heap.push(40), heap.push(30), heap.push(20);

    FibonHeap<int> heap2(heap);
	FibonHeap<int>::Repr test = heap.push(10), test2 = heap.push(40);
	heap2.pop();

    FibonHeap<int> heap3(heap2);
	heap.del(test), heap.decrease(test2, 15);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();

	heap2.merge(heap3);
	heap.merge(heap2);
	cout << endl;
}

void BinomTest1()
{
    SimplArrPool<BinomHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    BinomHeap<int,std::less<int>,Privates::BlockListAllocator<BinomHeapNode<int> > > heap(&alloc);
//    BinomHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);

    BinomHeap<int> heap, heap2;

	heap.push(10), heap.push(12), heap.push(20);
	BinomHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	BinomHeap<int>::Repr test2 = heap2.push(35);

    BinomHeap<int> heap3(heap);
//	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);



    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;

  BinomHeap<int> heap4(heap);
//	heap4=heap;
}

void BinomTest1Loc()
{
    SimplArrPool<BinomHeapNode<int> > alloc(100);
//    Privates::DefaultCPPAllocator stdAlloc;
    BinomHeap<int,std::less<int>  > heap(&alloc),heap2(&alloc);
//    BinomHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);


	heap.push(10), heap.push(12), heap.push(20);
	BinomHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	BinomHeap<int>::Repr test2 = heap2.push(35);

    BinomHeap<int,std::less<int> > heap3(heap), heap4,heap5;
//	heap3.check();


	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;

	heap4.push(102);
	heap5.push(103);
//	heap2.merge(heap5);
}


void BinomTest2()
{
    BinomHeap<int> heap;
	heap.push(40), heap.push(30), heap.push(20);

    BinomHeap<int> heap2(heap);
	BinomHeap<int>::Repr test = heap.push(10), test2 = heap.push(40);
	heap2.pop();

    BinomHeap<int> heap3(heap2);
	heap.del(test), heap.decrease(test2, 15);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();

	heap2.merge(heap3);
	heap.merge(heap2);
	cout << endl;

}

void PairTest1()
{
    SimplArrPool<PairHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<FibonHeapNode<int> > > heap(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);

    PairHeap<int> heap, heap2;

	heap.push(10), heap.push(12), heap.push(20);
	PairHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	PairHeap<int>::Repr test2 = heap2.push(35);

    PairHeap<int> heap3(heap);
//	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;
    PairHeap<int> heap4;
	heap4=heap;
}

void PairTest1Loc()
{
    SimplArrPool<PairHeapNode<int> > alloc(100);
//    Privates::DefaultCPPAllocator stdAlloc;
    PairHeap<int,std::less<int> > heap(&alloc),heap2(&alloc);
//    PairHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);


	heap.push(10), heap.push(12), heap.push(20);
	PairHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	PairHeap<int>::Repr test2 = heap2.push(35);

    PairHeap<int,std::less<int> > heap3(heap),heap4,heap5;
//	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;

	heap4.push(102);
	heap5.push(103);
//	heap2.merge(heap5);

}

void PairTest2()
{
    PairHeap<int> heap;
	heap.push(40), heap.push(30), heap.push(20);

    PairHeap<int> heap2(heap);
	PairHeap<int>::Repr test = heap.push(10), test2 = heap.push(40);
	heap2.pop();

    PairHeap<int> heap3(heap2);
	heap.del(test), heap.decrease(test2, 15);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();

	heap2.merge(heap3);
	heap.merge(heap2);
	cout << endl;
}

#include "main.hpp"
	FibonTest1();
	FibonTest1Loc();
	BinomTest1();
	BinomTest1Loc();
	PairTest1();
	PairTest1Loc();

	FibonTest2();
	BinomTest2();
	PairTest2();


    cout << "\n-------------\n";
    BinomHeap<int> heap, heap2,heap4;

	heap.push(10), heap.push(12);

	cout << heap.topRepr() << ' ' <<  heap.topRepr()->get() <<'\n';

	heap2.push(11), heap2.push(7), heap2.push(20); heap2.push(35);
	heap4=heap=heap2;
cout << "\n-------------\n";
    BinomHeap<int> heap3(heap2);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;

    while (!heap3.empty())
        cout << heap3.top() << " ", heap3.pop();

	heap4.clear();
	cout << endl << heap4.size();
cout << "\n-------------\n";
    int bufor[10],data[5]={3,4,1,3,5};
    BinomHeap<int> ii;

    ii.assign(data,data+5);

    while (!ii.empty())
    {
        cout << ii.top() << " ";
        ii.pop();
    }

	return 0;
}
