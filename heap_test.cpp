#include <iostream>
#include <string>

#include "koala/container/heap.h"

using namespace std;
using namespace Koala;


struct Cos {
    int a;
    Cos() {std::cout << "\nCos\n";}
};

int main()
{
    Privates::BlockListAllocator<Privates::FibonHeapNode<int> > alloc(4);
//    Privates::DefaultCPPAllocator stdAlloc;
//    FibonHeap<int,std::less<int>,Privates::BlockListAllocator<Privates::FibonHeapNode<int> > > heap(&alloc);
//    FibonHeap<int,std::less<int>,Privates::DefaultCPPAllocator  > heap(&stdAlloc);
    FibonHeap<int> heap;
    FibonHeap<int> heap2;


	heap.push(10);
	heap.push(12), heap.push(20);

    heap2.push(10), heap2.push(12), heap2.push(20);
	FibonHeap<int>::Repr test = heap.push(30);


	heap.decrease(test, 11);
    cout << heap.size() << endl;


	heap.merge(heap2);

//	heap.del(test);

	cout << heap.size() << endl<<endl;



    while (!heap.empty())
    {
        cout << heap.top() << endl, heap.pop();
    }

//    Privates::BlockListAllocator<Cos > a(4);
//    cout << "Kreacja";
//    Cos *c =a.allocate<Cos>();

	return 0;
}
