#ifndef KOALA_DEF_SCHEDULING_H
#define KOALA_DEF_SCHEDULING_H

/* schedule.h
 *
 */

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "../base/defs.h"
#include "../container/assoctab.h"
#include "../algorithm/search.h"

namespace Koala
{
    /* SchedulingPar
     *
     */
    template< class DefaultStructs > class SchedulingPar
    {
      public:
        template< class GraphType > struct Task
        {
            int length,release,duedate;
            typename GraphType::PVertex vertex;

            Task( typename GraphType::PVertex _vertex = 0): length( 1 ), release( 0 ),
                duedate(AlgsDefaultSettings::NumberTypeBounds< int >::plusInfty()), vertex(_vertex)
                { }

            Task( int _length, int _release, int _duedate, typename GraphType::PVertex _vertex = 0 ):
                length( _length ), release( _release ), duedate( _duedate ), vertex( _vertex )
                { }
        };

        struct TaskPart
        {
            int task,start,end,part;

            TaskPart( int _task = 0, int _start = 0, int _end = 0, int _part = 0):
                task( _task ), start( _start ), end( _end ), part( _part )
                { }
        };

        struct TaskWindow
        {
            int earliestStart,earliestFinish,latestStart,latestFinish;

            TaskWindow() { }

            template< class GraphType > TaskWindow( Task< GraphType > task, int start, int finish ):
                earliestStart( start ), earliestFinish( start + task.length ),
                latestStart( finish - task.length ), latestFinish( finish )
                { }
        };

        struct Schedule
        {
            typedef std::vector< TaskPart > Machine;
            typedef std::vector< Machine > Type;

            Type machines;

            Schedule( int m = 0 )
                { setMachNo( m ); }
            void setMachNo( int m )
                { machines.resize( m ); }
            int getMachNo()
                { return machines.size(); }
            void clearMachine( int m )
                { machines[m].clear(); }
            void clearMachines();
        };

        template< typename TaskIterator, typename Iterator >
            static int sortLPT( TaskIterator begin, TaskIterator end, Iterator out )
            { return sortByComp< compareLPT< std::pair< TaskIterator,int > > >( begin,end,out ); }

        template< typename TaskIterator, typename Iterator >
            static int sortSPT( TaskIterator begin, TaskIterator end, Iterator out )
            { return sortByComp< compareSPT< std::pair< TaskIterator,int > > >( begin,end,out ); }

        template< typename TaskIterator, typename Iterator >
            static int sortEDD( TaskIterator begin, TaskIterator end, Iterator out )
            { return sortByComp< compareEDD< std::pair< TaskIterator,int > > >( begin,end,out ); }

        template< typename TaskIterator >
            static int CMax( TaskIterator, TaskIterator, const Schedule & );
        template< typename TaskIterator >
            static int SigmaCi( TaskIterator, TaskIterator, const Schedule & );
        template< typename TaskIterator >
            static int SigmaTi( TaskIterator, TaskIterator, const Schedule & );
        template< typename TaskIterator >
            static int SigmaUi( TaskIterator, TaskIterator, const Schedule & );
        template< typename TaskIterator >
            static int LMax( TaskIterator, TaskIterator, const Schedule & );

        //Test poprawnoci danego harmonogramu
        template< typename GraphType, typename TaskIterator >
            static bool test( TaskIterator, TaskIterator, const GraphType &, const Schedule &, bool = true );

        //-|prec|Cmax - czyli szeregowanie cie¿k¹ krytyczn¹ z zadaniami w wierzcho³kach
        template< typename GraphType, typename TaskIterator, typename TaskWindowIterator >
            static int critPath( TaskIterator, TaskIterator, const GraphType &, TaskWindowIterator );

        //P|prec|- - szeregowanie listowe przy zadanej licie
        template< typename GraphType, typename TaskIterator >
            static int ls( TaskIterator, TaskIterator, const GraphType &, Schedule & );

        //P2|UET,prec|Cmax - algorytm CoffmanaGrahama
        template< typename GraphType, typename TaskIterator >
            static int coffmanGraham( TaskIterator, TaskIterator, const GraphType &, Schedule & );

        //1|pmtn,prec,ri|Lmax  zmodyfikowany algorytm Liu
        template< typename GraphType, typename TaskIterator >
            static int precLiu( TaskIterator, TaskIterator, const GraphType &, Schedule & );

        //P|UET,intree|Lmax  algorytm Bruckera
        template< typename GraphType, typename TaskIterator >
            static int brucker( TaskIterator, TaskIterator, const GraphType &, Schedule & );

        //P|UET,intree|Cmax  procedura Hu
        template< typename GraphType, typename TaskIterator >
            static int hu( TaskIterator, TaskIterator, const GraphType &, Schedule & );

        //P||SigmaCi
        template< typename TaskIterator > static int spt( TaskIterator, TaskIterator, Schedule & );

        //1||SigmaUi  algorytm Hodgsona
        template< typename TaskIterator > static int hodgson( TaskIterator, TaskIterator, Schedule & );

      protected:
        template< typename Comp, typename TaskIterator, typename Iterator >
            static int sortByComp( TaskIterator begin, TaskIterator end, Iterator out );

        template< typename Pair > struct compareLPT
        {
            bool operator()( Pair a, Pair b )
                { return a.first->length > b.first->length || (a.first->length == b.first->length && a.second < b.second); }
        };

        template< typename Pair > struct compareSPT
        {
            bool operator()( Pair a, Pair b )
                { return a.first->length < b.first->length || (a.first->length == b.first->length && a.second < b.second); }
        };

        template< typename Pair > struct compareEDD
        {
            bool operator()( Pair a, Pair b )
                { return a.first->duedate < b.first->duedate || (a.first->duedate == b.first->duedate && a.second < b.second); }
        };

        template< typename Pair > struct compareSecondFirst
        {
            bool operator()( Pair a, Pair b ) { return a.second < b.second; }
        };

        template< typename Pair > struct compareSecondLast
        {
            bool operator()( Pair a, Pair b ) { return a.second > b.second; }
        };

        template< typename Task > struct Triple
        {
            Task task;
            int start, finish;

            Triple( Task _task = Task()): task( _task )
                { }
        };

        template< typename Task > struct Element
        {
            Task task;
            int index,priority,duedate,degree,timeleft,parts;

            Element( Task _task = Task(), int _index = 0 ): task( _task ), index( _index ), priority( 0 ),
                duedate( 0 ), timeleft( _task.length ), parts( 0 )
                { }
        };

        struct HodgsonElement
        {
            int index,length,duedate;
            bool late;

            HodgsonElement( int _index = 0, int _length = 1, int _duedate = 0 ): index( _index ), length( _length ),
                duedate( _duedate ), late( 0 )
                { }
        };
    };

    class Scheduling: public SchedulingPar< AlgsDefaultSettings > { };

#include "schedule.hpp"
}

#endif
