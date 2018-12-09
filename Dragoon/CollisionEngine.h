#pragma once
#include "GameObject.h"
#include "Player.h"

class CollisionEngine
{
public:
	
	static bool isCollidding(GameObject* a, GameObject* b, bool perfect);

private:
	CollisionEngine(void);
	~CollisionEngine(void);
};

