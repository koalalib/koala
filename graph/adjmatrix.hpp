template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
void Graph<VertInfo, EdgeInfo,EdAllow>::AdjMatrix::add(typename Graph<VertInfo,EdgeInfo,EdAllow>::PEdge edge)
{   if (!edge) return;
    if (edge->type==Directed)
    {   std::pair<typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex,typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	Parals& pole=dirs(ends.first,ends.second);
	edge->pParal() = pole.last;
	edge->nParal() = NULL;
	if(edge->pParal())
	    edge->pParal()->nParal() = edge;
	else
	    pole.first = edge;
	pole.last = edge;
	pole.degree++;
    }
    else if (edge->type==Undirected)
    {   std::pair<typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex,typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	Parals& pole=undirs(ends.first,ends.second);
	edge->pParal() = pole.last;
	edge->nParal() = NULL;
	if(edge->pParal())
	    edge->pParal()->nParal() = edge;
	else
	    pole.first = edge;
	pole.last = edge;
	pole.degree++;
    }
}

