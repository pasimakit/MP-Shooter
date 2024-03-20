// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/Character/BSaveGame.h"

UBlasterGameInstance::UBlasterGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}

void UBlasterGameInstance::LoadGame()
{
    // Try to load a saved game file (with name: <SaveGameSlotName>.sav) if exists
    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
    SaveGameObject = Cast<UBSaveGame>(LoadedGame);

    // If file does not exist try create a new one
    if (!SaveGameObject)
    {

        // Instantiate a new SaveGame object
        SaveGameObject = Cast<UBSaveGame>(UGameplayStatics::CreateSaveGameObject(UBSaveGame::StaticClass()));

        // Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
        const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);
    }
}

void UBlasterGameInstance::SaveGame()
{

    // Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
    const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);
}
