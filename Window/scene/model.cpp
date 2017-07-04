#include "Frame/main/main.h"
#include "Frame/shader/shader.h"
#include "model.h"
#include "eye.h"

Shader *elementShader;
Shader *texShader;
Shader *shadowShader;

extern vector<Scene*> globe;
extern Eye *eye;

void Element::load(const char *filename) {
	std::ifstream fin;

	fin.open(filename);
	if (fin.is_open() == FALSE)return;

	string op;
	float num1, num2, num3;

	vector<float>pos;
	vector<float>color;
	vector<float>normal;

	while (fin >> op) {
		if (op == "v") {
			fin >> num1 >> num2 >> num3;
			pos.push_back(num1);
			pos.push_back(num3);
			pos.push_back(num2);
		}
		else if (op == "vc") {
			fin >> num1 >> num2 >> num3;
			color.push_back(num1);
			color.push_back(num2);
			color.push_back(num3);
		}
		else if (op == "vn") {
			fin >> num1 >> num2 >> num3;
			normal.push_back(num1);
			normal.push_back(num2);
			normal.push_back(num3);
		}
		else if (op == "f") {
			string v1, v2, v3;
			int tmp1 = 0, tmp2 = 0, tmp3 = 0;

			fin >> v1 >> v2 >> v3;
			num1 = (float)atoi(v1.c_str() + tmp1);
			num2 = (float)atoi(v2.c_str() + tmp2);
			num3 = (float)atoi(v3.c_str() + tmp3);
			this->pushPos(pos[(int)num1 * 3 - 3], pos[(int)num1 * 3 - 2], pos[(int)num1 * 3 - 1]);
			this->pushPos(pos[(int)num2 * 3 - 3], pos[(int)num2 * 3 - 2], pos[(int)num2 * 3 - 1]);
			this->pushPos(pos[(int)num3 * 3 - 3], pos[(int)num3 * 3 - 2], pos[(int)num3 * 3 - 1]);

			tmp1 = v1.find_first_of('/', tmp1) + 1;
			tmp2 = v2.find_first_of('/', tmp2) + 1;
			tmp3 = v3.find_first_of('/', tmp3) + 1;
			if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
				if (active == -1) {
					this->pushColor(1, 1, 1);
					this->pushColor(1, 1, 1);
					this->pushColor(1, 1, 1);
				}
				else {
					this->pushColor(surface[active].kd.r, surface[active].kd.g, surface[active].kd.b);
					this->pushColor(surface[active].kd.r, surface[active].kd.g, surface[active].kd.b);
					this->pushColor(surface[active].kd.r, surface[active].kd.g, surface[active].kd.b);
				}
			}
			else {
				num1 = (float)atoi(v1.c_str() + tmp1);
				num2 = (float)atoi(v2.c_str() + tmp2);
				num3 = (float)atoi(v3.c_str() + tmp3);
				this->pushColor(color[(int)num1 * 3 - 3], color[(int)num1 * 3 - 2], color[(int)num1 * 3 - 1]);
				this->pushColor(color[(int)num2 * 3 - 3], color[(int)num2 * 3 - 2], color[(int)num2 * 3 - 1]);
				this->pushColor(color[(int)num3 * 3 - 3], color[(int)num3 * 3 - 2], color[(int)num3 * 3 - 1]);
			}

			tmp1 = v1.find_first_of('/', tmp1) + 1;
			tmp2 = v2.find_first_of('/', tmp2) + 1;
			tmp3 = v3.find_first_of('/', tmp3) + 1;
			if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
				int p1 = atoi(v1.c_str());
				int p2 = atoi(v2.c_str());
				int p3 = atoi(v3.c_str());
				glm::vec3 edge1(pos[p2*3 - 3]-pos[p1*3 - 3],
					pos[p2 * 3 - 2] - pos[p1 * 3 - 2], 
					pos[p2 * 3 - 1] - pos[p1 * 3 - 1]);
				glm::vec3 edge2(pos[p3 * 3 - 3] - pos[p2 * 3 - 3],
					pos[p3 * 3 - 2] - pos[p2 * 3 - 2],
					pos[p3 * 3 - 1] - pos[p2 * 3 - 1]);
				glm::vec3 norm(edge2.y *edge1.z - edge1.y*edge2.z,
					edge2.z*edge1.x - edge1.z*edge2.x,
					edge2.x*edge1.y - edge1.x*edge2.y);
				this->pushNormal(norm.x, norm.y, norm.z);
				this->pushNormal(norm.x, norm.y, norm.z);
				this->pushNormal(norm.x, norm.y, norm.z);
			}
			else {
				num1 = (float)atoi(v1.c_str() + tmp1);
				num2 = (float)atoi(v2.c_str() + tmp2);
				num3 = (float)atoi(v3.c_str() + tmp3);
				this->pushNormal(normal[(int)num1 * 3 - 3], normal[(int)num1 * 3 - 2], normal[(int)num1 * 3 - 1]);
				this->pushNormal(normal[(int)num2 * 3 - 3], normal[(int)num2 * 3 - 2], normal[(int)num2 * 3 - 1]);
				this->pushNormal(normal[(int)num3 * 3 - 3], normal[(int)num3 * 3 - 2], normal[(int)num3 * 3 - 1]);
			}
		}
		else if (op == "mtllib") {
			std::ifstream min;
			string path = string(filename);
			unsigned int tmp = path.find_last_of('/');
			for (unsigned int i = path.length() - 1; i > tmp; i--)
				path.pop_back();
			fin >> op;
			path += op;

			min.open(path);
			if (min.is_open() == FALSE)continue;
			while (min >> op) {
				if (op == "newmtl") {
					surface.push_back(Material());
					min >> op;
					surface[surface.size() - 1].name = op;
				}
				else if (op == "Kd") {
					min >> num1 >> num2 >> num3;
					surface[surface.size() - 1].kd = glm::vec3(num1, num2, num3);
				}
				else if (op == "Ka") {
					min >> num1 >> num2 >> num3;
					surface[surface.size() - 1].ka = glm::vec3(num1, num2, num3);
				}
				else if (op == "Ks") {
					min >> num1 >> num2 >> num3;
					surface[surface.size() - 1].ks = glm::vec3(num1, num2, num3);
				}
				else {
					char *buf = new char[256];
					min.getline(buf, 256);
					delete buf;
				}
			}
		}
		else if (op == "usemtl") {
			fin >> op;
			active = -1;
			for (unsigned int i = 0; i < surface.size(); i++) {
				if (surface[i].name == op) {
					active = i;
					break;
				}
			}
		}
		else {
			char *buf = new char[256];
			fin.getline(buf, 256);
			delete buf;
		}
	}
}
void Element::shadow() {
	float *positionData = &pos[0];
	glBindBuffer(GL_ARRAY_BUFFER, spositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, positionData, GL_STATIC_DRAW);

	glBindVertexArray(svao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, spositionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	glBindVertexArray(svao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}
void Element::show() {
	float *positionData = &pos[0];
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, positionData, GL_STATIC_DRAW);
	float *colorData = &color[0];
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, color.size() * 4, colorData, GL_STATIC_DRAW);
	float *normalData = &normal[0];
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * 4, normalData, GL_STATIC_DRAW);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Scene::shadow() {
	for (auto &e : elements)e.shadow();
}
void Scene::show() {
	glUniform1i(glGetUniformLocation(elementShader->program, "u_StaticView"), 0);

	glm::mat4 perspectMatrix;
	perspectMatrix = glm::perspective(70.f, 800.f / 600.f, .1f, 1000000.f);
	glUniformMatrix4fv(glGetUniformLocation(elementShader->program, "u_PerspectMatrix"), 1, false, glm::value_ptr(perspectMatrix));
	glm::mat4 viewMatrix;
	viewMatrix = eye->view();
	glUniformMatrix4fv(glGetUniformLocation(elementShader->program, "u_ViewMatrix"), 1, false, glm::value_ptr(viewMatrix));

	for (auto &e : elements)e.show();
}
