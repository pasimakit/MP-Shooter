// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/SPrimaryGameLayout.h"
#include "Framework/Data/StaticGameData.h"

void USPrimaryGameLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterLayer(UILayerTags::TAG_UI_LAYER_GAME, Game_Stack);
	RegisterLayer(UILayerTags::TAG_UI_LAYER_GAMEMENU, GameMenu_Stack);
	RegisterLayer(UILayerTags::TAG_UI_LAYER_MENU, Menu_Stack);
	RegisterLayer(UILayerTags::TAG_UI_LAYER_MODAL, Modal_Stack);
}
