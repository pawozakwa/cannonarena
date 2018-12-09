#pragma once
#include <string>
#include <vector>



class LevelsManager
{


public:
	LevelsManager(void);
	static int* getLevel(std::string lvl);
	static std::vector<std::string>* getLevelsList();
	~LevelsManager(void);


};

