﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class GAMEASSETCLEANER_API SGameAssetCleanerViewer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameAssetCleanerViewer)
		{
		}

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
};
