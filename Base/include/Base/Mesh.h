/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:06:50 by sejpark           #+#    #+#             */
/*   Updated: 2022/09/01 18:57:06 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_H
#define MESH_H

#include <GLES3/gl3.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Base/Shader.h"
#include <string>
#include <vector>

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
private:
	// render data
	unsigned int VAO_;
	unsigned int VBO_;
	unsigned int EBO_;

public:
	// mesh Data
	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	std::vector<Texture> textures_;

	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		 std::vector<Texture> textures);

	~Mesh();

	// render the mesh
	//void Draw(Shader &shader);
	void Draw(GLuint programId);

private:
	// initializes all the buffer objects/arrays
	void setupMesh();
};

#endif //MESH_H
