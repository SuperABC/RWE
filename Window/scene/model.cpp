#include "Frame/main/main.h"
#include "Frame/shader/shader.h"
#include "Window/scene/light.h"
#include "Window/scene/model.h"
#include "Window/scene/eye.h"

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
	if (pos.size() == 0)return;
	if (pure)return;

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
	if (pos.size() == 0)return;

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
		for (unsigned int i = 0; i < light.size(); i++) {
			char tmp[64];
			sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
			elementShader->setVec3(tmp, kd * light[i]->diffuse);
			sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
			elementShader->setVec3(tmp, ka * light[i]->ambient);
			sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
			elementShader->setFloat(tmp, ks * light[i]->specular);
		}
	}

	elementShader->setInt("u_enLight", !pure);
	elementShader->setMat4("u_modelMatrix", model);
	glm::mat4x4 inv = glm::transpose(glm::inverse(model));
	elementShader->setMat4("u_normalMatrix", inv);
	for (unsigned int i = 0; i < light.size(); i++) {
		char tmp[64];
		sprintf(tmp, "u_shadowMap[%d]", i);
		elementShader->setInt(tmp, i+2);
	}
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);

	if (diy) {
		for (unsigned int i = 0; i < light.size(); i++) {
			char tmp[64];
			sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
			elementShader->setVec3(tmp, light[i]->diffuse);
			sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
			elementShader->setVec3(tmp, light[i]->ambient);
			sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
			elementShader->setFloat(tmp, light[i]->specular);
		}
	}
}

