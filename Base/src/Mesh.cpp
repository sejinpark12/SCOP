/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 16:51:18 by sejpark           #+#    #+#             */
/*   Updated: 2022/09/02 15:55:43 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures)
    : vertex_array_(0), vertex_buffer_(0), index_buffer_(0),
      vertices_(vertices), indices_(indices), textures_(textures) {
    // now that we have all the required data, set the vertex buffers and its attributes pointers.
    setupMesh();
}

Mesh::~Mesh() {}

//void Mesh::Draw(Shader &shader) {
void Mesh::Draw(GLuint programId) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 1; i < textures_.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures_[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        //glUniform1f(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
        glUniform1f(glGetUniformLocation(programId, ("material." + name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    // draw mesh
    glBindVertexArray(vertex_array_);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    GL_TEST(glGenBuffers(1, &vertex_buffer_));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_TEST(glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_TEST(glGenBuffers(1, &index_buffer_));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
    GL_TEST(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    GL_TEST(glGenVertexArrays(1, &vertex_array_));
    GL_TEST(glBindVertexArray(vertex_array_));
    GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
    GL_TEST(glEnableVertexAttribArray(0));
    GL_TEST(glEnableVertexAttribArray(1));
    GL_TEST(glEnableVertexAttribArray(2));
    GL_TEST(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    GL_TEST(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
    GL_TEST(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
    GL_TEST(glBindVertexArray(0));
}

void Mesh::clearMesh() {
    GL_TEST(glDeleteVertexArrays(1, &vertex_array_));
    vertex_array_ = 0;
    GL_TEST(glDeleteBuffers(1, &vertex_buffer_));
    vertex_buffer_ = 0;
    GL_TEST(glDeleteBuffers(1, &index_buffer_));
    index_buffer_ = 0;
}
