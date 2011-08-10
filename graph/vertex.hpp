
template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
Vertex<VertInfo,EdgeInfo,EdAllow>::Vertex():
		next(NULL), prev(NULL) {}

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
Vertex<VertInfo,EdgeInfo,EdAllow>::Vertex(const VertInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL)  {}
