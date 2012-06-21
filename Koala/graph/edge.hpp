
template<class VertInfo, class EdgeInfo, class Settings>
Edge<VertInfo,EdgeInfo,Settings>::Edge():
		next(NULL), prev(NULL), type(Detached) {}

template<class VertInfo, class EdgeInfo, class Settings>
Edge<VertInfo,EdgeInfo,Settings>::Edge(const EdgeInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL), type(Detached) {}