void Texture::pic(texStr s) {
	if (s.pure == true) {
		Bitmap bmp;
		bmp.sizeX = 1;
		bmp.sizeY = 1;
		bmp.data = new unsigned char[3];

		bmp.data[0] = char(s.Kd[0] * 255);
		bmp.data[1] = char(s.Kd[1] * 255);
		bmp.data[2] = char(s.Kd[2] * 255);

		src = bmp;
		return;
	}

	src = bmp(s.texDir);
}
Bitmap Texture::bmp(string s) {
	const char *fileName = s.c_str();

	CImage *img = new CImage;

	HRESULT hr = img->Load(fileName);
	if (!SUCCEEDED(hr)) {
		exit(0);
	}
	Bitmap bmp;
	bmp.sizeX = img->GetWidth();
	bmp.sizeY = img->GetHeight();
	if (img->GetPitch()<0)bmp.data = (unsigned char *)img->GetBits() + (img->GetPitch()*(img->GetHeight() - 1));
	else bmp.data = (unsigned char *)img->GetBits();

	return bmp;
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
	vector<texStr> texInfo;

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
					if (!complete)texInfo.push_back(texStr(""));
					min >> op;
					texName.push_back(op);
					complete = false;
				}
				else if (op == "map_Kd") {
					assert(!complete);
					min >> op;
					texInfo.push_back(texStr(op));
					complete = true;
				}
				else if (op == "map_Bump") {
					bump = true;
					assert(complete);
					min >> op;
					texInfo[texInfo.size() - 1].bumpDir = op;
				}
				else if (op == "Kd") {
					min >> num1 >> num2 >> num3;
					if (complete) {
						texInfo[texInfo.size() - 1].Kd[0] = num1;
						texInfo[texInfo.size() - 1].Kd[1] = num2;
						texInfo[texInfo.size() - 1].Kd[2] = num3;
					}
					else {
						float *tmp = new float[4];
						tmp[0] = num1;
						tmp[1] = num2;
						tmp[2] = num3;
						texInfo.push_back(texStr(tmp, NULL, NULL));
						complete = true;
					}
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
			for (unsigned int i = 0; i < texName.size(); i++) {
				if (op == texName[i]) {
					offset.push_back(pair<int, int>(this->pos.size() / 3, i));
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

	for (unsigned int i = 0; i < texName.size(); i++) {
		GLuint tmp;
		glGenTextures(1, &tmp);
		this->texName.push_back(tmp);

		pic(texInfo[i]);

		if (texInfo[i].bumpDir != "") {
			GLuint tmp;
			glGenTextures(1, &tmp);
			this->bumpName.push_back(tmp);

			Bitmap bumpSrc = bmp(texInfo[i].bumpDir);

			glBindTexture(GL_TEXTURE_2D, tmp);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bumpSrc.sizeX, bumpSrc.sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bumpSrc.data);
		}
		else this->bumpName.push_back(0);

		glBindTexture(GL_TEXTURE_2D, this->texName[this->texName.size()-1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src.sizeX, src.sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, src.data);
	}

	return this;
}
void Texture::shadow() {
	if (pos.size() == 0)return;
	if (pure)return;

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
	if (pos.size() == 0)return;

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

	texShader->setInt("u_enBump", bump);

	if(diy){
		for (unsigned int i = 0; i < light.size(); i++) {
			char tmp[64];
			sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
			texShader->setVec3(tmp, kd * light[i]->diffuse);
			sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
			texShader->setVec3(tmp, ka * light[i]->ambient);
			sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
			texShader->setFloat(tmp, ks * light[i]->specular);
		}
	}

	texShader->setInt("u_enLight", !pure);
	texShader->setMat4("u_modelMatrix", model);
	glm::mat4x4 inv = glm::transpose(glm::inverse(model));
	texShader->setMat4("u_normalMatrix", inv);
	for (unsigned int i = 0; i < light.size(); i++) {
		char tmp[64];
		sprintf(tmp, "u_shadowMap[%d]", i);
		texShader->setInt(tmp, i + 2);
	}
	texShader->setInt("u_textureMap", 0);
	texShader->setInt("u_bumpMap", 1);
	glBindVertexArray(vao);


	int tmpSum = 0;
	for (unsigned int i = 0; i < offset.size(); i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texName[offset[i].second]);
		if (bumpName[offset[i].second] != 0) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, bumpName[offset[i].second]);
			texShader->setInt("u_enBump", 1);
		}
		else {
			texShader->setInt("u_enBump", 0);
		}
		if (i != offset.size() - 1) {
			glDrawArrays(GL_TRIANGLES, tmpSum, offset[i+1].first - offset[i].first);
			tmpSum += offset[i+1].first - offset[i].first;
		}
		else {
			glDrawArrays(GL_TRIANGLES, tmpSum, this->pos.size() / 3 - offset[i].first);
		}
	}

	if (diy) {
		for (unsigned int i = 0; i < light.size(); i++) {
			char tmp[64];
			sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
			texShader->setVec3(tmp, light[i]->diffuse);
			sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
			texShader->setVec3(tmp, light[i]->ambient);
			sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
			texShader->setFloat(tmp, light[i]->specular);
		}
	}
}

void Scene::shadow() {
	for (auto &e : elements)e->shadow();
	for (auto &t : textures)t->shadow();
}
void Scene::show() {
	for (auto &e : elements)e->show();
	for (auto &t : textures)t->show();
	for (auto &c : canvases)c->show();
}

