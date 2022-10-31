/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:25:28 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/31 13:50:46 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
#define SPHERE_H

#include <GLES3/gl3.h>
#include <Base/utility.h>
#include <vector>

/// Sphere 클래스.
class Sphere {
public:
    /// 생성자.
    /// \param radius 구의 반지름입니다.
    /// \param sectorCount 구의 전체 sector의 수입니다.
    /// \param stackCount 구의 전체 stack의 수입니다.
    /// \param smooth smooth shading 여부입니다.
    explicit Sphere(float radius = 1.0f, int sectorCount = 36,
                    int stackCount = 18, bool smooth = true);
    
    /// 소멸자.
    ~Sphere();

    /// 인덱스 배열의 전체 원소 개수를 반환하는 함수입니다.
    /// \return 인덱스 배열의 원소 개수입니다.
    unsigned int getIndexCount() const {
        return (unsigned int)indices_.size();
    }

    /// 인덱스 배열의 전체 크기를 반환하는 함수입니다.
    /// \return 인덱스 배열의 모든 윈소들의 크기입니다.
    unsigned int getIndexSize() const {
        return (unsigned int)indices_.size() * sizeof(unsigned int);
    }

    /// 인덱스 베열의 주소를 반환하는 함수입니다.
    /// \return 인덱스 배열의 주소입니다. 
    const unsigned int *getIndices() const {
        return indices_.data();
    }

    /// interleavedVertices_ 배열의 전체 원소 개수를 반환하는 함수입니다.
    /// \return 맴버변수 interleavedVertices_의 모든 원소들의 개수입니다. 
    unsigned int getInterleavedVertexCount() const  {
        return (unsigned int)interleavedVertices_.size();
    }

    /// interleavedVertices_ 배열의 전체 크기를 반환하는 함수입니다.
    /// \return 맴버변수 interleavedVertices_의 모든 원소들의 크기입니다. 
    unsigned int getInterleavedVertexSize() const {
        return (unsigned int)interleavedVertices_.size() * sizeof(float);
    }

    /// interleavedVertices_ 베열의 주소를 반환하는 함수입니다.
    /// \return interleavedVertices_ 배열의 주소입니다. 
    const float *getInterleavedVertices() const {
        return interleavedVertices_.data();
    }

    /// interleavedVertices_ 배열의 stride를 반환하는 함수입니다.
    /// \return interleavedStride_ 를 반환합니다.
    int getInterleavedStride() const {
        return interleavedStride_;
    }
    
    /// 구를 렌더링합니다.
    /// \param programId 사용할 셰이더의 네임입니다.
    void draw(GLuint programId) const;

private:
    /// smooth shading으로 구의 버텍스를 생성합니다.
    void buildVerticesSmooth();

    /// flat shading으로 구의 버텍스를 생성합니다.
	void buildVerticesFlat();


    /// 각 버텍스의 속성을 interleavedVertices_ 멤버변수에 
    /// 버텍스 좌표, 법선 벡터 좌표, 텍스처 좌표순으로 추가합니다. 
    void buildInterleavedVertices();

    /// vertices_, normals_, texCoords_, indices_ 의 capacity를
    /// 0으로 만듭니다.
    void clearArrays();

    /// vertices_ 맴버변수에 버텍스를 추가합니다.
    /// \param x 버텍스의 x좌표입니다.
    /// \param y 버텍스의 y좌표입니다.
    /// \param z 버텍스의 z좌표입니다.
    void addVertex(float x, float y, float z);

    /// normals_ 맴버변수에 법선벡터를 추가합니다.
    /// \param nx 법선벡터의 x좌표입니다.
    /// \param ny 법선벡터의 y좌표입니다.
    /// \param nz 법선벡터의 z좌표입니다.
    void addNormal(float nx, float ny, float nz);

    /// texCoords_ 맴버변수에 텍스처 좌표를 추가합니다.
    /// \param s 텍스처 좌표의 s좌표입니다.
    /// \param t 텍스처 좌표의 t좌표입니다.
    void addTexCoord(float s, float t);

    /// indices_ 맴버변수에 삼각형을 구성하는 인덱스를 추가합니다.
    /// \param i1 삼각형을 구성하는 버텍스의 vertices_ 배열 상의 인덱스입니다.
    /// \param i2 삼각형을 구성하는 버텍스의 vertices_ 배열 상의 인덱스입니다.
    /// \param i3 삼각형을 구성하는 버텍스의 vertices_ 배열 상의 인덱스입니다.
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

	std::vector<float> computeFaceNormal(float x1, float y1, float z1,
										 float x2, float y2, float z2,
										 float x3, float y3, float z3);

private:
    float radius_;
    int sectorCount_;
    int stackCount_;
    bool smooth_;

    std::vector<float> vertices_;
    std::vector<float> normals_;
    std::vector<float> texCoords_;
    std::vector<unsigned int> indices_;

    std::vector<float> interleavedVertices_;
    int interleavedStride_;

    GLuint vertex_array_;
    GLuint vertex_buffer_;
    GLuint index_buffer_;
};

#endif ///SPHERE_H 
