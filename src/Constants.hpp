#pragma once

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

const float PLAYER_START_POS_X = 100.f;

const float GROUND_HEIGHT = 100.f;

const float GRAVITY = 2000.f;
const float JUMP_FORCE = -700.f;
const float NORMAL_SPEED = 300.f;
const float MAX_SPEED = 600.f;
const float MIN_SPEED = 100.f;
const float ACCELERATION = 900.f;
const float DECELERATION = 600.f;
const float FRICTION = 100.f;

const float SMALL_CLOUD_SPEED_RATIO = 80.f / NORMAL_SPEED;
const float BIG_CLOUD_SPEED_RATIO = 20.f / NORMAL_SPEED;

const float WALL_HEIGHT = 100.f;
