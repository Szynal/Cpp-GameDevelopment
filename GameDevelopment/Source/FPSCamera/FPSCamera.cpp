#include "FPSCamera.h"

#include "Math/BsVector3.h"
#include "Scene/BsSceneObject.h"
#include "Physics/BsPhysics.h"
#include "Components/BsCCharacterController.h"
#include "BsApplication.h"

namespace gameDevelopnemt
{
	constexpr float ROTATION_SPEED = 3.0f;
	constexpr bs::Degree PITCH_RANGE = bs::Degree(45.0f);

	constexpr float START_SPEED = 4.0f;
	constexpr float TOP_SPEED = 7.0f;
	constexpr float ACCELERATION = 1.5f;

	FPSCamera::FPSCamera(const bs::HSceneObject& parent) : Component(parent)
	{
		setName("FPSCamera");
		controller = SO()->getComponent<bs::CCharacterController>();
	
		auto rotation = SO()->getTransform().getRotation();
		bs::Radian x_angle, y_angle, roll;
		(void)rotation.toEulerAngles(x_angle, y_angle, roll);

		rotationVerticalAxis = x_angle;
		rotationHorizontalAxis = y_angle;

		inputHorizontalAxis = bs::VirtualAxis("Horizontal");
		inputVerticalAxis = bs::VirtualAxis("Vertical");
		moveForward = bs::VirtualButton("Forward");
		moveBack = bs::VirtualButton("Back");
		moveLeft = bs::VirtualButton("Left");
		moveRight = bs::VirtualButton("Right");
		applyAngles();
	}


	void FPSCamera::update()
	{
		rotationHorizontalAxis += bs::Degree(bs::gVirtualInput().getAxisValue(inputHorizontalAxis) * ROTATION_SPEED);
		rotationVerticalAxis += bs::Degree(bs::gVirtualInput().getAxisValue(inputVerticalAxis) * ROTATION_SPEED);
		bs::gDebug().logDebug(bs::toString(1 / bs::gTime().getFrameDelta()));
		applyAngles();
	}

	void FPSCamera::fixedUpdate()
	{
		auto goingForward = bs::gVirtualInput().isButtonHeld(moveForward);
		auto goingBack = bs::gVirtualInput().isButtonHeld(moveBack);
		auto goingLeft = bs::gVirtualInput().isButtonHeld(moveLeft);
		auto goingRight = bs::gVirtualInput().isButtonHeld(moveRight);
		const auto& tfrm = SO()->getTransform();

		bs::Vector3 direction = bs::Vector3::ZERO;
		if (goingForward) direction += tfrm.getForward();
		if (goingBack) direction -= tfrm.getForward();
		if (goingRight) direction += tfrm.getRight();
		if (goingLeft) direction -= tfrm.getRight();

		direction.y = 0.0f;
		direction.normalize();

		const float frameDelta = bs::gTime().getFrameDelta();

		if (direction.squaredLength() != 0)
		{
			direction.normalize();
			float multiplier = 1.0f;
	
			mCurrentSpeed = bs::Math::clamp(mCurrentSpeed + ACCELERATION * frameDelta, START_SPEED, TOP_SPEED);
			mCurrentSpeed *= multiplier;
		}
		else
		{
			mCurrentSpeed = 0.0f;
		}

		bs::Vector3 velocity(bs::BsZero);
		float tooSmall = std::numeric_limits<float>::epsilon();

		if (mCurrentSpeed > tooSmall)
		{
			velocity = direction * mCurrentSpeed;

		}
		
		bs::Vector3 gravity = bs::gPhysics().getGravity();
		controller->move((velocity + gravity) * frameDelta);
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
