#include "CollisionEngine.h"
#include <math.h>

CollisionEngine::CollisionEngine(void)
{
}

bool CollisionEngine::isCollidding(GameObject *a, GameObject *b, bool perfect){

	int xmaxA = a->GetX() + a->picW; // right end of a
	int ymaxA = a->GetY() + a->picH; // down end of a
	int xmaxB = b->GetX() + b->picW; //	right end of b
	int ymaxB = b->GetY() + b->picH; // down end of b

	int xmin = fmax(a->GetX(), b->GetX()); // top end of test
	int ymin = fmax(a->GetY(), b->GetY()); // left end of test

	int xmax = fmin(xmaxA, xmaxB); // bottom end of test
	int ymax = fmin(ymaxA, ymaxB); // right end of test

	if (xmaxA < b->GetX() || ymaxA < b->GetY()) {
		return false;
	}
	if (xmaxB < a->GetX() || ymaxB < a->GetY()) {
		return false;
	}

	if(!perfect)
		return true;

	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			int ax = x - xmin + (xmin - a->GetX()); 
			int ay = y - ymin + (ymin - a->GetY());
			int bx = x - xmin + (xmin - b->GetX());
			int by = y - ymin + (ymin - b->GetY());
			
			if(a->GetBitMap()[ax][ay] && b->GetBitMap()[bx][by])
					return true;
		}
	}

	return false;
}



CollisionEngine::~CollisionEngine(void)
{
}
