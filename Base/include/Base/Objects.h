/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:40:11 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/31 18:50:20 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
#define OBJECTS_H

#include "Model.h"
#include "GeometricShapes/Sphere.h"

struct Objects {
    std::vector<Model*> models;
    std::vector<Sphere*> spheres;
};

#endif //OBJECTS_H
