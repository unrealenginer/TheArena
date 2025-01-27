// Fill out your copyright notice in the Description page of Project Settings.

#include "TheArena.h"
#include "ArenaStartMenu.h"
#include "ArenaGameInstance.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "GenericPlatformChunkInstall.h"
#include "Online/ArenaOnlineGameSettings.h"

#define LOCTEXT_NAMESPACE "TheArena.UMG.Menu"

static const FString MapNames[] = { TEXT("StagingArea") };

UArenaStartMenu::~UArenaStartMenu()
{
	auto Sessions = Online::GetSessionInterface();
	CleanupOnlinePrivilegeTask();
}

void UArenaStartMenu::SetUp(UArenaGameInstance* _GameInstance, ULocalPlayer* _PlayerOwner)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Initiate Set Up");
	bIsLanMatch = false;
	GameInstance = _GameInstance;
	PlayerOwner = _PlayerOwner;
}

void UArenaStartMenu::HostGame(const FString& GameType)
{
	if (ensure(GameInstance.IsValid()) && GetPlayerOwner() != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Instance and Owner Valid");
		FString const StartURL = FString::Printf(TEXT("/Game/Maps/%s?game=%s%s%s?%s=%d"), TEXT("StagingArea"), *GameType, GameInstance->GetIsOnline() ? TEXT("?listen") : TEXT(""), bIsLanMatch ? TEXT("?bIsLanMatch") : TEXT(""), TEXT("Bots"), 0);

		GameInstance->HostGame(GetPlayerOwner(), GameType, StartURL);
	}
}

void UArenaStartMenu::HostTeamDeathMatch()
{
	HostGame(LOCTEXT("TDM", "TDM").ToString());
}

ULocalPlayer* UArenaStartMenu::GetPlayerOwner() const
{
	return PlayerOwner.Get();
}

FString UArenaStartMenu::GetMapName() const
{
	return MapNames[0];
}

AArenaGameSession* UArenaStartMenu::GetGameSession() const
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AGameMode* const Game = World->GetAuthGameMode();
		if (Game)
		{
			return Cast<AArenaGameSession>(Game->GameSession);
		}
	}

	return nullptr;
}

void UArenaStartMenu::CleanupOnlinePrivilegeTask()
{
	if (GameInstance.IsValid())
	{
		GameInstance->CleanupOnlinePrivilegeTask();
	}
}

#undef LOCTEXT_NAMESPACE

