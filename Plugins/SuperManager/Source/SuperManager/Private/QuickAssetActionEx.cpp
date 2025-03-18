// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAssetActionEx.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetActionEx::TestFunc()
{
	Print(TEXT("Working"), FColor::Cyan);
	PrintLog(TEXT("Working"));
}