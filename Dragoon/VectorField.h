#pragma once
#include <vector>
#include "Effect.h"

class VectorField
{
private:
	int **xVectors;
	int **yVectors;
	int weight;
	int high;

	float FieldPower;
	int Range;



public:
	VectorField(int sweight, int shigh);
	float getFieldX(int x, int y);
	float getFieldY(int x, int y);
	~VectorField(void);

	void Calculate(vector<Effect*> &explosions);
};

