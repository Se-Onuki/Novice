#include "Vector3.hpp"
#include "Matrix2x2.hpp"
#include <Novice.h>

void Vector3::Printf(int x_, int y_) const {
	Novice::ScreenPrintf(x_, y_, "%12.2f // %6.2f", this->x, Length());
	Novice::ScreenPrintf(x_, y_ + TextHeight, "%12.2f", this->y);
	Novice::ScreenPrintf(x_, y_ + TextHeight * 2, "%12.2f", this->z);
}
