template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::CMax(TaskIterator begin, TaskIterator end, const Schedule &schedule)
{
	int ans = 0;
	for(typename Schedule::Type::const_iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		if(!i->empty() && ans < i->rbegin()->end)
			ans = i->rbegin()->end;
	return ans;
}
	
template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::SigmaCi(TaskIterator begin, TaskIterator end, const Schedule &schedule)
{
	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	int LOCALARRAY(finish, n);
	for(int i = 0; i < n; i++)
		finish[i] = 0;

	for(typename Schedule::Type::const_iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::const_iterator j = i->begin(); j != i->end(); ++j)
			if(finish[j->task] < j->end)
				finish[j->task] = j->end;

	int ans = 0;
	for(int i = 0; i < n; i++)
		ans += finish[i];
	return ans;
}

template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::SigmaTi(TaskIterator begin, TaskIterator end,  const Schedule &schedule)
{
	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	int LOCALARRAY(finish, n);
	for(int i = 0; i < n; i++)
		finish[i] = 0;

	for(typename Schedule::Type::const_iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::const_iterator j = i->begin(); j != i->end(); ++j)
			if(finish[j->task] < j->end)
				finish[j->task] = j->end;

	int ans = 0, i = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, i++)
		ans += finish[i] > iterator->duedate ? finish[i] - iterator->duedate : 0;
	return ans;
}

template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::SigmaUi(TaskIterator begin, TaskIterator end, const Schedule &schedule)
{
	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	int LOCALARRAY(finish, n);
	for(int i = 0; i < n; i++)
		finish[i] = 0;

	for(typename Schedule::Type::const_iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::const_iterator j = i->begin(); j != i->end(); ++j)
			if(finish[j->task] < j->end)
				finish[j->task] = j->end;

	int ans = 0, i = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, i++)
		ans += (finish[i] > iterator->duedate);
	return ans;
}

template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::LMax(TaskIterator begin, TaskIterator end,  const Schedule &schedule)
{
	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	int LOCALARRAY(finish, n);
	for(int i = 0; i < n; i++)
		finish[i] = 0;

	for(typename Schedule::Type::const_iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::const_iterator j = i->begin(); j != i->end(); ++j)
			if(finish[j->task] < j->end)
				finish[j->task] = j->end;

	int i = 0, ans = AlgorithmsDefaultSettings::NumberTypeBounds<int>::minusInfty();
	for(TaskIterator iterator = begin; iterator != end; ++iterator, i++)
	{
		int value = finish[i] - iterator->duedate;
		if(ans < value)
			ans = value;
	}
	return ans;
}

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
bool SchedulingPar<DefaultStructs>::test(TaskIterator begin, TaskIterator end, const GraphType &DAG, const Schedule &schedule,
	bool nonPmtn)
{
	typedef std::pair<TaskPart, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,TaskPart >::Type tasks(DAG.getVertNo());

	int parts = 0, n = 0;
	// D³ugoœci zadañ s¹ nieujemne
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		if(iterator->length < 0)
			return false;

	// Dwa rózne zadania nie s¹ wykonywane jednoczeœnie na jednej maszynie
	for(typename Schedule::Type::const_iterator i = schedule.machines.begin();
		i != schedule.machines.end(); parts += i->size(), ++i)
		if(!i->empty())
			for(typename Schedule::Machine::const_iterator j = i->begin(), k = j + 1; k != i->end(); ++j, ++k)
				if(j->end > k->start)
					return false;

	if(nonPmtn && n != parts)
		return false;

	TaskPart LOCALARRAY(result, n);
	Pair LOCALARRAY(available, parts);
	PriQueueInterface<Pair*, compareSecondLast<Pair> > pq(available, parts);
	
	// To samo zadanie nie jest wykonywane jednoczeœnie na obu maszynach
	for(typename Schedule::Type::const_iterator i = schedule.machines.begin();i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::const_iterator j = i->begin(); j != i->end(); ++j)
			pq.push(Pair(*j, j->start));

	while(!pq.empty())
	{
		TaskPart first = pq.top().first, &second = result[first.task];

		if(first.part != second.part)
			return false;
		if(!first.part)
			second.start = first.start;
		else if(second.end > first.start)
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
	typedef std::pair<int, int> IntPair;
	typedef std::pair<typename GraphType::PVertex, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        Element<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	IntPair LOCALARRAY(machines, schedule.getMachNo());
	PriQueueInterface<IntPair*, compareSecondLast<IntPair> > machine(machines, schedule.getMachNo());
	for(int i = 0; i < schedule.getMachNo(); i++)
		machine.push(std::make_pair(i, 0));
	
	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	Pair LOCALARRAY(candidates, n);
	Pair LOCALARRAY(actives, n);
	PriQueueInterface<Pair*, compareSecondLast<Pair> > candidate(candidates, n), active(actives, n);

	n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		Element<Task<GraphType> > &element = tasks[iterator->vertex] = Element<Task<GraphType> >(*iterator, n);
		element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);

		if(element.degree == 0)
			candidate.push(Pair(iterator->vertex, element.task.release));
	}
	
	int time = 0, out = 0;
	while(n--)
	{
		if(time < machine.top().second)
			time = machine.top().second;
		int machNo = machine.top().first;
		machine.pop();

		if(active.empty() && time < candidate.top().second)
			time = candidate.top().second;
		while(!candidate.empty() && candidate.top().second <= time)
			active.push(Pair(candidate.top().first, tasks[candidate.top().first].index)), candidate.pop();

		typename GraphType::PVertex u, v = active.top().first;
		
		int stop = time + tasks[v].task.length;
		schedule.machines[machNo].push_back(TaskPart(active.top().second, time, stop));
		machine.push(std::make_pair(machNo, stop)), active.pop();

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
		{
			Element<Task<GraphType> > &second = tasks[u = DAG.getEdgeEnd(e, v)];
			if(second.priority < stop)
				second.priority = stop;
			
			if(--second.degree == 0)
				candidate.push(Pair(u, std::max(second.priority, second.task.release)));
		}

		if(out < stop)
			out = stop;
	}

	return out;
}

