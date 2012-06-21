template <class DefaultStructs>
template<typename GraphType, typename TaskIterator, typename TaskWindowIterator>
int SchedulingPar<DefaultStructs>::critPath(TaskIterator begin, TaskIterator end, const GraphType &DAG,
	TaskWindowIterator schedule)
{
    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        Triple<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0, time = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		tasks[iterator->vertex] = Triple<Task<GraphType> >(*iterator);

	// Wyznaczenie kolejnoœci wierzcho³ków
	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	// Wyznaczenie czasow najwczeœniejszego startu
	for(int i = 0; i < n; i++)
	{
		Triple<Task<GraphType> > &element = tasks[vertices[i]];
		element.start = element.task.release;
	}
	for(int i = 0, stop; i < n; i++)
	{
		typename GraphType::PVertex v = vertices[i];
		Triple<Task<GraphType> > &first = tasks[v];
		stop = first.start + first.task.length;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
		{
			Triple<Task<GraphType> > &second = tasks[DAG.getEnd(e, v)];
			if(second.start < stop)
				second.start = stop;
		}

		if(time < stop)
			time = stop;
	}

	// Wyznaczenie czasow najpóŸniejszego koñca
	for(int i = 0; i < n; i++)
		tasks[vertices[i]].finish = time;

	for(int i = n - 1, start; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		Triple<Task<GraphType> > &first = tasks[v];
		start = first.finish - first.task.length;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			Triple<Task<GraphType> > &second = tasks[DAG.getEnd(e, v)];
			if(second.finish > start)
				second.finish = start;
		}
	}

	for(TaskIterator iterator = begin; iterator != end; ++iterator)
	{
		Triple<Task<GraphType> > &element = tasks[iterator->vertex];
		*schedule = TaskWindow(element.task, element.start, element.finish), ++schedule;
	}

	return time;
}

template <class DefaultStructs>
template<typename GraphType, typename TaskIterator>
bool SchedulingPar<DefaultStructs>::test(TaskIterator begin, TaskIterator end, const GraphType &DAG, Schedule &schedule,bool nonPmtn)
{
	typedef std::pair<TaskPart, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,TaskPart >::Type tasks(DAG.getVertNo());

	int parts = 0, n = 0;
	// Dwa rózne zadania nie s¹ wykonywane jednoczeœnie na jednej maszynie
	for(typename std::vector<std::vector<TaskPart> >::iterator i = schedule.machines.begin();
		i != schedule.machines.end(); parts += i->size(), ++i)
		for(typename std::vector<TaskPart>::iterator j = i->begin(), k = j + 1; k != i->end(); ++j, ++k)
			if(j->end > k->start)
				return false;

	// To samo zadanie nie jest wykonywane jednoczeœnie na obu maszynach
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{   assert(iterator->length >= 0);  }

	if (nonPmtn && n!=parts) return false;

	TaskPart LOCALARRAY(result, n);
	Pair LOCALARRAY(available, parts);
	PriQueueInterface<Pair*, compareSecondLast<Pair> > pq(available, parts);

	for(typename std::vector<std::vector<TaskPart> >::iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename std::vector<TaskPart>::iterator j = i->begin(); j != i->end(); ++j)
			pq.push(Pair(*j, j->start));

	while(!pq.empty())
	{
		TaskPart first = pq.top().first;
		TaskPart &second = result[first.task];

		if(first.part != second.part)
			return false;
		if(!first.part)
			second.start = first.start;
		else if(first.end > second.start)
			return false;

		second.part++, second.end = first.end, second.task += (first.end - first.start);
		pq.pop();
	}

	n = 0;
	// Ka¿de zadanie jest wykonane w ca³oœci
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		if(iterator->release > result[n].start || iterator->length != result[n].task)
			return false;

	n = 0;
	// Zachowania jest relacja prec
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		tasks[iterator->vertex] = result[n];

	for(typename GraphType::PVertex v = DAG.getVert(); v; v = DAG.getVertNext(v))
		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
			if(tasks[v].end > tasks[DAG.getEnd(e, v)].start)
				return false;

	return true;
}

template <class DefaultStructs>
template<typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::ls(TaskIterator begin, TaskIterator end, const GraphType &DAG, Schedule &schedule)
{
	typedef std::pair<int, int> Pair;

	Pair LOCALARRAY(machines, schedule.getMachNo());
	PriQueueInterface<Pair*, compareSecondLast<Pair> > machine(machines, schedule.getMachNo());
	for(int i = 0; i < schedule.getMachNo(); i++)
		machine.push(std::make_pair(i, 0));

	int n = 0, time = 0;
	TaskIterator iterator = begin;
	while(iterator != end)
	{
		int index = machine.top().first, start = machine.top().second, stop = machine.top().second + iterator->length;
		bool allowed = true;

		for(int i = 0; i < schedule.getMachNo(); i++)
			if(!schedule.machines[i].empty() && schedule.machines[i].back().end > start)
			{
//TODO: nie zakladalbym, ze TaskIterator potrafia dodawac liczby (to nie musi byc vector ani tablica)
// Jesli musisz w ten sposob, to po prostu przepisz na poczatku zakres begin-end do Localarraya taskow i dalej dzialaj na niej
				TaskIterator element = begin + schedule.machines[i].back().task;
				if(DAG.getEdge(element->vertex, iterator->vertex, EdDirOut))
				{
					allowed = false;
					break;
				}
			}

		if(allowed)
			schedule.machines[index].push_back(TaskPart(n++, start, stop)), ++iterator;
		machine.pop(); machine.push(std::make_pair(index, stop));

		if(time < stop)
			time = stop;
	}

	return time;
}

