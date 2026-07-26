// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FwidgetControllerParams& controllerparams)
{
	PlayerController = controllerparams.PlayerController;
	PlayerState = controllerparams.PlayerState;
	AbilitySystemComponent = controllerparams.AbilitySystemComponent;
	AttributeSet = controllerparams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
