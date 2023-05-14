// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleCharacter.h"
#include "Camera/CameraComponent.h"
#include <Net/UnrealNetwork.h>
#include "Engine/World.h"
#include "MultiplayerGameModeBase.h"
#include "EngineGlobals.h"
#include "GameLiftServerSDK.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "ReadyPlayerMeComponent.h"

//////////////////////////////////////////////////////////////////////////
// AExampleCharacter

AExampleCharacter::AExampleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ReadyPlayerMeComponent = CreateDefaultSubobject<UReadyPlayerMeComponent>(TEXT("ReadyPlayerMeComponent"));
	AddOwnedComponent(ReadyPlayerMeComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AExampleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AExampleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AExampleCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AExampleCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AExampleCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AExampleCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AExampleCharacter::TouchStopped);
}

void AExampleCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AExampleCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AExampleCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AExampleCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AExampleCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AExampleCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/* This is the testing code for player controller */

void AExampleCharacter::AcceptClient(FString ClientID)
{
	//Getting the module first.
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->AcceptPlayerSession(ClientID);

	UE_LOG(LogTemp, Warning, TEXT("client accepted"));
}

void AExampleCharacter::TerminateGameSession()
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->ProcessEnding();

	UE_LOG(LogTemp, Warning, TEXT("game session terminated"));
}

void AExampleCharacter::RemovePlayerSession(FString SessionId)
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->RemovePlayerSession(SessionId);

	UE_LOG(LogTemp, Warning, TEXT("player session terminated"));
}

void AExampleCharacter::ManageNewPlayer(APlayerController* NewPlayer)
{
	// Super::HandleStartingNewPlayer(NewPlayer);

	if (NewPlayer)
	{
		//These lines are required commented out only for testing purpose

		/*
		FUniqueNetIdRepl UniqueNetIDRepl;
		if (NewPlayer->IsLocalController()) {

			ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
			if (LocalPlayerRef) {
				UniqueNetIDRepl = LocalPlayerRef->GetPreferredUniqueNetId();
			}
			else {
				UNetConnection* RemoteNetConnecionRef = Cast<UNetConnection>(NewPlayer->Player);
				UE_LOG(LogTemp, Warning, TEXT("1"));
				if (IsValid(RemoteNetConnecionRef)) {
					UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
					UE_LOG(LogTemp, Warning, TEXT("2"));
				}
			}
		}
		else {
			UNetConnection* RemoteNetConnecionRef = Cast<UNetConnection>(NewPlayer->Player);
			UE_LOG(LogTemp, Warning, TEXT("4"));
			if (IsValid(RemoteNetConnecionRef)) {
				UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
				UE_LOG(LogTemp, Warning, TEXT("UniqueIdRepl %s"), *UniqueNetIDRepl.ToString());
			}
		}
		TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();
		if (UniqueNetId != nullptr) {
			IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
			IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
			bool bRegSuccess = SessionRef->RegisterPlayer(FName("EOS_ExampleSession"), *UniqueNetId, false);
			if (bRegSuccess) {
				UE_LOG(LogTemp, Warning, TEXT("Registration Successful!"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Registration Nahi hua!"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unique ID is null ptr!"));
		}
		*/
	}
}

void AExampleCharacter::StartVoice()
{

}