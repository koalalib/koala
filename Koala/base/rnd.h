#ifndef KOALA_RAND_H
#define KOALA_RAND_H

#include <cstdlib>
/* */

namespace Koala
{

template <class Int = int>
class StdRandGen {

        int digNo,digNo2;

        void initwykl()
        {   long double base=RAND_MAX,maks=std::numeric_limits< Int >::max(),res=base,res1=1;
            base++;
            for(;res<maks;res=res*base+base-1)
            {
                 digNo++; res1=res;
            }
            res=res1;
            for(;res<maks;res=2*res+1) digNo2++;
        }

    public:

        const Int maxRand;

        StdRandGen() : maxRand(0), digNo(0), digNo2(0)
        {
            initwykl();
            Int& maks=const_cast<Int&>(maxRand);
            for(int i=0;i<digNo;i++) maks=maks*((Int)RAND_MAX+1)+(Int)RAND_MAX;
            for(int i=0;i<digNo2;i++) maks=2*maks+1;
        }

        Int rand()
        {
            Int res=0;
            for(int i=0;i<digNo;i++) res=res*((Int)RAND_MAX+1)+std::rand();
            for(int i=0;i<digNo2;i++) res=2*res+(std::rand() & 1);
            return res;
        }

        Int rand(Int maks)
        {
            if (maks<0) maks=-maks;
            return Int( (maks+1) * (long double)(rand())/((long double)(maxRand) + 1.0) );
        }

};

}

#endif
