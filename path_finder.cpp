#include "path_finder.h"


//===============================================================================================================================
// constructor & destructor & init
//===============================================================================================================================
APath_Finder::~APath_Finder()
{
	Clear_Data();
}
//===============================================================================================================================
APath_Finder::APath_Finder()
{
}
//===============================================================================================================================
void APath_Finder::Init()
{
	//top
	Ways[0].X = 0;
	Ways[0].Y = -1;
	//right
	Ways[0].X = 1;
	Ways[0].Y = 0;
	//bottom
	Ways[0].X = 0;
	Ways[0].Y = 1;
	//left
	Ways[0].X = -1;
	Ways[0].Y = 0;
}
//===============================================================================================================================
// public section:
//===============================================================================================================================
void APath_Finder::Get_Graf(unsigned char* lvl_data)
{
	unsigned char i{}, j{};
	int lvl_width{}, lvl_height{};
	int index{};

	lvl_width = 40;
	lvl_height = 40;

	for (i = 0; i < lvl_height; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{

			if (*(lvl_data + index) == 0 )
			{
				Get_Nearly_Coordinates(j, i, lvl_data, &Graf[i][j]);
			}
			++index;
		}
	}
	//Print();
}

//===============================================================================================================================
void APath_Finder::Print()
{
	unsigned char len{};
	int i{}, j{}, k{};
	int lvl_width{}, lvl_height{};
	std::stringstream my_str;

	lvl_width = 40;
	lvl_height = 40;

	my_str.str(" ");

	my_str << "=============================================================================================================" << "\n";

	for (i = 0; i < lvl_height; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{
			len = Graf[i][j].Len;
			if (len > 0)
			{
				my_str << "( " << j << "," << i << " )" << " : ";
				for (k = 0; k < len; ++k)
					my_str << Graf[i][j].Pos[k].X << "," << Graf[i][j].Pos[k].Y << "; ";
				my_str << "\n";
			}
		}
	}

	my_str << "=============================================================================================================" << "\n";

	printf(my_str.str().c_str());
}
//===============================================================================================================================
void APath_Finder::Get_Path(SCoord start_coord, SCoord goal_coord, std::vector<SCoord>& path)
{
	std::map<SCoord, SCoord> visited;
	std::map<SCoord, SCoord>::iterator it;
	//std::vector<SCoord> path;

	SCoord step;

	path.erase(path.begin(), path.end());


	Dejkstra_A_Starr(start_coord, goal_coord, visited);

	path.push_back(goal_coord);
	it = visited.find(goal_coord);
	if (it != visited.end())
		step = it->second;
	else
		step = start_coord;

	while (step != start_coord)
	{
		path.push_back(step);
		it = visited.find(step);
		if (it != visited.end())
			step = it->second;
		else
			step = start_coord;
	}

	path.push_back(start_coord);

	//for debaging print result
	//std::vector<SCoord>::iterator it2;
	//std::stringstream my_str;

	//my_str.str(" ");

	//for (it2 = path.begin(); it2 != path.end(); ++it2)
	//{
	//	my_str << it2->X << " : " << it2->Y << "\n";
	//}

	//my_str << "--------------------------------------------" << "\n";

	//printf(my_str.str().c_str());

}
//===============================================================================================================================
SCoord APath_Finder::Get_Right_Free_Ceil(SCoord start)
{
	SCoord goal{};
	SNode_Elem* graf_elem;
	int node_elem_len{};
	int node_elem_pos_index{};


	graf_elem = &Graf[start.Y][start.X];
	node_elem_len = graf_elem->Len;

	node_elem_pos_index = rand() % node_elem_len;

	goal = graf_elem->Pos[node_elem_pos_index];

	return goal;
}
//===============================================================================================================================
void APath_Finder::Recalculate_Graf_Segment(int x, int y, unsigned char* lvl_data)
{
	int i{}, j{};
	int index{};
	glm::u16vec2 start_ind;
	glm::u16vec2 end_ind;

	start_ind.x = x > 0 ? x - 1 : x;
	start_ind.y = y > 0 ? y - 1 : y;

	end_ind.x = x < 40 - 1 ? x + 1 : x;
	end_ind.y = y < 40 - 1 ? y + 1 : y;

	for (i = start_ind.y; i <= end_ind.y; ++i)
		for (j = start_ind.x; j <= end_ind.x; ++j)
		{
			index = i * 40 + j;
			Graf[i][j].Len = 0;

			if (*(lvl_data + index) == 0 || *(lvl_data + index) == 3 || *(lvl_data + index) == 8 || *(lvl_data + index) == 7)
			{
				Get_Nearly_Coordinates(j, i, lvl_data, &Graf[i][j]);
			}
		}
}
//===============================================================================================================================
// private section:
//===============================================================================================================================
void APath_Finder::Clear_Data()
{
	unsigned char len{};
	int i{}, j{};
	int lvl_width{}, lvl_height{};

	lvl_width = 40;
	lvl_height = 40;

	for (i = 0; i < lvl_height; ++i)
		for (j = 0; j < lvl_width; ++j)
		{
			Graf[i][j].Len = 0;
		}
}
//===============================================================================================================================
void APath_Finder::Get_Nearly_Coordinates(unsigned char x, unsigned char y, unsigned char* lvl_data, SNode_Elem* node_elem)
{
	int index{};
	SCoord cord;

	//top ceil
	if (y > 0)
	{
		index = (y - 1) * 40 + x;
		if (*(lvl_data + index) == 0)
		{
			cord.X = x;
			cord.Y = y - 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//right ceil
	if (x < 40 - 1)
	{
		index = y * 40 + (x + 1);
		if (*(lvl_data + index) == 0)
		{
			cord.X = x + 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//bottom ceil
	if (y < 40 - 1)
	{
		index = (y + 1) * 40 + x;
		if (*(lvl_data + index) == 0)
		{
			cord.X = x;
			cord.Y = y + 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//left ceil
	if (x > 0)
	{
		index = y * 40 + (x - 1);
		if (*(lvl_data + index) == 0)
		{
			cord.X = x - 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}
}

//===============================================================================================================================
int APath_Finder::Get_Heuristick(SCoord a, SCoord b)
{
	return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
	//return abs((a.X - b.X) + (a.Y - b.Y));
}
//===============================================================================================================================
void APath_Finder::Dejkstra_A_Starr(SCoord start, SCoord goal, std::map<SCoord, SCoord>& visited)
{
	int k{}, v{};
	int len{};
	int i{};
	int cur_cost{};
	int new_cost{};
	int priority{};

	std::map<SCoord, int> queue_heap;
	std::map<SCoord, int>::iterator queue_itr, queue_itr_min;

	std::map<SCoord, int> cost_visited;

	SCoord cur_node;
	SCoord neight_node;

	queue_heap[start] = 0;

	cost_visited[start] = 0;
	visited[start] = start;

	int test{};
	int a{};

	while (queue_heap.size() > 0)
	{
		++test;
		queue_itr_min = queue_heap.begin();
		for (queue_itr = queue_heap.begin(); queue_itr != queue_heap.end(); ++queue_itr)
			if (queue_itr->second < queue_itr_min->second)
				queue_itr_min = queue_itr;

		cur_node = queue_itr_min->first;
		cur_cost = queue_itr_min->second;
		queue_heap.erase(queue_itr_min);

		if (cur_node == goal)
			break;

		k = cur_node.X;
		v = cur_node.Y;


		len = Graf[v][k].Len;


		if (len > 0)
		{
			for (i = 0; i < len; ++i)
			{
				neight_node = Graf[v][k].Pos[i];

				new_cost = cost_visited[cur_node] + 1;
				queue_itr = cost_visited.find(neight_node);
				if (queue_itr != cost_visited.end())
				{
					if (new_cost < queue_itr->second)
					{
						priority = new_cost + Get_Heuristick(neight_node, goal);
						queue_heap[neight_node] = priority;
						cost_visited[neight_node] = new_cost;
						visited[neight_node] = cur_node;
					}
				}
				else
				{
					priority = new_cost + Get_Heuristick(neight_node, goal);
					queue_heap[neight_node] = priority;
					//cost_visited[neight_node] = priority;
					cost_visited[neight_node] = new_cost;
					visited[neight_node] = cur_node;
				}
			}
		}
	}

}
//===============================================================================================================================