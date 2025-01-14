// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosVehicleWheelFront.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
 */

UChaosVehicleWheelFront::UChaosVehicleWheelFront()
{	
	WheelRadius = 39.f;
	WheelWidth = 35.f;
	FrictionForceMultiplier = 7.f;
	bAffectedByBrake = true;
	bAffectedByEngine = true;
	
	bAffectedBySteering = true;

	MaxBrakeTorque = 6000.f;
	MaxHandBrakeTorque = 6000.f;
}