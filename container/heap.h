#ifndef KOALA_BINOMIALHEAP_H
#define KOALA_BINOMIALHEAP_H

#include <cassert>
#include <functional>

#include "localarray.h"
#include "privates.h"

namespace Koala
{


namespace Privates {

    template <class Key>
    struct BinomHeapNode
    {
        BinomHeapNode<Key> *parent, *child, *next;
        unsigned degree;
        Key key;

        Key get() { return key; }

        inline BinomHeapNode(const Key& = Key());
        inline void insert(BinomHeapNode<Key> *);
        bool check() const;
    };

	template <class Key>
	inline BinomHeapNode<Key>::BinomHeapNode(const Key &key) : parent(0), child(0), next(0), degree(0), key(key) { }

	template <class Key>
	inline void BinomHeapNode<Key>::insert(BinomHeapNode<Key> *A)
	{
		A->parent = this, A->next = child, child = A, degree++;
	}


	template <class Key>
	bool BinomHeapNode<Key>::check() const
	{
		unsigned degree = 0;
		for(BinomHeapNode<Key> *child = this->child; child; child = child->next, degree++)
			if(child->parent != this || !child->check())
				return false;
		return degree == this->degree;
	}

}

	template <class Key, class Compare = std::less<Key>,class Allocator=Privates::DefaultCPPAllocator >
	class BinomHeap
	{
    public:
        typedef Privates::BinomHeapNode<Key> Node;
        typedef Privates::BinomHeapNode<Key> * Repr;

	protected:
		Node *root, *minimum;
		unsigned nodes;
		Compare function;
		Allocator* allocator;
	public:
		inline BinomHeap(const Compare& = Compare());
		inline BinomHeap(Allocator*,const Compare& = Compare());
		~BinomHeap();

		Key top() const;
		Node* push(const Key&);
		void pop();

		void decrease(Node*, const Key&);
		void del(Node*);

		void merge(BinomHeap&);
		void clear();

		unsigned size() const;
		bool empty() const;

		bool check() const;

	protected:

		inline Node* join(Node*, Node*);
		inline Node* reverse(Node*);
		inline Node* cut(Node*);
		void clear(Node*);

		Node* newNode(Key key)
		{
		    if (!allocator) return new Node(key);
		    Node* res=allocator->template allocate<Privates::BinomHeapNode<Key> >();
            res->key=key;
		    return res;
		}
		void delNode(Node* node)
		{
		    if (!allocator) delete node;
		    else allocator->deallocate(node);
		}

	};


	template <class Key, class Compare,class Allocator>
	inline BinomHeap<Key, Compare,Allocator>::BinomHeap(const Compare &function) : root(0), minimum(0), nodes(0), allocator(0), function(function) { }

	template <class Key, class Compare,class Allocator>
	inline BinomHeap<Key, Compare,Allocator>::BinomHeap(Allocator* all,const Compare &function) : root(0), minimum(0), nodes(0), allocator(all), function(function)
	{  }


	template <class Key, class Compare,class Allocator>
	BinomHeap<Key, Compare,Allocator>::~BinomHeap()
	{
		clear();
	}

	template <class Key, class Compare,class Allocator>
	Key BinomHeap<Key, Compare,Allocator>::top() const
	{
		return minimum->key;
	}

	template <class Key, class Compare,class Allocator>
	typename BinomHeap<Key, Compare,Allocator>::Node* BinomHeap<Key, Compare,Allocator>::push(const Key &key)
	{
		nodes++;
		Node *A = newNode(key);

		if(root == 0)
			return root = minimum = A;

		root = join(root, A);
		if(function(A->key, minimum->key))
			minimum = A;
		return A;
	}

	template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::pop()
	{
		nodes--;

		if(root == minimum)
			root = root->next;
		else
		{
			Node *A = root;
			while(A->next != minimum)
				A = A->next;
			A->next = minimum->next;
		}

		if(nodes == 0)
		{
			delNode(minimum), minimum = 0;
			return;
		}

		Node *child = minimum->child;
		if(child)
		{
			for(Node *A = child; A; A = A->next)
				A->parent = 0;
			root = root ? join(root, reverse(child)) : reverse(child);
		}

		delNode(minimum), minimum = root;
		if(minimum)
			for(Node *A = root->next; A; A = A->next)
				if(function(A->key, minimum->key))
					minimum = A;
	}