template <class DefaultStructs>
template <typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::coffmanGraham(TaskIterator begin, TaskIterator end, const GraphType &DAG, Schedule &schedule)
{   assert(schedule.getMachNo()==2);
	typedef std::list<typename GraphType::PVertex> VertexList;

	typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        std::pair<Task<GraphType>, int> >::Type tasks(DAG.getVertNo());

	VertexList vertices;
	std::list<int> LOCALARRAY(candidates, DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		tasks[iterator->vertex] = std::make_pair(*iterator, n), vertices.push_back(iterator->vertex);

	int label = 0;
	std::stack<typename GraphType::PVertex> order;
	while(!vertices.empty())
	{
		VertexList active;
		for(typename VertexList::iterator iterator = vertices.begin(); iterator != vertices.end();)
		{
			if(DAG.getEdgeNo(*iterator, EdDirOut) == candidates[tasks[*iterator].second].size())
				active.push_back(*iterator), iterator = vertices.erase(iterator);
			else
				 ++iterator;
		}

		while(!active.empty())
		{
			typename VertexList::iterator smallest = active.begin();
//TODO: bylo (czy na pewno dobrze?)			std::list<int> &candidate = candidates[tasks[*smallest].second];
			std::list<int> *candidate = &candidates[tasks[*smallest].second];
			for(typename VertexList::iterator iterator = ++(active.begin()); iterator != active.end(); ++iterator)
			{
				std::list<int> &next = candidates[tasks[*iterator].second];
//TODO: bylo (czy na pewno dobrze?)				if(!std::lexicographical_compare(candidate.begin(), candidate.end(), next.begin(), next.end()))
				if(!std::lexicographical_compare(candidate->begin(), candidate->end(), next.begin(), next.end()))
//TODO: bylo (czy na pewno dobrze?)					smallest = iterator, candidate = next;
					smallest = iterator, candidate = &next;
			}

			for(typename GraphType::PEdge e = DAG.getEdge(*smallest, EdDirIn); e; e = DAG.getEdgeNext(*smallest, e, EdDirIn))
				candidates[tasks[DAG.getEnd(e, *smallest)].second].push_front(label);

			order.push(*smallest), active.erase(smallest), label++;
		}
	}

	int time = 0;
	while(!order.empty())
	{
		typename GraphType::PVertex v = order.top();
		schedule.machines[0].push_back(TaskPart(tasks[v].second, time, time + 1)), order.pop();
		if(!order.empty() && !DAG.getEdge(v, order.top()))
			schedule.machines[1].push_back(TaskPart(tasks[order.top()].second, time, time + 1)), order.pop();
		time++;
	}

	return time;
}

template <class DefaultStructs>
template <typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::precLiu(TaskIterator begin, TaskIterator end, const GraphType& DAG, Schedule &schedule)
{   assert(schedule.getMachNo()==1);
	typedef std::pair<typename GraphType::PVertex, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        LiuElement<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		LiuElement<Task<GraphType> > &element = tasks[iterator->vertex] = LiuElement<Task<GraphType> >(*iterator, n);
		element.duedate = iterator->duedate, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	Pair LOCALARRAY(arrival, n);
	Pair LOCALARRAY(timeout, n);
	PriQueueInterface<Pair*, compareSecondLast<Pair> > candidate(arrival, n), active(timeout, n);

	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		LiuElement<Task<GraphType> > &first = tasks[v];

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			LiuElement<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
			if(second.duedate > first.duedate)
				second.duedate = first.duedate;
		}

		if(first.degree == 0)
			candidate.push(Pair(v, first.task.release));
	}

	int time = 0, out = DefaultStructs::template NumberTypeBounds<int>::minusInfty();
	while(!candidate.empty())
	{
		typename GraphType::PVertex v = candidate.top().first;
		LiuElement<Task<GraphType> > &activated = tasks[v];
		active.push(Pair(v, activated.duedate)), candidate.pop();

		if(time < activated.task.release)
			time = activated.task.release;

		int stop = candidate.empty() ? DefaultStructs::template NumberTypeBounds<int>::plusInfty()
			: candidate.top().second;

		while(!active.empty() && time < stop)
		{
			typename GraphType::PVertex v = active.top().first;
			LiuElement<Task<GraphType> > &element = tasks[v];

			int length = std::min(element.timeleft, stop - time);
			time += length, element.timeleft -= length;

			if(schedule.machines[0].empty() || schedule.machines[0].back().task != element.index)
				schedule.machines[0].push_back(TaskPart(element.index, time - length, time, element.parts++));
			else
				schedule.machines[0].back().end += length;

			if(element.timeleft == 0)
			{
				int delay = time - element.task.duedate;
				if(out < delay)
					out = delay;

				typename GraphType::PVertex u;
				for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
				{
					LiuElement<Task<GraphType> > &next = tasks[u = DAG.getEdgeEnd(e, v)];
					if(!(--next.degree))
						candidate.push(Pair(u, next.task.release));
				}
				active.pop();
			}
		}
	}

	return out;
}

