/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 18:17:23 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/30 20:21:40 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <Base/Mesh.h>
#include <Base/utility.h>

class Model {
private:
    std::vector<Mesh> meshes_;
    std::string directory_;
    std::vector<Texture> textures_loaded_;

public:
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path);

    ~Model();

    // draws the model, and thus all its meshes
    //void Draw(Shader &shader); 
    void Draw(GLuint programId); 

    void clearModel();
    const std::vector<Mesh> &getMeshes() const;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodese (if any).
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    unsigned int textureFromFile(const char *path, const std::string &directory);
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
            aiTextureType type, std::string typeName);
};

#endif //MODEL_H