	template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::decrease(Node *A, const Key &key)
	{
		assert(!function(A->key,key));
		if (!function(A->key,key) && !function(key,A->key)) return;

		A->key = key;
		if(function(key, minimum->key))
			minimum = A;

		if(!A->parent || function(A->parent->key, A->key))
			return;

		Node *start = 0, *previous = 0, *B = A, *C = A->parent, *D;
		while(C)
		{
			D = C->child, C->child = B->next;
			if(B == D)
				D = C, C->degree--;
			else
			{
				Node *E = D;
				while(B != E->next)
					E = E->next, C->degree--;
				E->next = C, C->degree -= 2;
			}
			B->next = start, B = C, C = C->parent, start = previous, previous = D;
		}

		if(B == root)
			root = root->next;
		else
		{
			C = root;
			while(B != C->next)
				C = C->next;
			C->next = B->next;
		}
		B->next = start, start = previous;

		if(start)
		{
			for(B = start; B; B = B->next)
				B->parent = 0;
			root = root ? join(root, reverse(start)) : reverse(start);
		}
		A->parent = 0, A->next = 0, root = root ? join(root, A) : A;
	}

	template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::del(Node *A)
	{
		nodes--;

		if(nodes == 0)
		{
			delNode(A), root = minimum = 0;
			return;
		}

		Node *start = A->child, *previous = A->child, *next, *B = A, *C = A->parent;
		while(C)
		{
			next = C->child, C->child = B->next;
			if(B == next)
				next = C, C->degree--;
			else
			{
				Node *D = next;
				while(B != D->next)
					D = D->next, C->degree--;
				D->next = C, C->degree -= 2;
			}
			B->next = start, B = C, C = C->parent, start = previous, previous = next;
		}

		if(B == root)
			root = root->next;
		else
		{
			C = root;
			while(B != C->next)
				C = C->next;
			C->next = B->next;
		}
		B->next = start, start = previous;

		if(start)
		{
			for(B = start; B; B = B->next)
				B->parent = 0;
			root = root ? join(root, reverse(start)) : reverse(start);
		}

		if(minimum == A)
		{
			minimum = root;
			for(B = root->next; B; B = B->next)
				if(function(B->key, minimum->key))
					minimum = B;
		}
		delNode(A);
	}

	template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::merge(BinomHeap& heap)
	{
		if(!heap.root || root == heap.root)
			return;
		else if(root)
		{
			root = join(root, heap.root);
			if(function(heap.minimum->key, minimum->key))
				minimum = heap.minimum;
			nodes += heap.nodes;
		}
		else
			root = heap.root, minimum = heap.minimum, nodes = heap.nodes;
		heap.root = heap.minimum = 0, heap.nodes = 0;
	}

	template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::clear(Node* n)
	{
		if(n->next) clear(n->next);
		if(n->child) clear(n->child);
		allocator->deallocate(n);
	}

    template <class Key, class Compare,class Allocator>
	void BinomHeap<Key, Compare,Allocator>::clear()
	{
		if(root) clear(root);
	}

	template <class Key, class Compare,class Allocator>
	unsigned BinomHeap<Key, Compare,Allocator>::size() const
	{
		return nodes;
	}

	template <class Key, class Compare,class Allocator>
	bool BinomHeap<Key, Compare,Allocator>::empty() const
	{
		return root == 0;
	}

	template <class Key, class Compare,class Allocator>
	bool BinomHeap<Key, Compare,Allocator>::check() const
	{
		Node *A = root;
		while(A)
		{
			if(A->parent || !A->check())
				return false;
			A = A->next;
		}
		return true;
	}

	template <class Key, class Compare,class Allocator>
	inline typename BinomHeap<Key, Compare,Allocator>::Node* BinomHeap<Key, Compare,Allocator>::join(Node *A, Node *B)
	{
		Node *start, *C;
		if(A->degree <= B->degree)
			start = C = A, A = A->next;
		else
			start = C = B, B = B->next;
		while(A && B)
		{
			if(A->degree <= B->degree)
				C->next = A, A = A->next;
			else
				C->next = B, B = B->next;
			C = C->next;
		}
		C->next = A ? A : B;

		for(A = 0, B = start, C = B->next; C; C = B->next)
			if(B->degree != C->degree || (C->next && C->degree == C->next->degree))
				A = B, B = C;
			else if(function(B->key, C->key))
				B->next = C->next, B->insert(C);
			else
			{
				if(A)
					A->next = C;
				else
					start = C;
				C->insert(B), B = C;
			}
		return start;
	}