template <class DefaultStructs>
template <typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::coffmanGraham(TaskIterator begin, TaskIterator end, const GraphType &DAG, Schedule &schedule)
{
	assert(schedule.getMachNo()==2);
	typedef std::list<typename GraphType::PVertex> VertexList;

	typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        std::pair<Task<GraphType>, int> >::Type tasks(DAG.getVertNo());

	VertexList vertices;
	std::list<int> LOCALARRAY(candidates, DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		tasks[iterator->vertex] = std::make_pair(*iterator, n), vertices.push_back(iterator->vertex);

	int label = 0, i = n;
	Task<GraphType> LOCALARRAY(order, n);
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
			std::list<int> &candidate = candidates[tasks[*smallest].second];
			for(typename VertexList::iterator iterator = ++(active.begin()); iterator != active.end(); ++iterator)
			{
				std::list<int> &next = candidates[tasks[*iterator].second];
				if(!std::lexicographical_compare(candidate.begin(), candidate.end(), next.begin(), next.end()))
					smallest = iterator, candidate = next;
			}

			for(typename GraphType::PEdge e = DAG.getEdge(*smallest, EdDirIn); e; e = DAG.getEdgeNext(*smallest, e, EdDirIn))
				candidates[tasks[DAG.getEnd(e, *smallest)].second].push_front(label);

			order[--i] = tasks[*smallest].first, active.erase(smallest), label++;
		}
	}
	
	int out = SchedulingPar<DefaultStructs>::ls(order, order + n, DAG, schedule);
	for(typename Schedule::Type::iterator i = schedule.machines.begin(); i != schedule.machines.end(); ++i)
		for(typename Schedule::Machine::iterator j = i->begin(); j != i->end(); ++j)
			j->task = tasks[order[j->task].vertex].second;

	return out;
}

