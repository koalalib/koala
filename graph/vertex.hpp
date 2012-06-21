
template<class VertInfo, class EdgeInfo, class Settings>
Vertex<VertInfo,EdgeInfo,Settings>::Vertex():
		next(NULL), prev(NULL) {}

template<class VertInfo, class EdgeInfo, class Settings>
Vertex<VertInfo,EdgeInfo,Settings>::Vertex(const VertInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL)  {}
