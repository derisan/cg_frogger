// -----------------------------------
// main.cpp
// 2020. 11. 02
// Code by Kim
// -----------------------------------

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "game.h"

void DisplayFunc();
void ReshapeFunc(int w, int h);
void KeyboardFunc(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);
void TimerFunc(int value);

void Shutdown();

Game game{};

int main(int argc, char** argv)
{
	if (!game.Init(&argc, argv))
	{
		std::cout << "Failed to initialize game" << std::endl;
		return -1;
	}

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutTimerFunc(16, TimerFunc, 1);

	glutMainLoop();

	return 0;
}

void DisplayFunc()
{
	game.Draw();
}

void ReshapeFunc(int w, int h)
{
	glViewport(0, 0, w, h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	game.ProcessKeyboardInput(key);
}

void MouseFunc(int button, int state, int x, int y)
{
	game.ProcessMouseInput(button, state, x, y);
}

void TimerFunc(int value)
{
	// Update is called per 16 ms
	glutTimerFunc(16, TimerFunc, 1);
	game.Update();
	if (game.GetShouldCloseWindow())
		Shutdown();
	glutPostRedisplay();
}

void Shutdown()
{
	game.Shutdown();
	glutLeaveMainLoop();
}