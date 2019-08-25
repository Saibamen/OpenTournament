// Fill out your copyright notice in the Description page of Project Settings.


#include "UR_Projectile_Assault.h"

// Sets default values
AUR_Projectile_Assault::AUR_Projectile_Assault(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> newAsset(TEXT("StaticMesh'/Game/SciFiWeapDark/Weapons/Darkness_AssaultRifle_Ammo.Darkness_AssaultRifle_Ammo'"));
	UStaticMesh* helper = newAsset.Object;
	ProjMesh->SetStaticMesh(helper);

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particles->SetRelativeLocation(FVector::ZeroVector);
	Particles->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticlesInAssets(TEXT("ParticleSystem'/Game/SciFiWeapDark/FX/Particles/P_AssaultRifle_Tracer_Dark.P_AssaultRifle_Tracer_Dark'"));

	Particles->SetTemplate(ParticlesInAssets.Object);
}