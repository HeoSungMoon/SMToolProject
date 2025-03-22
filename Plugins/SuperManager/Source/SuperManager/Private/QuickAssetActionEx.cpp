// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAssetActionEx.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "Algo/ForEach.h"
#include "ObjectTools.h"

void UQuickAssetActionEx::DuplicateAssets(int32 NumOfDuplicates)
{
	if (NumOfDuplicates <= 0)
	{
		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter an invalid number of duplicates!"));
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;
	for (const FAssetData& SelectedAsset : SelectedAssetsData)
	{
		for (int32 i = 0; i < NumOfDuplicates; i++)
		{
			const FString SourceAssetPath = SelectedAsset.GetSoftObjectPath().ToString(); // GetPathName()
			const FString NewDuplicateAssetName = SelectedAsset.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);
			const FString NewPathName = FPaths::Combine(SelectedAsset.PackagePath.ToString(), NewDuplicateAssetName);
			
			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{
				UEditorAssetLibrary::SaveAsset(SourceAssetPath, false);
				++Counter;
			}
		}
	}

	if (Counter > 0)
	{
		//Print(TEXT("Successfully Duplicated Assets" + FString::FromInt(Counter) + "files"), FColor::Red);
		ShowNotifyInfo(TEXT("Successfully Duplicated Assets " + FString::FromInt(Counter) + " files"));
	}
}

void UQuickAssetActionEx::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (!SelectedObject) continue;

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());
		if (!PrefixFound || PrefixFound->IsEmpty())
		{
			Print(TEXT("Failed to find prefix for class ") + SelectedObject->GetClass()->GetName(), FColor::Red);
			continue;
		}

		FString OldName = SelectedObject->GetName();
		if (OldName.StartsWith(*PrefixFound))
		{
			Print(OldName + TEXT(" already has prefix added"), FColor::Red);
			continue;
		}
		
		Algo::ForEachIf(PrefixMap, [&OldName](const TTuple<UClass*, FString>& Value)
		{
			return OldName.StartsWith(Value.Get<1>());
		},
		[&OldName](const TTuple<UClass*, FString>& Value)
		{
			OldName.RemoveFromStart(Value.Get<1>());
		});

		if (SelectedObject->IsA(UMaterialInstanceConstant::StaticClass()))
		{
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName;
		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix);
		++Counter;
	}

	if (Counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}

void UQuickAssetActionEx::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;
	for (const FAssetData& SelectedAsset : SelectedAssetsData)
	{
		TArray<FString> AssetReference = UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAsset.ToSoftObjectPath().ToString(), true);
		if (AssetReference.Num() == 0)
		{
			UnusedAssetsData.Add(SelectedAsset);
		}
	}

	if (UnusedAssetsData.Num() == 0)
	{
		ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found among selected assets"), false);
		return;
	}

	//UEditorAssetLibrary::DeleteAsset()
	int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData);
	if (NumOfAssetsDeleted == 0) return;

	ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + TEXT("unused Assets")));
}
