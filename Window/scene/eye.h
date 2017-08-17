#ifndef MOLE_WINDOW_SCENE_EYE
#define MOLE_WINDOW_SCENE_EYE
#include "Frame/main/main.h"
#include "Geometry/primitive/ray.h"

class Eye {
public:
	Eye();
	Eye(glm::vec3 pos);
	~Eye();

	glm::vec3 pos, dir, up;
	float theta, phy;
	float dist;

	glm::mat4 view();
	void rotate(float angx, float angy);
	void zoom(float prop);
	Ray ray(int x, int y);
};

#endif
