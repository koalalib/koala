
template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
Edge<VertInfo,EdgeInfo,EdAllow>::Edge():
		next(NULL), prev(NULL), type(Detached) {}

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
Edge<VertInfo,EdgeInfo,EdAllow>::Edge(const EdgeInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL), type(Detached) {}
