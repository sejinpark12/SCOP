/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sphere.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:35:45 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/19 19:36:18 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/GeometricShapes/Sphere.h"
#include <cmath>

Sphere::Sphere(float radius, int sectorCount, int stackCount, bool smooth)
    : radius_(radius),
      sectorCount_(sectorCount),
      stackCount_(stackCount),
      smooth_(smooth),
      interleavedVertices_(32) {
    if (smooth_)
        buildVerticesSmooth();
//    else
//        buildVerticesFlat();

    GL_TEST(glGenBuffers(1, &vertex_buffer_));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_TEST(glBufferData(GL_ARRAY_BUFFER, this->getInterleavedVertexSize(), this->getInterleavedVertices(), GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_TEST(glGenBuffers(1, &index_buffer_));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
    GL_TEST(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

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

void Sphere::draw() const {
    GL_TEST(glBindVertexArray(vertex_array_));
    GL_TEST(glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, 0));

    GL_TEST(glBindVertexArray(0));
}

void Sphere::buildVerticesSmooth() {
    const float PI = acos(-1);

    clearArrays();

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius_;
    float s, t;

    float sectorStep = 2 * PI / sectorCount_;
    float stackStep = PI / stackCount_;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount_; i++) {
        stackAngle = PI / 2 - i * stackStep;

        xy = radius_ * cosf(stackAngle);
        z = radius_ * sinf(stackAngle);

        for (int j = 0; j <= sectorCount_; j++) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            addVertex(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);

            s = (float)j / sectorCount_;
            t = (float)i / stackCount_;
            addTexCoord(s, t);
        }
    }

    unsigned int k1, k2;

    for (int i = 0; i < stackCount_; i++) {
        k1 = i * (sectorCount_ + 1);
        k2 = k1 + sectorCount_ + 1;

        for (int j = 0; j < sectorCount_; j++, k1++, k2++) {
            if (i != 0)
                addIndices(k1, k2, k1 + 1);
            if (i != (stackCount_ - 1))
                addIndices(k1 + 1, k2, k2 + 1);

            lineIndices_.push_back(k1);
            lineIndices_.push_back(k2);

            if (i != 0) {
                lineIndices_.push_back(k1);
                lineIndices_.push_back(k1 + 1);
            }
        }
    }

    buildInterleavedVertices();
}

void Sphere::buildInterleavedVertices() {
    std::vector<float>().swap(interleavedVertices_);

    std::size_t i, j;
    std::size_t count = vertices_.size();

    for (int i = 0, j = 0; i < count; i += 3, j += 2) {
        interleavedVertices_.push_back(vertices_[i]);
        interleavedVertices_.push_back(vertices_[i + 1]);
        interleavedVertices_.push_back(vertices_[i + 2]);

        interleavedVertices_.push_back(normals_[i]);
        interleavedVertices_.push_back(normals_[i + 1]);
        interleavedVertices_.push_back(normals_[i + 2]);

        interleavedVertices_.push_back(texCoords_[i]);
        interleavedVertices_.push_back(texCoords_[i + 1]);
    }
}

void Sphere::clearArrays() {
    std::vector<float>().swap(vertices_);
    std::vector<float>().swap(normals_);
    std::vector<float>().swap(texCoords_);
    std::vector<unsigned int>().swap(indices_);
    std::vector<unsigned int>().swap(lineIndices_);
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
