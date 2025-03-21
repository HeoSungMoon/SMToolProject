// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "QuickAssetActionEx.generated.h"

/**
 * 
 */
UCLASS()
class UQuickAssetActionEx : public UAssetActionUtility
{ 
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void DuplicateAssets(int32 NumOfDuplicates);

	UFUNCTION(CallInEditor)
	void AddPrefixes();
	
private:
	TMap<UClass*, FString> PrefixMap =
	{
		{ UBlueprint::StaticClass(), TEXT("BP_")}
	};
};

