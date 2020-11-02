#pragma once
class Component
{
public:
	Component(class Object* owner);
	virtual ~Component();

	virtual void Update() { };
	virtual void ProcessKeyboardInput(unsigned char key) { };
	
protected:
	class Object* mOwner;
};

