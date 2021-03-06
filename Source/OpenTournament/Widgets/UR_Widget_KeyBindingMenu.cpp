// Copyright (c) 2019-2020 Open Tournament Project, All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "UR_Widget_KeyBindingMenu.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "GameFramework/InputSettings.h"

#include "Data/UR_Object_KeyBind.h"
#include "UR_BasePlayerController.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

class UUR_PlayerInput;

/////////////////////////////////////////////////////////////////////////////////////////////////

UUR_Widget_KeyBindingMenu::UUR_Widget_KeyBindingMenu(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer),
    OwningPlayer(nullptr)
{
    
}

//Should check these again when we decide how our menu system will work
void UUR_Widget_KeyBindingMenu::OnCloseButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("CloseButton Clicked"));
    /*
    OwningPlayer->SetInputMode(FInputModeGameOnly());
    OwningPlayer->bShowMouseCursor = false;
    RemoveFromViewport();
    */
    Close();
}

//TODO: remove
void UUR_Widget_KeyBindingMenu::OpenMenu()
{
    OwningPlayer = GetOwningPlayer();
    OwningPlayer->SetInputMode(FInputModeUIOnly());
    OwningPlayer->bShowMouseCursor = true;
    AddToViewport();
}

void UUR_Widget_KeyBindingMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (CloseButton != nullptr)
    {
        CloseButton->OnClicked.AddDynamic(this, &UUR_Widget_KeyBindingMenu::OnCloseButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot bind CloseButton event: CloseButton pointer is null."));
    }

    PopulateKeyBindingList();
}

void UUR_Widget_KeyBindingMenu::CreateKeyBindObject(FName Name, FKey Key)
{
    UUR_Object_KeyBind* NewKeyBind = NewObject<UUR_Object_KeyBind>(this);
    NewKeyBind->Name = Name;
    NewKeyBind->Key = Key;

    ControlsList->AddItem(NewKeyBind);
}

void UUR_Widget_KeyBindingMenu::PopulateKeyBindingList()
{
    AUR_BasePlayerController* Player = Cast<AUR_BasePlayerController>(GetOwningPlayer());
    const UInputSettings * Settings = GetDefault<UInputSettings>();

    for (TIndexedContainerIterator<TArray<FName>, FName, int32> IterAxis = AxisNames.CreateIterator(); IterAxis; ++IterAxis)
    {
        TArray<FInputAxisKeyMapping> AxisMappingsForName;
        FName AxisName = AxisNames[IterAxis.GetIndex()];
        Settings->GetAxisMappingByName(AxisName, AxisMappingsForName);

        if (AxisMappingsForName.Num() > 0)
        {
            const FInputAxisKeyMapping AxisMapping = AxisMappingsForName[0];
            CreateKeyBindObject(AxisMapping.AxisName, AxisMapping.Key);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No axis with that name found. Name was: %s"), *AxisName.ToString());
        }
    }

    for (TIndexedContainerIterator<TArray<FName>, FName, int32> IterAction = ActionNames.CreateIterator(); IterAction; ++IterAction)
    {
        TArray<FInputActionKeyMapping> ActionMappingsForName;
        FName ActionName = ActionNames[IterAction.GetIndex()];
        Settings->GetActionMappingByName(ActionName, ActionMappingsForName);

        if (ActionMappingsForName.Num() > 0)
        {
            const FInputActionKeyMapping ActionMapping = ActionMappingsForName[0];
            CreateKeyBindObject(ActionMapping.ActionName, ActionMapping.Key);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No action with that name found. Name was: %s"), *ActionName.ToString());
        }
    }
}
