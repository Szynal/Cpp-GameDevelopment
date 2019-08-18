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
		void SetSceneObject(const bs::HSceneObject& sceneObject) { player = sceneObject; }

	private:

		void applyAngles();

		bs::HSceneObject player;

		bs::Degree rotationVerticalAxis = bs::Degree(0.0f);
		bs::Degree rotationHorizontalAxis = bs::Degree(0.0f);

		bs::VirtualAxis inputVerticalAxis;
		bs::VirtualAxis inputHorizontalAxis;
	};

	using HFPSCamera = bs::GameObjectHandle<FPSCamera>;
};
