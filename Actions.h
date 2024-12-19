#ifndef ACTIONS_H
#define ACTIONS_H

class IFlyAction {
public:
	virtual void fly() = 0;
};
class IJumpAction {
public:
	virtual void jump() = 0;
};
class IRunAction {
public:
	virtual void run() = 0;
};
class IWalkAction {
public:
	virtual void walk() = 0;
};

#endif