template <class DefaultStructs>
template <typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::brucker(TaskIterator begin, TaskIterator end, const GraphType& DAG, Schedule &schedule)
{
	typedef std::pair<typename GraphType::PVertex, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        BruckerElement<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		BruckerElement<Task<GraphType> > &element = tasks[iterator->vertex] = BruckerElement<Task<GraphType> >(*iterator, n);
		element.priority = 1 - iterator->duedate, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	// Wyznaczenie kolejnoœci wierzcho³ków
	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	// Aktualizacja terminów zakoñczenia zadañ, zgodnie z relacj¹ prec, i tworzenie kolejki priorytetowej zadañ dostêpnych
	Pair LOCALARRAY(timeout, n);
	PriQueueInterface<Pair*, compareSecondFirst<Pair> > active(timeout, n);
	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		BruckerElement<Task<GraphType> > &first = tasks[v];
		int priority = first.priority + 1;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			BruckerElement<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
			if(second.priority < priority)
				second.priority = priority;
		}

		if(first.degree == 0)
			active.push(Pair(v, first.priority));
	}

	int used, time = 0, out = DefaultStructs:: template NumberTypeBounds<int>::minusInfty();
	typename GraphType::PVertex LOCALARRAY(current, schedule.getMachNo());
	while(!active.empty())
	{
		time++;

		// Przydzia³ aktywnych zadañ do maszyn
		for(used = 0; used < schedule.getMachNo() && !active.empty(); used++)
		{
			BruckerElement<Task<GraphType> > &element = tasks[current[used] = active.top().first];
			schedule.machines[used].push_back(TaskPart(element.index, time - 1, time));
			out = std::max(time - element.task.duedate, out), active.pop();
		}

		// Dodanie nowych zadañ do kolejki
		for(int i = 0; i < used; i++)
		{
			typename GraphType::PVertex u, v = current[i];
			for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
			{
				BruckerElement<Task<GraphType> > &element = tasks[u = DAG.getEnd(e, v)];
				if(--element.degree == 0)
					active.push(Pair(u, element.priority));
			}
		}
	}

	return out;
}

template <class DefaultStructs>
template<typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::hu(TaskIterator begin, TaskIterator end, const GraphType& DAG, Schedule &schedule)
{
	typedef std::pair<typename GraphType::PVertex, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        BruckerElement<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		BruckerElement<Task<GraphType> > &element = tasks[iterator->vertex] = BruckerElement<Task<GraphType> >(*iterator, n);
		element.priority = 0, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	// Wyznaczenie kolejnoœci wierzcho³ków
	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	// Odwzorowanie poziomów drzewa i tworzenie kolejki priorytetowej zadañ dostêpnych
	Pair LOCALARRAY(timeout, n);
	PriQueueInterface<Pair*, compareSecondFirst<Pair> > active(timeout, n);
	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		BruckerElement<Task<GraphType> > &first = tasks[v];
		int priority = first.priority + 1;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			BruckerElement<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
			if(second.priority < priority)
				second.priority = priority;
		}

		if(first.degree == 0)
			active.push(Pair(v, first.priority));
	}

	int used, time = 0;
	typename GraphType::PVertex LOCALARRAY(current, schedule.getMachNo());
	while(!active.empty())
	{
		time++;

		// Przydzia³ aktywnych zadañ do maszyn
		for(used = 0; used < schedule.getMachNo() && !active.empty(); used++)
		{
			schedule.machines[used].push_back(TaskPart(tasks[current[used] = active.top().first].index, time - 1, time));
			active.pop();
		}

		// Dodanie nowych zadañ do kolejki
		for(int i = 0; i < used; i++)
		{
			typename GraphType::PVertex u, v = current[i];
			for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
			{
				BruckerElement<Task<GraphType> > &element = tasks[u = DAG.getEnd(e, v)];
				if(--element.degree == 0)
					active.push(Pair(u, element.priority));
			}
		}
	}

	return time;
}
