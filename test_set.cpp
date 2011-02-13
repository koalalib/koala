#include <stdio.h>
#include "Koala/container/set.h"
#include <set>
#include <time.h>
#include <stdlib.h>
using namespace Koala;

void pri(Set<int> &a) {
	std::cout << a << std::endl;

}

int main() {
	printf("test set\n");
	Set<int> a, b;

	const int tst = 6;

	for(int i=0; i<tst; i++) {
		a.add( rand()%10 );
	}
	for(int i=0; i<tst; i++) {
		b.add( rand()%10 );
	}
	pri(a);
	pri(b);
	Set<int> x = a+b;
	pri(x);
	x = a*b;
	pri(x);
	x = a-b;
	pri(x);
/*	pri(a+b);
	pri(a*b);
	pri(a-b);
//*/
	return 0;
}
