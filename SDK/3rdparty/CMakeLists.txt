# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/27 18:26:55 by sejpark           #+#    #+#              #
#    Updated: 2022/10/27 18:27:02 by sejpark          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

add_library(ImGuiFileDialog
    ImGuiFileDialog/ImGuiFileDialog.h
    ImGuiFileDialog/ImGuiFileDialog.cpp)

target_include_directories(ImGuiFileDialog
    PUBLIC ImGuiFileDialog)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(ImGuiFileDialog
    PUBLIC imgui::imgui)
