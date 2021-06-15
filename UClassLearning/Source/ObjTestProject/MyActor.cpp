// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	//�������ε���StaticClass�Ƿ���ͬ
	auto static1Class = AMyActor::StaticClass();
	auto static2Class = AMyActor::StaticClass();
	auto result = static1Class == static2Class;
	//�����1
	UE_LOG(LogTemp, Warning, TEXT("Is StaticClass Same: %d"), result);

	//��ε���GetClass����
	auto uClass = GetClass();
	auto ucClass = GetClass()->GetClass();
	auto uccClass = GetClass()->GetClass()->GetClass();
	auto uccccClass = GetClass()->GetClass()->GetClass()->GetClass();
	auto isSame = uClass == ucClass;
	UE_LOG(LogTemp, Warning, TEXT("uClass IsSame ucClass: %d"), isSame);//false
	 isSame = ucClass == uccClass;
	UE_LOG(LogTemp, Warning, TEXT("ucClass IsSame uccClass: %d"), isSame);//false
	 isSame = uccClass == uccccClass;
	UE_LOG(LogTemp, Warning, TEXT("uccClass IsSame ucccClass: %d"), isSame);//true
	
	//ClassDefaultObject����
	testIntValue = -1;
	auto defaultInt = GetClass()->GetDefaultObject<AMyActor>()->testIntValue;
	UE_LOG(LogTemp, Warning, TEXT("DefaultObject int: %d"), defaultInt);
	auto staticDefaultInt = AMyActor::StaticClass()->GetDefaultObject<AMyActor>()->testIntValue;
	UE_LOG(LogTemp, Warning, TEXT("StaticDefaultObject int: %d"), staticDefaultInt);
	
	//��������
	//auto staticInt = Cast<AMyActor>(AMyActor::StaticClass());
	//UE_LOG(LogTemp, Warning, TEXT("StaticObject int: %d"), staticInt);
}

//������һ��MyActor��GetClass�Ƿ���ͬ
void AMyActor::IsSameUClass(AMyActor* anotherActor)
{
	int result = GetClass() == anotherActor->GetClass();
	UE_LOG(LogTemp, Warning, TEXT("IsSame s: %d"), result);
}

