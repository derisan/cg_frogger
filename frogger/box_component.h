#pragma once

#include "component.h"
#include "aabb.h"

class BoxComponent :
    public Component
{
public:
	BoxComponent(class Actor* owner);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	const AABB& GetWorldBox() const { return mWorldBox; }

	void SetObjectBox(const AABB& box) { mObjectBox = box; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
	AABB mObjectBox;
	AABB mWorldBox;
	bool mShouldRotate;
};