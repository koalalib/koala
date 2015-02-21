#ifndef KOALA_RAND_H
#define KOALA_RAND_H

#include <cstdlib>
/* */

namespace std {
template <class IntType> class uniform_int_distribution;
}

namespace Koala
{

//Int powinien byc typem calkowitoliczbowym nie wezszym niz int
// generatora StdRandGen<int> mozna uzywac zamiast STLowych z <random>, jesli nie chcemy korzystac z dobrodziejstw standardu 2011
// (domyslnie Koala stosuje sie do C++ 2003). W kodach Koalowych korzystajacych z liczb losowych nie odowlujemy sie
// bezposrednio do metod/klas z <random>, co najwyzej tworzymy obiekty generatorow i przekazujemy je jako parametr do metod
/** \brief Random numbers generator
  *
  *  Methods that involve randomness take random numbers generator (like this) as a parameter. 
  *  C++ standard 2011 gives such generator in header <random>. However KOALA is consistent with standard 2003, for this reason we implement the following class.
  *  \tparam Int integer type returned by generator. The type shouldn't be greater then int.*/
template <class Int = int>
class StdRandGen {

        int digNo,digNo2;

        void initwykl()
        {   Int base=RAND_MAX,res=RAND_MAX;
            if (RAND_MAX==std::numeric_limits< Int >::max())
            {
                digNo=1; digNo2=0; return;
            }
            base++;
            for(;;res=res*base+(base-1))
            {
                 digNo++;
                 if (res>std::numeric_limits< Int >::max()/base ||
                    std::numeric_limits< Int >::max()-(base-1)<res*base) break;
            }
            for(;;res=2*res+1)
            {
                if (res>std::numeric_limits< Int >::max()/2
                    || 2*res==std::numeric_limits< Int >::max()) break;
                digNo2++;
            }
        }

    public:

        typedef Int ValType; /**< \brief Type of generated numbers. */
        const Int maxRand; /**< \brief Maximal vale of generated numbers.*/

		/** \brief Empty constructor.*/
        StdRandGen() : maxRand(0), digNo(0), digNo2(0)
        {
            this->initwykl();
            Int& maks=const_cast<Int&>(maxRand);
            for(int i=0;i<digNo;i++) maks=maks*((Int)RAND_MAX+1)+(Int)RAND_MAX;
            for(int i=0;i<digNo2;i++) maks=2*maks+1;
        }
		
        StdRandGen<Int>& operator=(const StdRandGen<Int>&) {}
		/** \brief Get random number.
		 *
		 *  \return random number of type Int.*/
        Int rand()
        {
            Int res=0;
            for(int i=0;i<digNo;i++) res=res*((Int)RAND_MAX+1)+std::rand();
            for(int i=0;i<digNo2;i++) res=2*res+(bool)(std::rand()<=(RAND_MAX/2));
            return res;
        }


		/** \brief Get random number.
		 *
		 *  \param maks maximal generated value.
		 *  \return random number of type Int.*/
        Int rand(Int maks)
        {
            if (maks<0) maks=-maks;
            return Int( (maks+1) * (long double)(this->rand())/((long double)(maxRand) + 1.0) );
        }

};

namespace Privates {


//NEW: ??? W srodku procedur Koali odwolujemy sie do generatorow tylko przy pomocy tych 3 funkcji:

//najwieksza liczba losowa, jaka moze sie pojawic. Zwykle cos kolo maxinta
template <class Gen>
int getMaxRandom(Gen& g)
{
    return std::numeric_limits< int >::max()-1;
}

// liczba losowa z przedzialu 0,1,...,getMaxRandom
template <class Gen>
int getRandom(Gen& g)
{
    std::uniform_int_distribution<int> distr(0,getMaxRandom( g));
    return distr(g);
}

// liczba losowa z przedzialu 0,1,...,maks
template <class Gen>
int getRandom(Gen& g,int maks)
{
    if (maks<0) maks=-maks;
    std::uniform_int_distribution<int> distr(0,maks);
    return distr(g);
}


template <>
inline int getMaxRandom(StdRandGen<int>& g)
{
    return g.maxRand;
}

template <>
inline int getRandom(StdRandGen<int>& g)
{
    return g.rand();
}

template <>
inline int getRandom(StdRandGen<int>& g,int maks)
{
    return g.rand(maks);
}

}


}

#endif