void Element::setColor(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}
void Element::addBall(float radius, float slice) {
	if (slice < 1.f)return;

	float angleSpan = 45.f/slice;
	vector<float>tmp;
	for (float i = -90; i <= 90; i += angleSpan) {
		for (float j = 0; j < 360; j += angleSpan) {
			float r = radius*glm::cos(glm::radians(i));
			float x = r*glm::cos(glm::radians(j));
			float y = radius*glm::sin(glm::radians(i));
			float z = r*glm::sin(glm::radians(j));
			tmp.push_back(x);
			tmp.push_back(y);
			tmp.push_back(z);
		}
	}

	int row = int(180 / angleSpan) + 1;
	int col = int(360 / angleSpan);
	int k = col*(row - 2) * 6 * 8;
	int count = 0;
	for (int i = 0; i < row; i++) {
		if (i != 0 && i != row - 1) {
			for (int j = 0; j < col; j++) {
				k = i*col + j;
				glm::vec3 norm = glm::normalize(glm::vec3(tmp[(k + col) * 3], tmp[(k + col) * 3 + 1], tmp[(k + col) * 3 + 2]));
				pushPos(tmp[(k + col) * 3], tmp[(k + col) * 3 + 1], tmp[(k + col) * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);

				int index = k + 1;
				if (j == col - 1)
					index -= col;
				norm = glm::normalize(glm::vec3(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]));
				pushPos(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);

				norm = glm::normalize(glm::vec3(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]));
				pushPos(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);
			}
			for (int j = 0; j < col; j++) {
				k = i*col + j;
				glm::vec3 norm = glm::normalize(glm::vec3(tmp[(k - col) * 3], tmp[(k - col) * 3 + 1], tmp[(k - col) * 3 + 2]));
				pushPos(tmp[(k - col) * 3], tmp[(k - col) * 3 + 1], tmp[(k - col) * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);

				int index = k - 1;
				if (j == 0)
					index += col;
				norm = glm::normalize(glm::vec3(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]));
				pushPos(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);

				norm = glm::normalize(glm::vec3(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]));
				pushPos(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]);
				pushColor(r, g, b);
				pushNormal(norm.x, norm.y, norm.z);
			}
		}
	}
}
void Element::addBox(float width) {
	float half = width / 2;

	pushPos(half, half, half);
	pushPos(half, -half, half);
	pushPos(half, -half, -half);
	pushPos(half, half, half);
	pushPos(half, -half, -half);
	pushPos(half, half, -half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(1.f, 0.f, 0.f);
	pushNormal(1.f, 0.f, 0.f);
	pushNormal(1.f, 0.f, 0.f);
	pushNormal(1.f, 0.f, 0.f);
	pushNormal(1.f, 0.f, 0.f);
	pushNormal(1.f, 0.f, 0.f);
	pushPos(-half, half, half);
	pushPos(-half, -half, half);
	pushPos(-half, -half, -half);
	pushPos(-half, half, half);
	pushPos(-half, -half, -half);
	pushPos(-half, half, -half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(-1.f, 0.f, 0.f);
	pushNormal(-1.f, 0.f, 0.f);
	pushNormal(-1.f, 0.f, 0.f);
	pushNormal(-1.f, 0.f, 0.f);
	pushNormal(-1.f, 0.f, 0.f);
	pushNormal(-1.f, 0.f, 0.f);

	pushPos(half, half, half);
	pushPos(-half, half, half);
	pushPos(-half, half, -half);
	pushPos(half, half, half);
	pushPos(-half, half, -half);
	pushPos(half, half, -half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(0.f, 1.f, 0.f);
	pushNormal(0.f, 1.f, 0.f);
	pushNormal(0.f, 1.f, 0.f);
	pushNormal(0.f, 1.f, 0.f);
	pushNormal(0.f, 1.f, 0.f);
	pushNormal(0.f, 1.f, 0.f);
	pushPos(half, -half, half);
	pushPos(-half, -half, half);
	pushPos(-half, -half, -half);
	pushPos(half, -half, half);
	pushPos(-half, -half, -half);
	pushPos(half, -half, -half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(0.f, -1.f, 0.f);
	pushNormal(0.f, -1.f, 0.f);
	pushNormal(0.f, -1.f, 0.f);
	pushNormal(0.f, -1.f, 0.f);
	pushNormal(0.f, -1.f, 0.f);
	pushNormal(0.f, -1.f, 0.f);

	pushPos(half, half, half);
	pushPos(-half, half, half);
	pushPos(-half, -half, half);
	pushPos(half, half, half);
	pushPos(-half, -half, half);
	pushPos(half, -half, half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(0.f, 0.f, 1.f);
	pushNormal(0.f, 0.f, 1.f);
	pushNormal(0.f, 0.f, 1.f);
	pushNormal(0.f, 0.f, 1.f);
	pushNormal(0.f, 0.f, 1.f);
	pushNormal(0.f, 0.f, 1.f);
	pushPos(half, half, -half);
	pushPos(-half, half, -half);
	pushPos(-half, -half, -half);
	pushPos(half, half, -half);
	pushPos(-half, -half, -half);
	pushPos(half, -half, -half);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushColor(r, g, b);
	pushNormal(0.f, 0.f, -1.f);
	pushNormal(0.f, 0.f, -1.f);
	pushNormal(0.f, 0.f, -1.f);
	pushNormal(0.f, 0.f, -1.f);
	pushNormal(0.f, 0.f, -1.f);
	pushNormal(0.f, 0.f, -1.f);
}

void Texture::setSrc(string src) {
	GLuint tmp;
	glGenTextures(1, &tmp);
	this->texName.push_back(tmp);

	this->src = bmp(src);
	bumpName.push_back(0);

	glBindTexture(GL_TEXTURE_2D, this->texName[this->texName.size() - 1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->src.sizeX, this->src.sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, this->src.data);
	
	offset.push_back(pair<int, int>(this->pos.size() / 3, this->texName.size() - 1));
}
void Texture::addBall(float radius, float slice) {
	if (slice < 1.f)return;

	float angleSpan = 45.f / slice;
	vector<float>tmp;
	for (float i = -90; i <= 90; i += angleSpan) {
		for (float j = 0; j < 360; j += angleSpan) {
			float r = radius * glm::cos(glm::radians(i));
			float x = r * glm::cos(glm::radians(j));
			float y = radius * glm::sin(glm::radians(i));
			float z = r * glm::sin(glm::radians(j));
			tmp.push_back(x);
			tmp.push_back(y);
			tmp.push_back(z);
		}
	}

	int row = int(180 / angleSpan) + 1;
	int col = int(360 / angleSpan);
	int k = col * (row - 2) * 6 * 8;
	int count = 0;
	for (int i = 0; i < row; i++) {
		if (i != 0 && i != row - 1) {
			for (int j = 0; j < col; j++) {
				k = i * col + j;
				glm::vec3 norm = glm::normalize(glm::vec3(tmp[(k + col) * 3], tmp[(k + col) * 3 + 1], tmp[(k + col) * 3 + 2]));
				pushPos(tmp[(k + col) * 3], tmp[(k + col) * 3 + 1], tmp[(k + col) * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord(j*1.f / col, 1.f / row);

				int index = k + 1;
				if (j == col - 1)
					index -= col;
				norm = glm::normalize(glm::vec3(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]));
				pushPos(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord((j + 1)*1.f / col, i*1.f / row);

				norm = glm::normalize(glm::vec3(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]));
				pushPos(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord(j*1.f / col, i*1.f / row);
			}
			for (int j = 0; j < col; j++) {
				k = i * col + j;
				glm::vec3 norm = glm::normalize(glm::vec3(tmp[(k - col) * 3], tmp[(k - col) * 3 + 1], tmp[(k - col) * 3 + 2]));
				pushPos(tmp[(k - col) * 3], tmp[(k - col) * 3 + 1], tmp[(k - col) * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord(j*1.f / col, 1.f / row);

				int index = k - 1;
				if (j == 0)
					index += col;
				norm = glm::normalize(glm::vec3(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]));
				pushPos(tmp[index * 3], tmp[index * 3 + 1], tmp[index * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord((j - 1)*1.f / col, i*1.f / row);

				norm = glm::normalize(glm::vec3(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]));
				pushPos(tmp[k * 3], tmp[k * 3 + 1], tmp[k * 3 + 2]);
				pushNormal(norm.x, norm.y, norm.z);
				pushCoord(j*1.f / col, i*1.f / row);
			}
		}
	}
}
