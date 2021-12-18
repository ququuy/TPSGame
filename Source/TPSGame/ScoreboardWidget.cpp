// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreboardWidget.h"

#include "Components/TextBlock.h"

void UScoreboardWidget::NativeConstruct()
{
	// Call the Blueprint "Event Construct" node
    Super::NativeConstruct();
	DisplayedScore = 0;
    if (TextScore)
    {
    	FString StrScore = "Score: ";
    	TextScore->SetText(FText::FromString(StrScore + FString::FromInt(DisplayedScore)));
    	FString StrKill = "Kill: ";
    	TextKill->SetText(FText::FromString(StrKill+ FString::FromInt(DisplayedKill)));
    }
}

void UScoreboardWidget::UpdateDisplayedScore(int NewScore)
{
	DisplayedScore = NewScore;
    if (TextScore)
    {
    	FString StrScore = "Score: ";
    	TextScore->SetText(FText::FromString(StrScore + FString::FromInt(DisplayedScore)));
    }
}

void UScoreboardWidget::UpdateDisplayedKill(int NewKill)
{
	DisplayedKill = NewKill;
    if (TextKill)
    {
    	FString StrKill = "Kill: ";
    	TextKill->SetText(FText::FromString(StrKill + FString::FromInt(DisplayedKill)));
    }
}
