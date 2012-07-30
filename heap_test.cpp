#include <iostream>
#include <string>

#include "koala/container/heap.h"

using namespace std;
using namespace Koala;


struct Cos {
    int a;
    Cos() {std::cout << "\nCos\n";}
};

void FibonTest1()
{
    Privates::BlockListAllocator<FibonHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<FibonHeapNode<int> > > heap(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);

    FibonHeap<int> heap, heap2;

	heap.push(10), heap.push(12), heap.push(20);
	FibonHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	FibonHeap<int>::Repr test2 = heap2.push(35);

    FibonHeap<int> heap3(heap);
	heap3.check();

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
    Privates::BlockListAllocator<FibonHeapNode<int> > alloc(100);
//    Privates::DefaultCPPAllocator stdAlloc;
    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<FibonHeapNode<int> > > heap(&alloc),heap2(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);


	heap.push(10), heap.push(12), heap.push(20);
	FibonHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	FibonHeap<int>::Repr test2 = heap2.push(35);

    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<FibonHeapNode<int> > > heap3(heap);
	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;
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
    Privates::BlockListAllocator<BinomHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    BinomHeap<int,std::less<int>,Privates::BlockListAllocator<BinomHeapNode<int> > > heap(&alloc);
//    BinomHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);

    BinomHeap<int> heap, heap2;

	heap.push(10), heap.push(12), heap.push(20);
	BinomHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	BinomHeap<int>::Repr test2 = heap2.push(35);

    BinomHeap<int> heap3(heap);
	heap3.check();

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
    Privates::BlockListAllocator<BinomHeapNode<int> > alloc(100);
//    Privates::DefaultCPPAllocator stdAlloc;
    BinomHeap<int,std::less<int>,Privates::BlockListAllocator<BinomHeapNode<int> > > heap(&alloc),heap2(&alloc);
//    BinomHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);


	heap.push(10), heap.push(12), heap.push(20);
	BinomHeap<int>::Repr test = heap.push(30);

    heap2.push(10), heap2.push(12), heap2.push(20);
	BinomHeap<int>::Repr test2 = heap2.push(35);

    BinomHeap<int,std::less<int>,Privates::BlockListAllocator<BinomHeapNode<int> > > heap3(heap);
	heap3.check();

	heap.decrease(test, 11);
	heap.del(test);
	heap.merge(heap2);
	heap.del(test2);
	heap.merge(heap3);

    while (!heap.empty())
        cout << heap.top() << " ", heap.pop();
	cout << endl;
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

int main()
{

	FibonTest1();
	FibonTest1Loc();
	BinomTest1();

	FibonTest2();
	BinomTest1Loc();
	BinomTest2();


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


	return 0;
}
