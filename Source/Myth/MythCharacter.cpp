#include "MythCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Projectile.h"
//#include "../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Private/ApexDestructionModule.cpp"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMythCharacter

AMythCharacter::AMythCharacter()
{
	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	//GetCapsuleComponent()->SetNotifyRigidBodyCollision(true); // Enables collision notifications
	
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMythCharacter::OnHit);

	//GetMesh()->SetCollisionProfileName(TEXT("Pawn")); // Use an appropriate collision profile
	//GetMesh()->SetNotifyRigidBodyCollision(true); // Enable hit events
	//GetMesh()->OnComponentHit.AddDynamic(this, &AMythCharacter::OnMeshHit);
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
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	//MythFieldSystemComponent = CreateDefaultSubobject<UMythFieldSystemComponent>(TEXT("FieldSystemComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}
//void AMythCharacter::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	UE_LOG(LogTemplateCharacter, Warning, TEXT("Its mesh hitting"));
//
//	// Ensure we hit something valid and that it's not ourselves
//	if (OtherActor && OtherActor != this && OtherComp)
//	{
//		// Log the hit for debugging
//		UE_LOG(LogTemp, Warning, TEXT("Mesh hit actor: %s"), *OtherActor->GetName());
//
//		// Example: Apply a force to the other actor if it simulates physics
//		if (OtherComp->IsSimulatingPhysics())
//		{
//			FVector ForceDirection = NormalImpulse.GetSafeNormal();
//			OtherComp->AddImpulse(ForceDirection * 1000.0f); // Adjust force as needed
//		}
//	}
//}
void AMythCharacter::OnLeftMouseClick()
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Its clickingtting"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // Extend the ray far enough into the world

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this); // Ignore the character itself

			// Perform the line trace
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
			{
				// Process the hit result
				if (UGeometryCollectionComponent* DestructibleComp = Cast<UGeometryCollectionComponent>(HitResult.Component))
				{
					FVector ImpulseDir = HitResult.ImpactNormal * -1.0f; // Impulse direction opposite to the hit normal
					float ImpulseStrength = 10000.0f; // Adjust this value as needed

					DestructibleComp->AddImpulseAtLocation(ImpulseDir * ImpulseStrength, HitResult.Location);

					UE_LOG(LogTemp, Warning, TEXT("Applied impulse at location: %s"), *HitResult.Location.ToString());
				}
			}
		}
	}
}


void AMythCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemplateCharacter, Warning, TEXT("Applied damage to GeometryCollection at "));
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Its hitting"));
	if (OtherActor && OtherActor != this && OtherComponent)
	{
	//	FVector FootLocation;
	//	bool bHitLeftFoot = false;
	//	bool bHitRightFoot = false;

	//	// Use sockets if available
	///*	FVector LeftFootLocation = GetMesh()->GetSocketLocation(TEXT("LeftFootSocket"));
	//	FVector RightFootLocation = GetMesh()->GetSocketLocation(TEXT("RightFootSocket"));*/

	//	// Alternatively, use bone locations if not using sockets
	//	 FVector LeftFootLocation = GetMesh()->GetBoneLocation(TEXT("hand_l"));
	//	 FVector RightFootLocation = GetMesh()->GetBoneLocation(TEXT("foot_r"));

	//	// Determine which foot is closer to the hit location
	//	if ((Hit.ImpactPoint - LeftFootLocation).Size() < (Hit.ImpactPoint - RightFootLocation).Size())
	//	{
	//		FootLocation = LeftFootLocation;
	//		bHitLeftFoot = true;
	//	}
	//	else
	//	{
	//		FootLocation = RightFootLocation;
	//		bHitRightFoot = true;
	//	}

		// Apply force at the foot location if we hit a destructible component
		UGeometryCollectionComponent* DestructibleComp = Cast<UGeometryCollectionComponent>(OtherComponent);
		if (DestructibleComp) 
		{
			 FVector ImpulseDir = NormalImpulse.GetSafeNormal();
			//FVector ImpulseDir = FVector(0.0f, 0.0f, -1.0f); // Directly down
			float ImpulseStrength = 10000.0f; // Adjust this value as needed

			DestructibleComp->AddImpulseAtLocation(ImpulseDir * ImpulseStrength, ImpulseDir);
		/*	DestructibleComp->AddImpulseAtLocation(ImpulseDir * ImpulseStrength, FootLocation);
			if (bHitLeftFoot)
			{
				UE_LOG(LogTemp, Warning, TEXT("Applied force at left foot location: %s"), *FootLocation.ToString());
			}
			else if (bHitRightFoot)
			{
				UE_LOG(LogTemp, Warning, TEXT("Applied force at right foot location: %s"), *FootLocation.ToString());
			}*/
		}
		//UGeometryCollectionComponent* GeometryCollectionComponent = OtherActor->FindComponentByClass<UGeometryCollectionComponent>();
		//if (GeometryCollectionComponent)
		//{
		//	// Calculate impact point and direction
		//	FVector ImpactPoint = Hit.ImpactPoint;
		//	FVector ImpactDirection = -Hit.ImpactNormal; // Reverse the normal for correct direction

		//	// Calculate impact strength based on character's velocity
		//	float ImpactStrength = GetVelocity().Size() * 100.0f; // Adjust multiplier as needed

		//	// Apply damage to the GeometryCollection
		//	//GeometryCollectionComponent->ApplyDamage(ImpactStrength, ImpactPoint, ImpactDirection, 1.0f);

		//	// Optionally, apply an impulse for additional physical response
		//	float ImpulseStrength = ImpactStrength * 10.0f; // Adjust as needed
		//	GeometryCollectionComponent->AddImpulseAtLocation(ImpactDirection * ImpulseStrength, ImpactPoint);
		//	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());

		//}
	}
}
void AMythCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true; // Show the mouse cursor
		PlayerController->bEnableClickEvents = true; // Enable mouse click events
		PlayerController->bEnableMouseOverEvents = true; // Enable mouse over events
	}


	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMythCharacter::OnHit);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMythCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMythCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMythCharacter::Look);
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMythCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMythCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMythCharacter::Shoot);
		EnhancedInputComponent->BindAction(LeftMouseClickAction, ETriggerEvent::Triggered, this, &AMythCharacter::OnLeftMouseClick);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
void AMythCharacter::Shoot()
{

	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FVector MuzzleLocation = GetMesh()->GetBoneLocation("head") ;// GetSocketLocation("MuzzleSocket");
		FRotator MuzzleRotation = Controller->GetControlRotation();

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass, MuzzleLocation
			, MuzzleRotation, SpawnParams);
		//UE_LOG(LogTemplateCharacter, Warning, TEXT("Its ashooting"));

		/*if (Projectile)
		{
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}*/
	}

	//FVector MuzzleLocation = GetMesh()->GetBoneLocation("head");
	//MythFieldSystemComponent->ApplyDamageToGeometryCollection(100.0f, 500.0f, MuzzleLocation);
}
void AMythCharacter::StartSprinting()
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Its a s sprintting"));

	GetCharacterMovement()->MaxWalkSpeed = 1000.f; // Increase speed to sprint
	//UE_LOG(LogTemplateCharacter, Warning, TEXT("Its sprinting"));
}

void AMythCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // Revert speed to walk
}

void AMythCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMythCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}