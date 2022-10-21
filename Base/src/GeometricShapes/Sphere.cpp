/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sphere.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:35:45 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/21 20:03:00 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/GeometricShapes/Sphere.h"
#include <cmath>

Sphere::Sphere(float radius, int sectorCount, int stackCount, bool smooth)
    : radius_(radius),
      sectorCount_(sectorCount),
      stackCount_(stackCount),
      smooth_(smooth),
      interleavedStride_(32) {
    // smooth shading, flat shading 선택
    if (smooth_)
        buildVerticesSmooth();
    else
        buildVerticesFlat();

    // 버텍스 버퍼 설정
    GL_TEST(glGenBuffers(1, &vertex_buffer_));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_TEST(glBufferData(GL_ARRAY_BUFFER, this->getInterleavedVertexSize(), this->getInterleavedVertices(), GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, 0));

    // 인덱스 버퍼 설정
    GL_TEST(glGenBuffers(1, &index_buffer_));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
    GL_TEST(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    // 버텍스 어레이 설정
    GL_TEST(glGenVertexArrays(1, &vertex_array_));
    GL_TEST(glBindVertexArray(vertex_array_));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
    GL_TEST(glEnableVertexAttribArray(0));
    GL_TEST(glEnableVertexAttribArray(1));
    GL_TEST(glEnableVertexAttribArray(2));

    int stride = this->getInterleavedStride();
    GL_TEST(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0));
    GL_TEST(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3)));
    GL_TEST(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6)));
    GL_TEST(glBindVertexArray(0));
}

Sphere::~Sphere() {}

void Sphere::draw(GLuint programId) const {
    GL_TEST(glBindVertexArray(vertex_array_));
    GL_TEST(glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, 0));

    GL_TEST(glBindVertexArray(0));
}

// 자세한 설명은 http://www.songho.ca/opengl/gl_sphere.html 를 참고
void Sphere::buildVerticesSmooth() {
    // 아크코사인으로 원주율을 구합니다.
    const float PI = acos(-1);

    // 배열의 메모리 초기화합니다.
    clearArrays();

    float x, y, z, xy;                                  // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius_;       // normal
    float s, t;                                         // texCoord
                                                        
    // 360도(2PI)를 sectorCount_로 나눠서 sectorStep 1개의 각도를 구합니다.
    float sectorStep = 2 * PI / sectorCount_;
    // 180도(PI)를 stackCount_로 나눠서 stackStep 1개의 각도를 구합니다.
    float stackStep = PI / stackCount_;
    float sectorAngle, stackAngle;

    // -90 <= stackAngle <= 90 범위에서 구 버텍스의 z좌표를 구합니다.
    for (int i = 0; i <= stackCount_; i++) {
        stackAngle = PI / 2 - i * stackStep;

        // 구의 stackAngle 상의 점에서 xy평면으로 투영한 값입니다.
        xy = radius_ * cosf(stackAngle);                
        // 구의 stackAngle 상의 점의 z좌표입니다.
        z = radius_ * sinf(stackAngle);

        // 0 <= sectorAngle <= 360 범위에서 구 버텍스의 x, y 좌표를 구합니다.
        for (int j = 0; j <= sectorCount_; j++) {
            sectorAngle = j * sectorStep;

            // 구의 stackAngle, sectorAngle 상의 x좌표입니다.
            x = xy * cosf(sectorAngle);
            // 구의 stackAngle, sectorAngle 상의 y좌표입니다.
            y = xy * sinf(sectorAngle);
            // 버텍스 좌표를 추가합니다.
            addVertex(x, y, z);

            // 버텍스 좌표를 정규화하여 법선 벡터를 구합니다.
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            // 버텍스의 법선 벡터 좌표를 추가합니다.
            addNormal(nx, ny, nz);

            s = (float)j / sectorCount_;
            t = (float)i / stackCount_;
            // 텍스처 좌표를 추가합니다.
            addTexCoord(s, t);
        }
    }

    // 인덱스의 구성
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    unsigned int k1, k2;   // 삼각형을 구성하는 버텍스의 인덱스를 담는 변수

    for (int i = 0; i < stackCount_; i++) {
        k1 = i * (sectorCount_ + 1);
        k2 = k1 + sectorCount_ + 1;

        for (int j = 0; j < sectorCount_; j++, k1++, k2++) {
            // 첫 번째 stack 의 sector들은 삼각형이 1개이므로 
            if (i != 0)
                addIndices(k1, k2, k1 + 1);
            // 마지막 stack 의 sector들은 삼각형이 1개이므로
            if (i != (stackCount_ - 1))
                addIndices(k1 + 1, k2, k2 + 1);
        }
    }
    buildInterleavedVertices();
}

