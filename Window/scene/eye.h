#ifndef MOLE_WINDOW_SCENE_EYE
#define MOLE_WINDOW_SCENE_EYE
#include "Frame/main/main.h"

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
};

#endif
