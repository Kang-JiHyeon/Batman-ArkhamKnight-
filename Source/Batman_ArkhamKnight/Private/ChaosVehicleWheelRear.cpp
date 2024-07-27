// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosVehicleWheelRear.h"

UChaosVehicleWheelRear::UChaosVehicleWheelRear()
{
	WheelRadius = 40.f;
	WheelWidth = 40.f;
	FrictionForceMultiplier = 4.f;
	bAffectedByBrake = true;
	bAffectedByHandbrake = true;
	SlipThreshold = 100.f;
	SkidThreshold = 100.f;

	MaxSteerAngle = 20.f;
	bAffectedBySteering = false;

	MaxBrakeTorque = 1500.f;
	MaxHandBrakeTorque = 6000.f;
}