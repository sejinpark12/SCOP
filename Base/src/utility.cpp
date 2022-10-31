//
// Created by Daemyung Jang on 2021/09/23.
// Updated by Sejin Park on 2022/08/31.
//

#include "Base/utility.h"
#include <fstream>

std::filesystem::path home() {
    return {HOME_PATH};
}

void millisecPerFrame(float currentTime, float &lastTime, int &framesNum, float &result) {
    framesNum++;

    if (currentTime - lastTime >= 1000.0f) {
        result = 1000.0f / float(framesNum);
        framesNum = 0;
        lastTime += 1000.0f;
    }
}