	template <class Key, class Compare,class Allocator>
	inline typename BinomHeap<Key, Compare,Allocator>::Node* BinomHeap<Key, Compare,Allocator>::reverse(Node *A)
	{
		Node *B = A->next, *C;
		A->next = 0;
		while(B)
			C = B->next, B->next = A, A = B, B = C;
		return A;
	}

	template <class Key, class Compare,class Allocator>
	inline typename BinomHeap<Key, Compare,Allocator>::Node* BinomHeap<Key, Compare,Allocator>::cut(Node *A)
	{
		Node *B = A->next, *C;
		A->next = 0;
		while(B)
			C = B->next, B->next = A, A = B, B = C;
		return A;
	}


namespace Privates {

        template <class Key>
        struct FibonHeapNode
		{
			FibonHeapNode<Key> *parent, *child, *previous, *next;
			unsigned flag;
			Key key;

			Key get() { return key; }

			FibonHeapNode(const Key& = Key());

			inline void insert(FibonHeapNode<Key>*);
			inline void remove();
			bool check() const;
			void init(const Key& =Key());
		};

	template <class Key>
	FibonHeapNode<Key>::FibonHeapNode(const Key &_key)
	{
		init(_key);
	}

	template <class Key>
	inline void FibonHeapNode<Key>::init(const Key &_key)
	{   parent=child=0;flag=0; key=_key;
		previous = next = this;
	}

	template <class Key>
	inline void FibonHeapNode<Key>::insert(FibonHeapNode<Key> *A)
	{
		next->previous = A->previous, A->previous->next = next, next = A, A->previous = this;
	}

	template <class Key>
	inline void FibonHeapNode<Key>::remove()
	{
		previous->next = next, next->previous = previous, previous = next = this;
	}

	template <class Key>
	bool FibonHeapNode<Key>::check() const
	{
		FibonHeapNode<Key> *child = this->child;
		unsigned degree = 0;

		if(!child)
			return flag < 2;

		do
		{
			if(child->previous->next != child || child->next->previous != child || child->parent != this || !child->check())
				return false;
			child = child->next, degree++;
		}
		while(child != this->child);
		return degree == (flag >> 1);
	}

}


	template <class Key, class Compare = std::less<Key>,class Allocator=Privates::DefaultCPPAllocator >
	class FibonHeap
	{
	public:

        typedef Privates::FibonHeapNode<Key> Node;
        typedef Privates::FibonHeapNode<Key> * Repr;

	private:
		Node *root,*degrees[sizeof(unsigned) << 3];
		unsigned nodes;
		Compare function;
		Allocator* allocator;

		Node* newNode(Key key)
		{
		    if (!allocator) return new Node(key);
		    Node* res=allocator->template allocate<Privates::FibonHeapNode<Key> >();
            res->init(key);
		    return res;
		}
		void delNode(Node* node)
		{
		    if (!allocator) delete node;
		    else allocator->deallocate(node);
		}

        void clear(Node*);

	public:
		inline FibonHeap(const Compare& = Compare());
		inline FibonHeap(Allocator*,const Compare& = Compare());
		~FibonHeap();

		Key top() const;
		Node* push(const Key&);
		void pop();

		void decrease(Node*, const Key&);
		void del(Node*);

		void merge(FibonHeap&);
		void clear();

		unsigned size() const;
		bool empty() const;

		bool check() const;
	};

	template <class Key, class Compare,class Allocator>
	inline FibonHeap<Key, Compare,Allocator>::FibonHeap(const Compare &function) : root(0), nodes(0), allocator(0),function(function)
	{
		for(int i=0;i<(sizeof(unsigned) << 3);i++) degrees[i]=0;
	}

	template <class Key, class Compare,class Allocator>
	inline FibonHeap<Key, Compare,Allocator>::FibonHeap(Allocator* all, const Compare &function) : root(0), nodes(0), allocator(all),function(function)
	{
		for(int i=0;i<(sizeof(unsigned) << 3);i++) degrees[i]=0;
	}


	template <class Key, class Compare,class Allocator>
	FibonHeap<Key, Compare,Allocator>::~FibonHeap()
	{
		clear();
	}

	template <class Key, class Compare,class Allocator>
	Key FibonHeap<Key, Compare,Allocator>::top() const
	{
		return root->key;
	}

