#include "Frame/main/main.h"
#include "Frame/shader/shader.h"
#include "light.h"
#include "model.h"
#include "eye.h"

Shader *elementShader;
Shader *texShader;
Shader *shadowShader;

extern vector<Scene*> globe;
extern vector<Light*> light;
extern Eye *eye;

Element *Element::load(const char *filename) {
	std::ifstream fin;

	fin.open(filename);
	if (fin.is_open() == FALSE)return NULL;

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
				glm::vec3 edge1(pos[p2 * 3 - 3] - pos[p1 * 3 - 3],
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
	return this;
}
void Element::shadow() {
	shadowShader->use();
	shadowShader->setMat4("u_modelMatrix", model);

	glBindVertexArray(svao);
	glBindBuffer(GL_ARRAY_BUFFER, spositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, getPos(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindVertexArray(0);

	glBindVertexArray(svao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
}
void Element::show() {
	elementShader->use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, getPos(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, color.size() * 4, getColor(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * 4, getNormal(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindVertexArray(0);

	if(diy){
		elementShader->setVec3("u_lightDiff", kd);
		elementShader->setVec3("u_lightAmb", ka);
		elementShader->setFloat("u_lightSpec", ks);
	}

	elementShader->setMat4("u_modelMatrix", model);
	elementShader->setInt("u_shadowMap", 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
}

void Texture::pic(const char *fileName) {
	CImage *img = new CImage;
	if (!fileName) {
		return;
	}
	HRESULT hr = img->Load(fileName);
	if (!SUCCEEDED(hr)) {
		return;
	}
	src.sizeX = img->GetWidth();
	src.sizeY = img->GetHeight();
	if (img->GetPitch()<0)src.data = (unsigned char *)img->GetBits() + (img->GetPitch()*(img->GetHeight() - 1));
	else src.data = (unsigned char *)img->GetBits();
}
Texture *Texture::load(const char *filename) {
	std::ifstream fin;

	fin.open(filename);
	if (fin.is_open() == FALSE)return NULL;

	string op;
	float num1, num2, num3;

	vector<float>pos;
	vector<float>coord;
	vector<float>normal;

	vector<string> texName;
	vector<string> texDir;

	while (fin >> op) {
		if (op == "v") {
			fin >> num1 >> num2 >> num3;
			pos.push_back(num1);
			pos.push_back(num3);
			pos.push_back(num2);
		}
		else if (op == "vt") {
			fin >> num1 >> num2;
			coord.push_back(num1);
			coord.push_back(num2);
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
				this->pushCoord();
				this->pushCoord();
				this->pushCoord();
			}
			else {
				num1 = (float)atoi(v1.c_str() + tmp1);
				num2 = (float)atoi(v2.c_str() + tmp2);
				num3 = (float)atoi(v3.c_str() + tmp3);
				this->pushCoord(coord[(int)num1 * 2 - 2], coord[(int)num1 * 2 - 1]);
				this->pushCoord(coord[(int)num2 * 2 - 2], coord[(int)num2 * 2 - 1]);
				this->pushCoord(coord[(int)num3 * 2 - 2], coord[(int)num3 * 2 - 1]);
			}

			tmp1 = v1.find_first_of('/', tmp1) + 1;
			tmp2 = v2.find_first_of('/', tmp2) + 1;
			tmp3 = v3.find_first_of('/', tmp3) + 1;
			if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
				int p1 = atoi(v1.c_str());
				int p2 = atoi(v2.c_str());
				int p3 = atoi(v3.c_str());
				glm::vec3 edge1(pos[p2 * 3 - 3] - pos[p1 * 3 - 3],
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

			bool complete = true;
			while (min >> op) {
				if (op == "newmtl") {
					if (!complete)texDir.push_back("");
					min >> op;
					texName.push_back(op);
					complete = false;
				}
				else if (op == "src") {
					if (complete)texName.push_back("");
					min >> op;
					texDir.push_back(op);
					complete = true;
				}
				else continue;
			}
		}
		else if (op == "usemtl") {
			fin >> op;
			for (unsigned int i = 0; i < texName.size(); i++) {
				if (op == texName[i]) {
					pic(texDir[i].c_str());
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
	
	return this;
}
void Texture::shadow() {
	shadowShader->use();
	shadowShader->setMat4("u_modelMatrix", model);

	glBindVertexArray(svao);
	glBindBuffer(GL_ARRAY_BUFFER, spositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, getPos(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindVertexArray(0);

	glBindVertexArray(svao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
}
void Texture::show() {
	texShader->use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, getPos(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, coordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, coord.size() * 4, getCoord(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * 4, getNormal(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src.sizeX, src.sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, src.data);
	
	if(diy){
		texShader->setVec3("u_lightDiff", kd);
		texShader->setVec3("u_lightAmb", ka);
		texShader->setFloat("u_lightSpec", ks);
	}

	texShader->setMat4("u_modelMatrix", model);
	texShader->setInt("u_shadowMap", 0);
	texShader->setInt("u_textureMap", 1);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
}

void Scene::shadow() {
	for (auto &e : elements)e.shadow();
	for (auto &t : textures)t.shadow();
}
void Scene::show(GLuint lightId) {
	for (auto &e : elements) {
		e.show();
		if (e.diy) {
			elementShader->setVec3("u_lightDiff", light[lightId]->diffuse);
			elementShader->setVec3("u_lightAmb", light[lightId]->ambient);
			elementShader->setFloat("u_lightSpec", light[lightId]->specular);
		}
	}
	for (auto &t : textures) { 
		t.show();
		if (t.diy) {
			elementShader->setVec3("u_lightDiff", light[lightId]->diffuse);
			elementShader->setVec3("u_lightAmb", light[lightId]->ambient);
			elementShader->setFloat("u_lightSpec", light[lightId]->specular);
		}
	}
}

