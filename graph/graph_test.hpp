#define TO_STRING(x) #x
//#define ANSWER(id,line) return false;
//#define ANSWER(id,line) throw Koala_Exception(id,line);
#define ANSWER(id,line) {printf("blad: %x (" TO_STRING(line) ")\n",id); fflush(stdout);}

template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::testGraph() {

	//checking global vertices list
	PVertex vert = first_vert;
	PVertex prev_vert = NULL;
	int len_vert = no_vert;
	while(vert&&len_vert) {
		if(vert->prev!=prev_vert)
			ANSWER(0x91, __LINE__)
		prev_vert = vert;
		vert = vert->next;
		--len_vert;
	}
	if(prev_vert!=last_vert)
		ANSWER(0x9D, __LINE__)
	if(len_vert)
		ANSWER(0x9E, __LINE__)
	if(vert)
		ANSWER(0x9F, __LINE__)

	//puting pointers to the vector
	std::vector<PVertex> vect_vert(no_vert);

	vert = first_vert;
	int i=0;
	while(vert) {
		vect_vert[i] = vert;
		vert = vert->next;
		++i;
	}
	std::sort(vect_vert.begin(), vect_vert.end());

	//checking global edges list
	PEdge edge = first_edge;
	PEdge prev_edge = NULL;
	int len_edge = no_loop_edge + no_undir_edge + no_dir_edge;
	int len_loop_edge = no_loop_edge;
	int len_undir_edge = no_undir_edge;
	int len_dir_edge = no_dir_edge;
	while(edge&&len_edge) {
		if(edge->prev!=prev_edge)
			ANSWER(0xA1, __LINE__)
		prev_edge = edge;
		switch(edge->type) {
		case Directed:
			--len_dir_edge;
			break;
		case Undirected:
			--len_undir_edge;
			break;
		case Loop:
			--len_loop_edge;
			break;
		default:
			ANSWER(0xAA, __LINE__)
		}
		edge = edge->next;
		--len_edge;
	}
	if(prev_edge!=last_edge)
		ANSWER(0xAD, __LINE__)
	if(len_dir_edge)
		ANSWER(0xA2, __LINE__)
	if(len_undir_edge)
		ANSWER(0xA3, __LINE__)
	if(len_loop_edge)
		ANSWER(0xA4, __LINE__)
	if(len_edge)
		ANSWER(0xAE, __LINE__)
	if(edge)
		ANSWER(0xAF, __LINE__)

	//puting pointers to the vector
	std::vector<PEdge> vect_edge(no_loop_edge + no_undir_edge + no_dir_edge);
	edge = first_edge;
	i=0;
	while(edge) {
		vect_edge[i] = edge;
		edge = edge->next;
		++i;
	}
	std::sort(vect_edge.begin(), vect_edge.end());

	//checking if all edges points to available vertices
	edge = first_edge;
	while(edge) {
		PVertex s_elem = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert; //searched element
		if(!s_elem||!std::binary_search(vect_vert.begin(),vect_vert.end(),s_elem))
			ANSWER(0x95, __LINE__)
		if(edge->type!=Loop) {
			s_elem = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert;
			if(!s_elem||!std::binary_search(vect_vert.begin(),vect_vert.end(),s_elem))
				ANSWER(0x96, __LINE__)
		} else {
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert!=edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert)
				ANSWER(0x97, __LINE__)
		}
		edge = edge->next;
	}

	//checking if all vertices points to available edges
	// and if checking edges sublists are well defined
	vert = first_vert;
	while(vert) {
		//begin: checking loops
		edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].first;
		len_edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].degree;
		prev_edge = NULL;
		while(edge&&len_edge) {
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert!=vert)
				ANSWER(0xB1, __LINE__)
			if(edge->type!=Loop)
				ANSWER(0xB2, __LINE__)
			if(!std::binary_search(vect_edge.begin(), vect_edge.end(), edge))
				ANSWER(0xB3, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev!=prev_edge)
				ANSWER(0xB4, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert!=edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].vert) //for loop edge second vertex is equal to first
				ANSWER(0xB5, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].next) //as well as next and prev for second vertex
				ANSWER(0xB6, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].prev)
				ANSWER(0xB7, __LINE__)
			prev_edge = edge;
			edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next;
			--len_edge;
		}
		if(prev_edge!=vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last)
			ANSWER(0xBD, __LINE__)
		if(len_edge)
			ANSWER(0xBE, __LINE__)
		if(edge)
			ANSWER(0xBF, __LINE__)
		//end: checking loops

		//begin: checking directed in
		edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].first;
		len_edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].degree;
		prev_edge = NULL;
		while(edge&&len_edge) {
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert!=vert)
				ANSWER(0xC1, __LINE__)
			if(edge->type!=Directed)
				ANSWER(0xC2, __LINE__)
			if(!std::binary_search(vect_edge.begin(), vect_edge.end(), edge))
				ANSWER(0xC3, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert==edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert)
				ANSWER(0xC4, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev!=prev_edge)
				ANSWER(0xC5, __LINE__)
			prev_edge = edge;
			edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next;
			--len_edge;
		}
		if(prev_edge!=vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last)
			ANSWER(0xCD, __LINE__)
		if(len_edge)
			ANSWER(0xCE, __LINE__)
		if(edge)
			ANSWER(0xCF, __LINE__)
		//end: checking directed in

		//begin: checking directed out
		edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].first;
		len_edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].degree;
		prev_edge = NULL;
		while(edge&&len_edge) {
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert!=vert)
				ANSWER(0xD1, __LINE__)
			if(edge->type!=Directed)
				ANSWER(0xD2, __LINE__)
			if(!std::binary_search(vect_edge.begin(), vect_edge.end(), edge))
				ANSWER(0xD3, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert==edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert)
				ANSWER(0xD4, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev!=prev_edge)
				ANSWER(0xD5, __LINE__)
			prev_edge = edge;
			edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next;
			--len_edge;
		}
		if(prev_edge!=vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last)
			ANSWER(0xDD, __LINE__)
		if(len_edge)
			ANSWER(0xDE, __LINE__)
		if(edge)
			ANSWER(0xDF, __LINE__)
		//end: checking directed out

		//begin: checking undirected
		edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].first;
		len_edge = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;
		prev_edge = NULL;
		int is_UV = 0;
		while(edge&&len_edge) {
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vert) is_UV = Graph<VertInfo,EdgeInfo>::Edge::V_U;
			else if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert==vert) is_UV = Graph<VertInfo,EdgeInfo>::Edge::V_V;
			else ANSWER(0xE0, __LINE__)
			if(edge->type!=Undirected)
				ANSWER(0xE1, __LINE__)
			if(!std::binary_search(vect_edge.begin(), vect_edge.end(), edge))
				ANSWER(0xE2, __LINE__)
			if(edge->vert[is_UV].prev!=prev_edge)
				ANSWER(0xE3, __LINE__)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert)
				ANSWER(0xE4, __LINE__)
			prev_edge = edge;
			edge = edge->vert[is_UV].next;
			--len_edge;
		}
		if(prev_edge!=vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last)
			ANSWER(0xED, __LINE__)
		if(len_edge)
			ANSWER(0xEE, __LINE__)
		if(edge)
			ANSWER(0xEF, __LINE__)
		//end: checking undirected
		vert = vert->next;
	}
	return true;
}

