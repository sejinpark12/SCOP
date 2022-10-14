/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:25:28 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/14 18:47:43 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
#define SPHERE_H

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

public:
    explicit Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18,
            bool smooth = true);
    
    ~Sphere();

    void draw() const;

private:
    void buildVerticesSmooth();
    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float nx, float ny, float nz);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

};

#endif ///SPHERE_H 
