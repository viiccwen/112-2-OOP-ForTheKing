#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <conio.h>
#include <Windows.h>

class Control {
private:

public:
	bool PressButton() const;
	int GetInput() const;


	bool isUp(const int& press) const;
	bool isDown(const int& press) const;
	bool isLeft(const int& press) const;
	bool isRight(const int& press) const;
	bool isP(const int& press) const;
	bool isI(const int& press) const;
	bool isBackspace(const int& press) const;
	bool isEnter(const int& press) const;
};

#endif // !_CONTROL_H_