// Copyright 2019 Open Tournament Project, All Rights Reserved.

#include "UR_Widget_ControlsListEntry.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

UUR_Widget_ControlsListEntry::UUR_Widget_ControlsListEntry(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	IsSelectingKey = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void UUR_Widget_ControlsListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	EntryKeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &UUR_Widget_ControlsListEntry::OnEntryKeySelectorIsSelectingKeyChanged);
	EntryKeySelector->OnKeySelected.AddDynamic(this, &UUR_Widget_ControlsListEntry::OnEntryKeySelectorKeyChanged);

	//LOCTABLE_FROMFILE_GAME("KeyBindingStringTable", "KeyBindingStringTable", "OpenTournament/StringTables/ST_KeyBindingNames.csv");
}

void UUR_Widget_ControlsListEntry::OnEntryKeySelectorKeyChanged(FInputChord SelectedKey)
{
	UE_LOG(LogTemp, Warning, TEXT("EntryKeySelector Key was Changed"));
	if (IsSelectingKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("EntryKeySelector Key Changed (keybind modify"));
		AUR_BasePlayerController * PlayerController = Cast<AUR_BasePlayerController>(GetOwningPlayer());
		UUR_PlayerInput * PlayerInput = Cast<UUR_PlayerInput>(PlayerController->GetPlayerInput());
		UUR_Object_KeyBind * KeyBind = Cast<UUR_Object_KeyBind>(Item);

		/*FInputActionKeyMapping KeyMapping(KeyBind->ActionName, SelectedKey.Key);

		PlayerInput->ModifyActionKeyMapping(KeyBind->ActionName, KeyMapping);
		KeyBind->Key = SelectedKey.Key;*/
		if (PlayerInput->ModifyKeyMapping(KeyBind->Name, SelectedKey))
		{
			KeyBind->Key = SelectedKey.Key;
			UpdateEntry();
		}

		IsSelectingKey = false;
	}
}

void UUR_Widget_ControlsListEntry::OnEntryKeySelectorIsSelectingKeyChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("EntryKeySelector IsSelectingKey Changed"));
	IsSelectingKey = true;
}

void UUR_Widget_ControlsListEntry::NativeOnListItemObjectSet(UObject * InObject)
{
	Item = InObject;
	UpdateEntry();
}

void UUR_Widget_ControlsListEntry::UpdateEntry()
{
	UUR_Object_KeyBind* KeyBind = Cast<UUR_Object_KeyBind>(Item);
	//EntryName->SetText(FText::FromName((KeyBind->ActionName)));
	EntryName->SetText(FText::FromStringTable("/Game/OpenTournament/StringTables/ST_KeyBindingNames.ST_KeyBindingNames", KeyBind->Name.ToString()));
	EntryKeySelector->SetSelectedKey(FInputChord(KeyBind->Key));
}