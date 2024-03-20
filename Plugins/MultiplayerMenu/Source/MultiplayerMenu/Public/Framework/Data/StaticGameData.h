﻿#pragma once

#include "NativeGameplayTags.h"

// Asset Manager data types
#define S_DATA_ASSET_TYPE_GAMEDATA			FName(TEXT("GameData"))
#define S_DATA_ASSET_TYPE_GAMEDATA_DEFAULT	FName(TEXT("DA_GameData_Default"))

// Multiplayer Settings
#define S_MP_SETTINGS_GAMEMODE				TEXT("GAMEMODE")

// Asset bundles
#define S_DATA_ASSET_BUNDLE_GAME			FName(TEXT("Game"))

namespace UILayerTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_GAME);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_GAMEMENU);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_MENU);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_MODAL);
}