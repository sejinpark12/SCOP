/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uniforms.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:17:32 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/25 15:32:09 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glm/glm.hpp>

enum lightType {
    DIRECT_GOURAUD,
    DIRECT_PHONG,
    POINT,
    SPOTLIGHT
};

struct Uniforms {
    glm::vec4 backGroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    glm::vec4 diffuse = glm::vec4(0.47f, 0.30f, 0.64f, 0.0f);
    float ambient = 0.25f;
    float shininess = 80.0f;
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    glm::vec4 lightDir = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    glm::vec4 lightPos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 spotDir = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
    float spotCutOffAngle = 30.0f;
    int selectLight = DIRECT_PHONG;
};

#endif ///UNIFORMS_H
