// Fill out your copyright notice in the Description page of Project Settings.


#include "CompileBpBlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

#include "Windows/WindowsPlatformProcess.h"

DEFINE_LOG_CATEGORY(LogCompileBlueprints);

void UCompileBpBlueprintFunctionLibrary::CompileAllBlueprints()
{
	FString ProjFilePath = FPaths::GetProjectFilePath();
	FString UE4EditorCmd = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries"), TEXT("Win64"), TEXT("UE4Editor-cmd.exe")));
	FString Params = "\"" + ProjFilePath + "\"" + " -run=CompileAllBlueprints";

	int32 ReturnCode = -1;

	void* PipeRead = nullptr;
	void* PipeWrite = nullptr;

	if (!FPlatformProcess::CreatePipe(PipeRead, PipeWrite))
	{
		return;
	}

	FProcHandle Proc = FPlatformProcess::CreateProc(*UE4EditorCmd, *Params, true, false, false, nullptr, 0, nullptr, PipeWrite);

	UE_LOG(LogCompileBlueprints, Warning, TEXT(" > %s %s"), *UE4EditorCmd, *Params);

	FString StringOutput;

	if (Proc.IsValid())
	{
		while (FPlatformProcess::IsProcRunning(Proc))
		{
			FString ThisRead = FPlatformProcess::ReadPipe(PipeRead);
			StringOutput += ThisRead;
		}

		StringOutput += FPlatformProcess::ReadPipe(PipeRead);

		TArray<FString> arr;

		StringOutput.ParseIntoArrayLines(arr);

		for (FString s : arr)
		{
			if (s.Contains(TEXT("error"), ESearchCase::IgnoreCase))
			{
				UE_LOG(LogCompileBlueprints, Error, TEXT("%s"), *s);
			}
			else
			{
				UE_LOG(LogCompileBlueprints, Warning, TEXT("%s"), *s);
			}
		}

		FPlatformProcess::GetProcReturnCode(Proc, &ReturnCode);
	}

	FPlatformProcess::ClosePipe(PipeRead, PipeWrite);
	FPlatformProcess::CloseProc(Proc);
}

