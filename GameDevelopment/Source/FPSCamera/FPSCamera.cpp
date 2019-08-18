#include "FPSCamera.h"

#include "Math/BsVector3.h"
#include "Scene/BsSceneObject.h"
#include "Physics/BsPhysics.h"

namespace gameDevelopnemt
{
	constexpr float ROTATION_SPEED = 3.0f;
	constexpr bs::Degree PITCH_RANGE = bs::Degree(45.0f);

	FPSCamera::FPSCamera(const bs::HSceneObject& parent) : Component(parent)
	{
		setName("FPSCamera");

		inputHorizontalAxis = bs::VirtualAxis("Horizontal");
		inputVerticalAxis = bs::VirtualAxis("Vertical");

		auto rotation = SO()->getTransform().getRotation();
		bs::Radian x_angle, y_angle, roll;
		(void)rotation.toEulerAngles(x_angle, y_angle, roll);

		rotationVerticalAxis = x_angle;
		rotationHorizontalAxis = y_angle;

		applyAngles();
	}


	void FPSCamera::update()
	{
		rotationHorizontalAxis += bs::Degree(bs::gVirtualInput().getAxisValue(inputHorizontalAxis) * ROTATION_SPEED);
		rotationVerticalAxis += bs::Degree(bs::gVirtualInput().getAxisValue(inputVerticalAxis) * ROTATION_SPEED);
		applyAngles();
	}


	void FPSCamera::applyAngles()
	{
		rotationHorizontalAxis.wrap();
		rotationVerticalAxis.wrap();

		const auto pitchMax = PITCH_RANGE;
		const auto pitchMin = bs::Degree(360.0f) - PITCH_RANGE;

		if (rotationVerticalAxis > pitchMax && rotationVerticalAxis < pitchMin)
		{
			if ((rotationVerticalAxis - pitchMax) > (pitchMin - rotationVerticalAxis))
			{
				rotationVerticalAxis = pitchMin;
			}
			else
			{
				rotationVerticalAxis = pitchMax;
			}
		}

		bs::Quaternion yRot(bs::Vector3::UNIT_Y, bs::Radian(rotationHorizontalAxis));
		bs::Quaternion xRot(bs::Vector3::UNIT_X, bs::Radian(rotationVerticalAxis));

		if (!sceneObject())
		{
			auto camRot = yRot * xRot;
			camRot.normalize();

			SO()->setRotation(camRot);
		}
		else
		{
			sceneObject()->setRotation(yRot);
			SO()->setRotation(xRot);
		}
	}
};
