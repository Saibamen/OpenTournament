// Copyright 2019 Open Tournament Project, All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "UR_GameState.h"

#include "Engine/World.h"

#include "OpenTournament.h"

/////////////////////////////////////////////////////////////////////////////////////////////////


void AUR_GameState::OnRep_MatchState()
{
	OnMatchStateChanged.Broadcast();
	
	Super::OnRep_MatchState();
}