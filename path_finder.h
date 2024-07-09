#ifndef PATH_FINDER_CLASS_H
#define PATH_FINDER_CLASS_H

#include "common.h"


//===============================================================================================================================
// node elem for graf structure
//===============================================================================================================================
struct  SNode_Elem
{
	int Len{};
	SCoord Pos[4];
};

//===============================================================================================================================
class APath_Finder
{
public:
	~APath_Finder();
	APath_Finder();

	void Init();

	void Get_Graf(unsigned char* lvl_data);
	void Print();
	void Get_Path(SCoord start, SCoord goal, std::vector<SCoord>& path);

	SCoord Get_Right_Free_Ceil(SCoord start);

	void Recalculate_Graf_Segment(int x, int y, unsigned char* lvl_data);

	void Clear_Data();

private:
	void Get_Nearly_Coordinates(unsigned char x, unsigned char y, unsigned char* lvl_data, SNode_Elem* node_elem);
	
	int Get_Heuristick(SCoord a, SCoord b);
	void Dejkstra_A_Starr(SCoord start, SCoord goal, std::map<SCoord, SCoord>& visited);

	SCoord Ways[4];

	SNode_Elem Graf[40][40]{};
};

#endif // !PATH_FINDER_CLASS_H

