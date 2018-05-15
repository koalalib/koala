#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#define DEBUG
//#define NODEUPDATE
//#define LOCALALLOC
#define HASHSETDEBUG
#include "Koala/container/hashcont.h"

#include<map>
using namespace std;
using namespace Koala;


#define TestStruct	HashMap<int, int>

// Verify
bool Verify(TestStruct *) {
	return true;
	};

//#ifdef _MSC_VER
#define random(v)	(rand() % v)
#define randomize()	(srand(time(NULL)))
//#endif


// RandKey
int RandKey(char *txt) {
	int v;
	v = random(10000) * 10000 + random(10000);
//	v = random(1000000);
	sprintf(txt, "%09d", v);
	return v;
	};


// Test
int Test() {
	map<int, int> keys;
	map<int, int>::iterator it;
	char key[128];
	int i, s, nr, k;
	TestStruct tree(8192);

	HashMap<std::pair<int*, int *>, int> m;
	m[make_pair((int *)3, (int *)NULL)] = 4;

	randomize();

	tree.insert(6, 1);
	tree.insert(4, 1);
	tree.insert(3, 1);

	for(i = 0; i < 100; i++) {
		if(i == 3) continue;
		if(i == 4) continue;
		if(i == 6) continue;
		tree.insert(i, 1);
		TestStruct::iterator tt = tree.find(i);
		if(tt == tree.end()) {
			printf("error aaa\n");
			return 0;
			};
		tree.erase(tt);
		if(tree.size() != 3) { printf("error size\n"); return 0; };
		if(tree.find(3) == tree.end()) { printf("error 3\n"); return 0; };
		if(tree.find(4) == tree.end()) { printf("error 4\n"); return 0; };
		if(tree.find(6) == tree.end()) { printf("error 6\n"); return 0; };
		};

#define A(v) \
	tree.Insert(key, 1);
//	sprintf(key, "%06d", v);
//	node = CreateNode(RBTC_RED, key, 1);
//	root = Insert(root, node);
//	Print(root); printf("\n");

#define R(v) \
	tree.Delete(key);
//	sprintf(key, "%06d", v);
//	node = Find(root, key);
//	root = Remove(root, node);
//	Print(root); printf("\n");

	printf("---------------------\n");

	keys[6] = 6;
	keys[4] = 4;
	keys[3] = 3;

	nr = 0;
	for(i = 0; i < 4; i++) {
		while(true) {
			k = RandKey(key);
			if(keys.find(k) == keys.end()) break;
			};
		keys[k] = k;
		tree.insert(k, 1);
                if(!Verify(&tree)) { /*Print(root);*/ return 0; };
		};
        while(true) {
		int size;
		size = tree.size();
		if((nr % 1000) == 0) printf("%9d: %6d %9d\r", nr, size, tree.collisions);
		nr++;
		if(random(100) < 50 && keys.size() > 0) {
			s = keys.size();
			i = random(s);
			for(k = 0, it = keys.begin(); k < i; k++) it++;
			i = it->first;
			if(tree.find(i) == tree.end()) {
				printf("error (find %d)\n", i);
				return 0;
			} else {
				keys.erase(it);
				tree.erase(i);
				};
                } else {
			while(true) {
				k = RandKey(key);
				if(keys.find(k) == keys.end()) break;
				};
			keys[k] = k;
			tree.insert(k, 1);
			if(random(100) == 0) {
				TestStruct t2(tree);
				TestStruct::iterator it;
//				t2 = tree;
				for(it = tree.begin(); it != tree.end(); ++it) {
					if(t2.find(it->first) == t2.end()) {
						printf("\nfailed copy\n");
						break;
						};
					};
				};
			};
		if(!Verify(&tree)) {
			printf("!verify\n");
			return 0;
			};
		};
	};


#include "main.hpp"
	Test();
	return 0;
	};