	template <class Key, class Compare,class Allocator>
	typename FibonHeap<Key, Compare,Allocator>::Node* FibonHeap<Key, Compare,Allocator>::push(const Key &key)
	{
		nodes++;

		Node *A = newNode(key);
		if(!root)
			return root = A;

		root->insert(A);
		if(function(A->key, root->key))
			root = A;
		return A;
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::pop()
	{
		nodes--;

		Node *A = root->child, *B;
		if(A)
		{
			B = A;
			do
			{
				B->parent = 0, B = B->next;
			}
			while(A != B);
			root->insert(A);
		}

		if(!nodes)
		{
            delNode(root); root = 0;
			return;
		}

		Node **degrees = this->degrees, *C;
		unsigned degree_max = 0, degree=0;
		for(A = root->next, B = A->next; A != root; degrees[degree] = A, A = B, B = A->next)
		{
			while(degrees[degree = A->flag >> 1])
			{
				C = degrees[degree];
				if(function(C->key, A->key))
					C = A, A = degrees[degree];
				degrees[degree] = 0, C->remove(), C->parent = A, C->flag &= ~1;
				if(A->child)
					A->flag += 2, A->child->insert(C);
				else
					A->flag = 2, A->child = C;
			}

			if(degree > degree_max)
				degree_max = degree;
		}
		root->remove(), delNode(root);

		for(degree = 0; degree <= degree_max; degree++)
			if(degrees[degree])
			{
				root = degrees[degree], degrees[degree] = 0, degree++;
				break;
			}
		for(; degree <= degree_max; degree++)
			if(degrees[degree])
			{
				if(function(degrees[degree]->key, root->key))
					root = degrees[degree];
				degrees[degree] = 0;
			}
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::decrease(Node *A, const Key &key)
	{
		assert(!function(A->key,key));
		if (!function(A->key,key) && !function(key,A->key)) return;


		A->key = key;
		Node *B = A->parent;
		if(!B)
		{
			if(function(key, root->key))
				root = A;
			return;
		}
		else if(!function(key, B->key))
			return;

		while(1)
		{
			if(A == A->next)
				B->child = 0;
			else
			{
				if(A == B->child)
					B->child = A->next;
				A->remove(), A->flag &= ~1;
			}
			B->flag -= 2, root->insert(A), A->parent = 0;
			if(function(A->key, root->key))
				root = A;

			if(!B->parent)
				return;
			if(!(B->flag & 1))
			{
				B->flag |= 1;
				return;
			}
			A = B, B = B->parent;
		}
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::del(Node *A)
	{
		Node *B = A->parent, *C = A;
		if(!B)
		{
			root = A, pop();
			return;
		}

		while(1)
		{
			if(A == A->next)
				B->child = 0;
			else
			{
				if(A == B->child)
					B->child = A->next;
				A->remove(), A->flag &= ~1;
			}
			B->flag -= 2, root->insert(A), A->parent = 0;

			if(!B->parent)
				break;
			if(!(B->flag & 1))
			{
				B->flag |= 1;
				break;
			}
			A = B, B = B->parent;
		}

		root = C, pop();
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::merge(FibonHeap& heap)
	{
		if(!heap.root || root == heap.root)
			return;
		else if(root)
		{
			root->insert(heap.root);
			if(function(heap.root->key, root->key))
				root = heap.root;
			nodes += heap.nodes;
		}
		else
			root = heap.root, nodes = heap.nodes;
		heap.root = 0, heap.nodes = 0;
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::clear()
	{
		if(root)
			clear(root);
	}

	template <class Key, class Compare,class Allocator>
	void FibonHeap<Key, Compare,Allocator>::clear(Node* n)
	{
		if(n->child)
			clear(n->child);
		if(n->previous != n)
			n->next->previous = n->previous, clear(n->next);
		delNode(n);
	}



	template <class Key, class Compare,class Allocator>
	unsigned FibonHeap<Key, Compare,Allocator>::size() const
	{
		return nodes;
	}

	template <class Key, class Compare,class Allocator>
	bool FibonHeap<Key, Compare,Allocator>::empty() const
	{
		return !root;
	}

	template <class Key, class Compare,class Allocator>
	bool FibonHeap<Key, Compare,Allocator>::check() const
	{
		if(!root)
			return true;

		Node *A = root;
		do
		{
			if(A->next->previous != A || A->parent || !A->check())
				return false;
			A = A->next;
		}
		while(A != root);

		return true;
	}

}

#endif
