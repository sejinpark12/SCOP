/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:25:28 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/20 01:04:06 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
#define SPHERE_H

#include <GLES3/gl3.h>
#include <Base/utility.h>
#include <vector>

class Sphere {
private:
    float radius_;
    int sectorCount_;
    int stackCount_;
    bool smooth_;
    std::vector<float> vertices_;
    std::vector<float> normals_;
    std::vector<float> texCoords_;
    std::vector<unsigned int> indices_;
    std::vector<unsigned int> lineIndices_;

    std::vector<float> interleavedVertices_;
    int interleavedStride_;

    GLuint vertex_array_;
    GLuint vertex_buffer_;
    GLuint index_buffer_;

public:
    explicit Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18,
            bool smooth = true);
    
    ~Sphere();

    unsigned int getIndexCount() const {
        return (unsigned int)indices_.size();
    }
    unsigned int getIndexSize() const {
        return (unsigned int)indices_.size() * sizeof(unsigned int);
    }
    const unsigned int *getIndices() const {
        return indices_.data();
    }

    unsigned int getInterleavedVertexSize() const {
        return (unsigned int)interleavedVertices_.size() * sizeof(float);
    }
    const float *getInterleavedVertices() const {
        return interleavedVertices_.data();
    }
    int getInterleavedStride() const {
        return interleavedStride_;
    }

    void draw() const;

private:
    void buildVerticesSmooth();
	void buildVerticesFlat();
    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float nx, float ny, float nz);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	std::vector<float> computeFaceNormal(float x1, float y1, float z1,
										 float x2, float y2, float z2,
										 float x3, float y3, float z3);

};

#endif ///SPHERE_H 