template <class DefaultStructs>
template <typename GraphType, typename TaskIterator>
int SchedulingPar<DefaultStructs>::precLiu(TaskIterator begin, TaskIterator end, const GraphType& DAG, Schedule &schedule)
{   assert(schedule.getMachNo()==1);
	typedef std::pair<typename GraphType::PVertex, int> Pair;

    typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
        Element<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		Element<Task<GraphType> > &element = tasks[iterator->vertex] = Element<Task<GraphType> >(*iterator, n);
		element.duedate = iterator->duedate, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	Pair LOCALARRAY(candidates, n);
	Pair LOCALARRAY(actives, n);
	PriQueueInterface<Pair*, compareSecondLast<Pair> > candidate(candidates, n), active(actives, n);

	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		Element<Task<GraphType> > &first = tasks[v];

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			Element<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
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
		Element<Task<GraphType> > &activated = tasks[v];
		active.push(Pair(v, activated.duedate)), candidate.pop();

		if(time < activated.task.release)
			time = activated.task.release;

		int stop = candidate.empty() ? DefaultStructs::template NumberTypeBounds<int>::plusInfty()
			: candidate.top().second;

		while(!active.empty() && time < stop)
		{
			typename GraphType::PVertex v = active.top().first;
			Element<Task<GraphType> > &element = tasks[v];

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
					Element<Task<GraphType> > &next = tasks[u = DAG.getEdgeEnd(e, v)];
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
        Element<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		Element<Task<GraphType> > &element = tasks[iterator->vertex] = Element<Task<GraphType> >(*iterator, n);
		element.priority = 1 - iterator->duedate, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	// Wyznaczenie kolejnoœci wierzcho³ków
	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	// Aktualizacja terminów zakoñczenia zadañ, zgodnie z relacj¹ prec, i tworzenie kolejki priorytetowej zadañ dostêpnych
	Pair LOCALARRAY(actives, n);
	PriQueueInterface<Pair*, compareSecondFirst<Pair> > active(actives, n);
	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		Element<Task<GraphType> > &first = tasks[v];
		int priority = first.priority + 1;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			Element<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
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
			Element<Task<GraphType> > &element = tasks[current[used] = active.top().first];
			schedule.machines[used].push_back(TaskPart(element.index, time - 1, time));
			out = std::max(time - element.task.duedate, out), active.pop();
		}

		// Dodanie nowych zadañ do kolejki
		for(int i = 0; i < used; i++)
		{
			typename GraphType::PVertex u, v = current[i];
			for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirOut); e; e = DAG.getEdgeNext(v, e, EdDirOut))
			{
				Element<Task<GraphType> > &element = tasks[u = DAG.getEnd(e, v)];
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
        Element<Task<GraphType> > >::Type tasks(DAG.getVertNo());

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		Element<Task<GraphType> > &element = tasks[iterator->vertex] = Element<Task<GraphType> >(*iterator, n);
		element.priority = 0, element.degree = DAG.getEdgeNo(iterator->vertex, EdDirIn);
	}

	// Wyznaczenie kolejnoœci wierzcho³ków
	typename GraphType::PVertex LOCALARRAY(vertices, DAG.getVertNo());
	Koala::DAGAlgs::topOrd(DAG, vertices);

	// Odwzorowanie poziomów drzewa i tworzenie kolejki priorytetowej zadañ dostêpnych
	Pair LOCALARRAY(actives, n);
	PriQueueInterface<Pair*, compareSecondFirst<Pair> > active(actives, n);
	for(int i = DAG.getVertNo() - 1; i >= 0; i--)
	{
		typename GraphType::PVertex v = vertices[i];
		Element<Task<GraphType> > &first = tasks[v];
		int priority = first.priority + 1;

		for(typename GraphType::PEdge e = DAG.getEdge(v, EdDirIn); e; e = DAG.getEdgeNext(v, e, EdDirIn))
		{
			Element<Task<GraphType> > &second = tasks[DAG.getEdgeEnd(e, v)];
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
				Element<Task<GraphType> > &element = tasks[u = DAG.getEnd(e, v)];
				if(--element.degree == 0)
					active.push(Pair(u, element.priority));
			}
		}
	}

	return time;
}

template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::mcNaughton(TaskIterator begin, TaskIterator end, Schedule &schedule)
{
	int out = 0, n = 0, length = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
	{
		out += iterator->length;
		if(length < iterator->length)
			length = iterator->length;
	}

	out = std::max((out + n - 1) / n, length), n = 0;
	int index = 0, time = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		if(time + iterator->length < out)
		{
			schedule.machines[index].push_back(TaskPart(n, time, time + iterator->length));
			time += iterator->length;
		}
		else if(time + iterator->length == out)
		{
			schedule.machines[index++].push_back(TaskPart(n, time, out)), time = 0;
		}
		else
		{
			schedule.machines[index++].push_back(TaskPart(n, time, out, 1));
			time = time + iterator->length - out;
			schedule.machines[index].push_back(TaskPart(n, 0, time));
		}

	return out;
}

template <class DefaultStructs>
template<typename TaskIterator>
int SchedulingPar<DefaultStructs>::hodgson(TaskIterator begin, TaskIterator end, Schedule &schedule)
{
	typedef std::pair<int, int> Pair;

	int n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++);

	int LOCALARRAY(tasks, n);
	Scheduling::sortEDD(begin, end, tasks);

	HodgsonElement LOCALARRAY(info, n);
	n = 0;
	for(TaskIterator iterator = begin; iterator != end; ++iterator, n++)
		info[n] = HodgsonElement(n, iterator->length, iterator->duedate);
	
	Pair LOCALARRAY(actives, n);
	PriQueueInterface<Pair*, compareSecondFirst<Pair> > active(actives, n);

	int out = 0, sum = 0;
	for(int i = 0; i < n; i++)
	{
		HodgsonElement &element = info[tasks[i]]; 
		sum += element.length;
		active.push(Pair(element.index, element.length));
		if(sum > info[active.top().first].duedate)
		{
			sum -= active.top().second, info[active.top().first].late = 1, out++;
			active.pop();
		}
	}
	
	int time = 0;
	for(int i = 0; i < n; i++)
	{
		HodgsonElement &element = info[tasks[i]]; 
		if(!element.late)
			schedule.machines[0].push_back(TaskPart(element.index, time, time + element.length)), time += element.length;
	}
	for(int i = 0; i < n; i++)
		if(info[i].late)
			schedule.machines[0].push_back(TaskPart(i, time, time + info[i].length)), time += info[i].length;

	return out;
}