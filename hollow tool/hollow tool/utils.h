#pragma once

#define PI 3.141592f
#define PI2 PI * 2
#define TORADIAN(x) ((x) * PI / 180)
#define TODEGREE(x) ((x) * 180 / PI)

namespace MACRO_FUNC {
	float getDistance(float x1, float y1, float x2, float y2);
	float getAngle(float x1, float y1, float x2, float y2);
}