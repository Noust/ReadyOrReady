#pragma once
#include "include.h"


void SimulateMouseMove(int dx, int dy) {
    const int steps = 15;

    float stepX = (float)dx / steps;
    float stepY = (float)dy / steps;

    float accX = 0.0f;
    float accY = 0.0f;

    for (int i = 0; i < steps; i++) {
        accX += stepX;
        accY += stepY;

        int moveX = (int)accX;
        int moveY = (int)accY;

        if (moveX != 0 || moveY != 0) {
            mouse_event(MOUSEEVENTF_MOVE, moveX, moveY, 0, 0);
            accX -= moveX;
            accY -= moveY;
        }

        Sleep(1);
    }
}

void MoveToTarget(int targetX, int targetY) {
    fvector2d center{ widthscreen / 2.0f, heightscreen / 2.0f };

    fvector2d targetscreen = {
        targetX - center.x,
        targetY - center.y
	};

    const int maxMove = 100;
    targetscreen.x = (targetscreen.x > maxMove) ? maxMove : (targetscreen.x < -maxMove) ? -maxMove : targetscreen.x;
    targetscreen.y = (targetscreen.y > maxMove) ? maxMove : (targetscreen.y < -maxMove) ? -maxMove : targetscreen.y;

    SimulateMouseMove(targetscreen.x, targetscreen.y);
}