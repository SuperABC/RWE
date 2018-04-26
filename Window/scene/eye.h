#ifndef RWE_WINDOW_SCENE_EYE
#define RWE_WINDOW_SCENE_EYE

#include "Frame/main/main.h"
#include "Geometry/primitive/ray.h"

class Eye {
public:
	Eye();
	Eye(glm::vec3 pos);
	~Eye();

	glm::vec3 pos, center, dir, up;
	float theta, phy;
	float dist;

	glm::mat4 model;

	void sync(float theta, float phy);
	void sync(glm::vec3 dir);
	glm::mat4 view();
	void move(float dx, float dy);
	void rotate(float angx, float angy);
	void zoom(float prop);
	Ray ray(int x, int y);

	void animate(glm::vec3 pos, glm::vec3 dir);
};

#endif
