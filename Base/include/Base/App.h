/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:55:00 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/31 18:39:51 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include <vector>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "Ui.h"

struct App {
    EGLDisplay display{EGL_NO_DISPLAY};
    EGLConfig config{nullptr};
    EGLContext context{EGL_NO_CONTEXT};
    EGLSurface surface{EGL_NO_SURFACE};
    std::vector<GLuint> programs;
    GLint model_location{0};
    GLint view_location{0};
    GLint projection_location{0};
    Ui *ui{nullptr};
    Objects objects;
};

#endif ///APP_H
