/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:06:50 by sejpark           #+#    #+#             */
/*   Updated: 2022/09/02 15:52:42 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_H
#define MESH_H

#include <GLES3/gl3.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <Base/Shader.h>
#include <Base/utility.h>

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
    GLuint vertex_array_;
    GLuint vertex_buffer_;
    GLuint index_buffer_;

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

    void clearMesh();

private:
    // initializes all the buffer objects/arrays
    void setupMesh();
};

#endif //MESH_H
