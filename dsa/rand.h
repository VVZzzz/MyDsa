#ifndef RAND_H_
#define RAND_H_
#pragma once
#include <stdlib.h>
#include <time.h>
//在[0,range)中随机生成一个数,采用当前时间作为种子
static int dice(int range) { return rand() % range; }
static int dice(int lo, int hi) { return lo + rand() % (hi - lo); }
static float dice(float range) { return rand() % (1000 * (int)range) / (float)1000.0; }
static float dice(double range) { return rand() % (1000 * (int)range) / (double)1000.0; }
static char dice(char range) { return (char)(32 + rand() % 96); }
#endif
