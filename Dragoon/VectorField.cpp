#include "VectorField.h"

// Pass here s
VectorField::VectorField(int sweight, int shigh)
{
	FieldPower = 75.f;

	this->weight = sweight/10;
	this->high = shigh/10;

	xVectors = new int*[this->weight];
	yVectors = new int*[this->weight];

	for (int i = 0; i < this->weight; i++){		
		xVectors[i] = new int[this->high];
		yVectors[i] = new int[this->high];

		for (int j = 0; j < this->high; j++){
			xVectors[i][j] = 0;
			yVectors[i][j] = 0;
		}
	}
}

void VectorField::Calculate(vector <Effect*> &explosions){
	for (int j = 0; j < weight; j++){
		for (int k = 0; k < high; k++){			
			xVectors[j][k] = 0;
			yVectors[j][k] = 0;
			for (int i = 0; i < explosions.size(); i++){
				Range = 1.5 * explosions.at(i)->picH;

				int dX = j*10 - explosions.at(i)->GetOriginX();
				int dY = k*10 - explosions.at(i)->GetOriginY();
				float distance = sqrt(dX*dX + dY*dY);

				if(distance < Range){
					float a = (Range - distance)/Range;
					a *= a;
					xVectors[j][k] += dX * FieldPower * (1/distance);
					yVectors[j][k] += dY * FieldPower * (1/distance);
				}
			}
		}
	}
}

float VectorField::getFieldX(int x, int y){
	x /= 10;
	y /= 10;

	if(x < 128 && y < 76)
		return xVectors[x][y];
	else 
		return NULL;
}

float VectorField::getFieldY(int x, int y){
	x /= 10;
	y /= 10;

	if(x < 128 && y < 76)


		return yVectors[x][y];
	else 
		return NULL;
}

VectorField::~VectorField(void){
	delete xVectors;
	delete yVectors;
}
