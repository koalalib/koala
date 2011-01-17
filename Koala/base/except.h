#ifndef KOALA_EXCEPTION
#define KOALA_EXCEPTION


//TODO: wlasciwie wszystko
namespace Koala {

struct Koala_Exception {
	int id,line;
	Koala_Exception(int id, int line=0): id(id), line(line) {};
};

}

#endif
