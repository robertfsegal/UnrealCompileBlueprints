// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CompileBpBlueprintFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCompileBP, Log, All);

/**
 * 
 */
UCLASS()
class COMPILEBPTESTGAME_API UCompileBpBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "CompileBpBlueprintFunctionLibrary")
	static void CompileAllBlueprints();
};
