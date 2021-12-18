// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int DisplayedScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int DisplayedKill;
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextScore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextKill;

	UFUNCTION(BlueprintCallable, Category = "Shoot Event")
	void UpdateDisplayedScore(int NewScore);
	
	UFUNCTION(BlueprintCallable, Category = "Shoot Event")
	void UpdateDisplayedKill(int NewKill);
};
