#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "actions.h"
#include <iostream>

// Полёт
class FlyWithWings : public IFlyAction {
public:
	void fly() { std::cout << "I am flying!" << std::endl; }
};
class FlyWithoutWings : public IFlyAction {
public:
	void fly() { std::cout << "I can not fly..." << std::endl; }
};


// Прыжок
class JumpWithLegs : public IJumpAction {
public:
	void jump() { std::cout << "I am jumping!" << std::endl; }
};
class JumpWithoutLegs : public IJumpAction {
public:
	void jump() { std::cout << "I can not jump..." << std::endl; }
};


// Бег
class RunWithLegs : public IRunAction {
public:
	void run() { std::cout << "I am running!" << std::endl; }
};
class RunWithoutLegs : public IRunAction {
public:
	void run() { std::cout << "I can not run..." << std::endl; }
};

class WalkWithLegs : public IWalkAction {
public:
	void walk() { std::cout << "I am walking!" << std::endl; }
};
class WalkWithoutLegs : public IWalkAction {
public:
	void walk() { std::cout << "I can not walk..." << std::endl; }
};

#endif