#include "Control.h"

bool Control::PressButton() const {
	return _kbhit();
}

int Control::GetInput() const {
	return static_cast<int>(_getch());
}

bool Control::isUp(const int& press) const {
	return press == 119;
}

bool Control::isDown(const int& press) const {
	return press == 115;
}

bool Control::isLeft(const int& press) const {
	return press == 97;
}

bool Control::isRight(const int& press) const {
	return press == 100;
}

bool Control::isP(const int& press) const {
	return press == 112;
}
bool Control::isI(const int& press) const {
	return press == 105;
}
bool Control::isBackspace(const int& press) const {
	return press == 8;
}
bool Control::isEnter(const int& press) const {
	return press == 13;
}
bool Control::isOne(const int& press) const {
	return press == 49;
}
bool Control::isTwo(const int& press) const {
	return press == 50;
}
bool Control::isThree(const int& press) const {
	return press == 51;
}

/*
119 up
97 left
115 down
100 right
112 P
105 I
8 Backspace
13 Enter
*/