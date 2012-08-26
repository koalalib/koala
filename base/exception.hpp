// ExcBase

namespace Koala
{
    namespace Error
    {
        inline ExcBase::ExcBase( const char *adesc, const char *afile, int aline ): _line(aline)
        {
            assert( std::strlen( adesc ) + std::strlen( afile ) + 2 <= KOALA_EXCEPTION_BUF_SIZE );
            std::strcpy( buf,adesc );
            std::strcpy( buf + std::strlen( adesc ) + 1,afile );
        }
    }
}