void Sphere::buildVerticesFlat() {
	const float PI = acos(-1);

	struct Vertex {
		float x, y, z, s, t;
	};
	std::vector<Vertex> tmpVertices;

	float sectorStep = 2 * PI / sectorCount_;
	float stackStep = PI / stackCount_;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount_; i++) {
		stackAngle = PI / 2 - i * stackStep;
		float xy = radius_ * cosf(stackAngle);
		float z = radius_ * sinf(stackAngle);

		for (int j = 0; j <= sectorCount_; j++) {
			sectorAngle = j * sectorStep;

			Vertex vertex;
			vertex.x = xy * cosf(sectorAngle);
			vertex.y = xy * sinf(sectorAngle);
			vertex.z = z;
			vertex.s = (float)j / sectorCount_;
			vertex.t = (float)i / stackCount_;
			tmpVertices.push_back(vertex);
		}
	}

	clearArrays();

	Vertex v1, v2, v3, v4;
	std::vector<float> n;

	int i, j, k, vi1, vi2;
	int index = 0;
	for (i = 0; i < stackCount_; i++) {
		vi1 = i * (sectorCount_ + 1);
		vi2 = (i + 1) * (sectorCount_ + 1);

		for (j = 0; j < sectorCount_; j++, vi1++, vi2++) {
			v1 = tmpVertices[vi1];
			v2 = tmpVertices[vi2];
			v3 = tmpVertices[vi1 + 1];
			v3 = tmpVertices[vi2 + 1];

			if (i == 0) {
				addVertex(v1.x, v1.y, v1.z);
				addVertex(v2.x, v2.y, v2.z);
				addVertex(v4.x, v4.y, v4.z);

				addTexCoord(v1.s, v1.t);
				addTexCoord(v2.s, v2.t);
				addTexCoord(v4.s, v4.t);

				n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);

				for (k = 0; k < 3; k++) {
					addNormal(n[0], n[1], n[2]);
				}
				
				addIndices(index, index + 1, index + 2);

				index += 3;
			}
			else if (i == (stackCount_ - 1)) {
				addVertex(v1.x, v1.y, v1.z);
				addVertex(v2.x, v2.y, v2.z);
				addVertex(v3.x, v3.y, v3.z);

				addTexCoord(v1.s, v1.t);
				addTexCoord(v2.s, v2.t);
				addTexCoord(v3.s, v3.t);

				n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);

				for (k = 0; k < 3; k++) {
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 1, index + 2);

				index += 3;
			}
			else {
				addVertex(v1.x, v1.y, v1.z);
				addVertex(v2.x, v2.y, v2.z);
				addVertex(v3.x, v3.y, v3.z);
				addVertex(v4.x, v4.y, v4.z);

				addTexCoord(v1.s, v1.t);
				addTexCoord(v2.s, v2.t);
				addTexCoord(v3.s, v3.t);
				addTexCoord(v4.s, v4.t);

				n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);

				for (k = 0; k < 4; k++) {
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 1, index + 2);
				addIndices(index + 2, index + 1, index + 3);

				index += 4;
			}
		}
	}
    // interleavedVertices_에 버텍스 속성들을 일렬로 추가합니다.
	buildInterleavedVertices();
}

void Sphere::buildInterleavedVertices() {
    // interleavedVertices_ 의 메모리를 초기화합니다.
    std::vector<float>().swap(interleavedVertices_);

    std::size_t i, j;
    std::size_t count = vertices_.size();

    // interleavedVertices_ 맴버변수에 버텍스 좌표, 법선벡터, 텍스처 좌표 순으로
    // 추가합니다.
    for (int i = 0, j = 0; i < count; i += 3, j += 2) {
        interleavedVertices_.push_back(vertices_[i]);
        interleavedVertices_.push_back(vertices_[i + 1]);
        interleavedVertices_.push_back(vertices_[i + 2]);

        interleavedVertices_.push_back(normals_[i]);
        interleavedVertices_.push_back(normals_[i + 1]);
        interleavedVertices_.push_back(normals_[i + 2]);

        interleavedVertices_.push_back(texCoords_[j]);
        interleavedVertices_.push_back(texCoords_[j + 1]);
    }
}

void Sphere::clearArrays() {
    std::vector<float>().swap(vertices_);
    std::vector<float>().swap(normals_);
    std::vector<float>().swap(texCoords_);
    std::vector<unsigned int>().swap(indices_);
}

void Sphere::addVertex(float x, float y, float z) {
    vertices_.push_back(x);
    vertices_.push_back(y);
    vertices_.push_back(z);
}

void Sphere::addNormal(float nx, float ny, float nz) {
    normals_.push_back(nx);
    normals_.push_back(ny);
    normals_.push_back(nz);
}

void Sphere::addTexCoord(float s, float t) {
    texCoords_.push_back(s);
    texCoords_.push_back(t);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
    indices_.push_back(i1);
    indices_.push_back(i2);
    indices_.push_back(i3);
}

std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,
											 float x2, float y2, float z2,
											 float x3, float y3, float z3) {
	const float EPSILON = 0.000001f;

	std::vector<float> normal(3, 0.0f);
	float nx, ny, nz;

	float ex1 = x2 - x1;
	float ey1 = y2 - y1;
	float ez1 = z2 - z1;
	float ex2 = x3 - x1;
	float ey2 = y3 - y1;
	float ez2 = z3 - z1;
	
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	float length = sqrtf(nx * nx + ny * ny + nz * nz);

	if (length > EPSILON) {
		float lengthInv = 1.0f / length;
		normal[0] = nx * lengthInv;
		normal[1] = nx * lengthInv;
		normal[2] = nx * lengthInv;
	}

	return normal;
}

