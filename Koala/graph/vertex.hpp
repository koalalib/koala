
template<class VertInfo, class EdgeInfo>
Vertex<VertInfo,EdgeInfo>::Vertex():
		next(NULL), prev(NULL) {}

template<class VertInfo, class EdgeInfo>
Vertex<VertInfo,EdgeInfo>::Vertex(const VertInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL)  {}
