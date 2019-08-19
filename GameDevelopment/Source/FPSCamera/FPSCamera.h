#pragma once

#include "Scene/BsComponent.h"
#include "Math/BsDegree.h"
#include "Input/BsVirtualInput.h"

namespace gameDevelopnemt
{
	class FPSCamera : public bs::Component
	{
	public:

		FPSCamera(const bs::HSceneObject& parent);

		void update() override;
		void fixedUpdate() override;
		void SetSceneObject(const bs::HSceneObject& sceneObject) { player = sceneObject; }

	private:

		void applyAngles();

		bs::HSceneObject player;
		bs::HCharacterController controller;

		bs::Degree rotationVerticalAxis = bs::Degree(0.0f);
		bs::Degree rotationHorizontalAxis = bs::Degree(0.0f);


		float mCurrentSpeed = 0.0f; 
		bs::VirtualAxis inputVerticalAxis;
		bs::VirtualAxis inputHorizontalAxis;
		bs::VirtualButton moveForward; 
		bs::VirtualButton moveBack; 
		bs::VirtualButton moveLeft; 
		bs::VirtualButton moveRight;
	};

	using HFPSCamera = bs::GameObjectHandle<FPSCamera>;
};