#ifndef KOALA_VERTEX_H
#define KOALA_VERTEX_H

namespace Koala
{
	namespace Privates
	{
		/* NormalVertLink
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings > struct NormalVertLink
		{
			Edge< VertInfo,EdgeInfo,Settings > *first,*last;
			int degree;

			NormalVertLink(): first( NULL ), last( NULL ), degree( 0 )
				{ }

			Edge< VertInfo,EdgeInfo,Settings > *&getFirst()
				{ return first; }
			Edge< VertInfo,EdgeInfo,Settings > *&getLast()
				{ return last; }

			int &getDegree()
				{ return degree; }
		};

		/* EmptyVertLink
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings > struct EmptyVertLink
		{
			EmptyVertLink()
				{ }

			DummyVar< Edge< VertInfo,EdgeInfo,Settings > * > getFirst()
				{ return DummyVar< Edge< VertInfo,EdgeInfo,Settings > * >(); }
			DummyVar< Edge< VertInfo,EdgeInfo,Settings > * > getLast()
				{ return DummyVar< Edge< VertInfo,EdgeInfo,Settings > * >(); }
			DummyVar< int > getDegree() { return DummyVar< int >(); }
		};


		/* VertLinkEdDirIn
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings, EdgeType Present > struct VertLinkEdDirIn;
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdDirIn< VertInfo,EdgeInfo,Settings,EdDirIn >:
			public NormalVertLink< VertInfo,EdgeInfo,Settings > { };
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdDirIn< VertInfo,EdgeInfo,Settings,0 >:
			public EmptyVertLink< VertInfo,EdgeInfo,Settings > { };

		/* VertLinkEdDirOut
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings, EdgeType Present > struct VertLinkEdDirOut;
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdDirOut< VertInfo,EdgeInfo,Settings,EdDirOut >:
			public NormalVertLink< VertInfo,EdgeInfo,Settings > { };
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdDirOut< VertInfo,EdgeInfo,Settings,0 >:
			public EmptyVertLink< VertInfo,EdgeInfo,Settings > { };

		/* VertLinkEdUndir
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings, EdgeType Present > struct VertLinkEdUndir;
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdUndir< VertInfo,EdgeInfo,Settings,EdUndir >:
			public NormalVertLink< VertInfo,EdgeInfo,Settings > { };
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdUndir< VertInfo,EdgeInfo,Settings,0 >:
			public EmptyVertLink< VertInfo,EdgeInfo,Settings > { };

		/* VertLinkEdLoop
		 *
		 */
		template< class VertInfo, class EdgeInfo, class Settings, EdgeType Present > struct VertLinkEdLoop;
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdLoop< VertInfo,EdgeInfo,Settings,EdLoop >:
			public NormalVertLink< VertInfo,EdgeInfo,Settings > { };
		template< class VertInfo, class EdgeInfo, class Settings >
			struct VertLinkEdLoop< VertInfo,EdgeInfo,Settings,0 >:
			public EmptyVertLink< VertInfo,EdgeInfo,Settings > { };
	}

	/** \brief Vertex of graph
	 *
	 *  The class used as a basic structure of graph representing a vertex (node). Objects of this uncopyable class can be created only from the friend classes.
	 *  \tparam VertInfo the type of objects that store any information connected with vertex.
	 *  \tparam EdgeInfo the type of objects that store any information connected with edge.
	 *  \tparam Settings the type of objects which store parameters of graph.
	 *  \ingroup DMgraph*/
	template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyEdgeInfo,
		class Settings = GrDefaultSettings< EdAll,true > > class Vertex:
		private Privates::VertLinkEdDirIn< VertInfo,EdgeInfo,Settings,EdDirIn & Settings::EdAllow >,
		private Privates::VertLinkEdDirOut< VertInfo,EdgeInfo,Settings,EdDirOut & Settings::EdAllow >,
		private Privates::VertLinkEdUndir< VertInfo,EdgeInfo,Settings,EdUndir & Settings::EdAllow >,
		private Privates::VertLinkEdLoop< VertInfo,EdgeInfo,Settings,EdLoop & Settings::EdAllow >,
		public Settings::template VertAdditData< VertInfo,EdgeInfo,Settings >
	{
		friend class Graph< VertInfo,EdgeInfo,Settings >;
		friend class Edge< VertInfo,EdgeInfo,Settings >;

	public:
		/** \brief Additional user information kept in vertex.
		 *
		 *  This member object should be used any time additional information associated with vertex is necessary.
		 *  \a info may be used for algorithmic purposes but also to keep any data relevant from the point of view of an application.
		 */
		VertInfo info;

		/** \brief Get vertex information object.
		 *
		 *  \returns the information object associated with the vertex.
		 */
		VertInfo getInfo()
			{ return info; }

		/** \brief Set vertex information object.
		 *
		 *  The method sets \a info as the new value for data member info.
		 *  \param info the value to be stored as a new vertex information.
		 */
		void setInfo( const VertInfo &info )
			{ this->info = info; }

	private:
		// klasa jest niekopiowalna, obiekty mozna tworzyc i usuwac jedynie z metod klas zaprzyjaznionych
		/** Standard constructor*/
		Vertex(): info (), next( NULL ), prev( NULL )
			{ }
		/** Constructor sets info variable */
		Vertex( const VertInfo &infoExt ): info( infoExt ), next( NULL ), prev( NULL )
			{ }
		Vertex( const Vertex & X)
			{ }
		Vertex &operator=( const Vertex &X )
			{ }
		~Vertex()
			{ }

		Vertex *next,*prev;
	};
}
#endif
