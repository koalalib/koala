
template<class VertInfo, class EdgeInfo>
Edge<VertInfo,EdgeInfo>::Edge():
		next(NULL), prev(NULL), nParal(NULL), pParal(NULL), type(Detached) {}

template<class VertInfo, class EdgeInfo>
Edge<VertInfo,EdgeInfo>::Edge(const EdgeInfo &infoExt):
		info(infoExt), next(NULL), prev(NULL), nParal(NULL), pParal(NULL), type(Detached) {}
