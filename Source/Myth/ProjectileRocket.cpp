//#include "ProjectileRocket.h"
//#include "A01_BlockoutShooterCharacter.h"
//
//
//// Sets default values
//AProjectileRocket::AProjectileRocket()
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//	RocketMesh = CreateDefaultSubobject
//		<UStaticMeshComponent>(TEXT
//		("Mesh"));
//	RootComponent = RocketMesh;
//	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
//	ProjectileMovementComponent->SetUpdatedComponent(RocketMesh);
//
//	bReplicates = true;
//	RocketMesh->SetIsReplicated(true);
//	ProjectileMovementComponent->SetIsReplicated(true);
//
//	ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Hitbox"));
//	ExplosionRadius->SetupAttachment(FieldSystemComponent);
//	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
//	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
//	CullingField = CreateDefaultSubobject<UCullingField>(TEXT("Culling Field"));
//	FieldSystemComponent->SetIsReplicated(true);
//}
//
//// Called when the game starts or when spawned
//void AProjectileRocket::BeginPlay()
//{
//	Super::BeginPlay();
//
//	ProjectileMovementComponent->InitialSpeed = 5000;
//	ProjectileMovementComponent->MaxSpeed = 5000;
//	ProjectileMovementComponent->bRotationFollowsVelocity = true;
//	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
//
//	RocketMesh->SetSimulatePhysics(true);
//	RocketMesh->SetCollisionProfileName("BlockAll");
//	RocketMesh->SetNotifyRigidBodyCollision(true);
//	RocketMesh->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnRocketHit);
//}
//
//// Called every frame
//void AProjectileRocket::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//void AProjectileRocket::OnRocketHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComponent, FVector Normal, const FHitResult& HitResult)
//{
//	if (HasAuthority())
//	{
//		if (OtherActor && Owner != OtherActor)
//		{
//			ServerExplode(OtherActor, HitResult);
//		}
//	}
//}
//
//void AProjectileRocket::FireInDirection(const FVector& ShootDirection)
//{
//	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
//	RocketMesh->AddImpulse(ShootDirection * ProjectileMovementComponent->InitialSpeed);
//}
//
