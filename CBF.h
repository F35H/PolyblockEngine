#ifndef  CBF_H
#define CBF_H
#include "errorhandling.h"


namespace InitCBF{
extern void (*display)();

void Set();
void Display();	};
namespace MainMenCBF{
struct subwin{
	short win1, win2,
	win3, win4, win5	};

void Init();
void resize();
void mouse();
};
namespace GameCBF{
};
namespace GenCBF{
};
#endif
