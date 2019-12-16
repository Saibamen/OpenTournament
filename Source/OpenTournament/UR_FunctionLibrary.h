// Copyright 2019 Open Tournament Project, All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "OpenTournament.h"

#include "UR_GameModeBase.h"

#include "UR_FunctionLibrary.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class AUR_GameMode;

/////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * 
 */
UCLASS()
class OPENTOURNAMENT_API UUR_FunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    // Utility for retrieving GameMode CDO
    UFUNCTION(BlueprintPure, Category = "Character|State")
    static AUR_GameModeBase* GetGameModeDefaultObject(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Character|State") 
    static int32 GetPlayerStateValue(APlayerController* PlayerController);

    template<typename TEnum>
    static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
    {
        const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
        if (!enumPtr)
        {
            return FString("Invalid");
        }
        return enumPtr->GetNameByValue(static_cast<int64>(Value)).ToString();
    }


    /**
    * Resolve the text color a player should be written with.
    * Used for chat, death messages... [insert more]
    * Use to keep consistency around interfaces.
    * Especially considering that later on, players might have :
    * - customizeable team colors
    * - customizeable self color in non team games
    */
    UFUNCTION(BlueprintPure)
    static FColor GetPlayerDisplayTextColor(APlayerState* PS);


    /**
    * Text color for spectators.
    * Use to keep consistency around interfaces.
    */
    UFUNCTION(BlueprintPure)
    static FColor GetSpectatorDisplayTextColor() { return FColor(255, 200, 0, 255); }


    /**
    * Returns a modified version of the string, where rich-text special characters are escaped.
    * If printed as-is, characters like < will be encoded like &lt;
    * If used in a RichTextBlock, they will be displayed properly.
    */
    UFUNCTION(BlueprintPure)
    static FString EscapeForRichText(const FString& InText)
    {
        // see RichTextMarkupProcessing.cpp
        return InText.Replace(TEXT("&"), TEXT("&amp;"))
            .Replace(TEXT("\""), TEXT("&quot;"))
            .Replace(TEXT("<"), TEXT("&lt;"))
            .Replace(TEXT(">"), TEXT("&gt;"));
    }

    /**
    * Returns a modified version of the string, where encoded rich-text special characters are restored.
    */
    UFUNCTION(BlueprintPure)
    static FString UnescapeRichText(const FString& InText)
    {
        return InText.Replace(TEXT("&gt;"), TEXT(">"))
            .Replace(TEXT("&lt;"), TEXT("<"))
            .Replace(TEXT("&quot;"), TEXT("\""))
            .Replace(TEXT("&amp;"), TEXT("&"));
    }

    /**
    * Returns a modified version of the string, where rich-text decorators are visible but undetectable.
    */
    UFUNCTION(BlueprintPure)
    static FString BreakRichTextDecorators(const FString& InText)
    {
        return InText.Replace(TEXT("/>"), TEXT("/\u200B>"));	// zero width space
    }

    /**
    * Returns a modified version of the string without any rich-text decorators.
    * Only markers are removed. The content within decorators is left untouched.
    */
    UFUNCTION(BlueprintPure)
    static FString StripRichTextDecorators(const FString& InText);


    /**
    * Returns true if given key is mapped to given action.
    * Does not consider modifiers.
    *
    * This is a helper for doing special UMG navigation via OnKeyDown,
    * using realtime binds and not hardcoded.
    */
    UFUNCTION(BlueprintPure)
    static bool IsKeyMappedToAction(const FKey& Key, FName ActionName);

    /**
    * Returns true if given key is mapped to given axis.
    * Use direction 0 for any.
    *
    * This is a helper for doing special UMG navigation via OnKeyDown,
    * using realtime binds and not hardcoded.
    */
    UFUNCTION(BlueprintPure)
    static bool IsKeyMappedToAxis(const FKey& Key, FName AxisName, float Direction=1.f);

    /**
    * Get local player controller as UR_PlayerController.
    *
    * Flagged as cosmetic-only to put emphasis on client-side usage.
    * Should be avoided on dedicated server code as the result would be random.
    */
    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Game", Meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
    static class AUR_PlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

    /**
    * Get the Time as a String
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FunctionLibrary")
    static FString GetTimeString(const float TimeSeconds);
};
