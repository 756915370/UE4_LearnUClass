// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//对运行时对象进行测试
	UE_LOG(LogTemp, Warning, TEXT("——————————对运行时对象进行测试——————————"));
	FString SelfName = this->GetName();
	UE_LOG(LogTemp, Warning, TEXT("SelfName %s"), *SelfName);
	FString SelfCName = this->GetClass()->GetFName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("SelfClassName %s"), *SelfCName);
	FString SelfSCName = this->GetClass()->GetSuperClass()->GetFName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("SelfSuperClassName %s"), *SelfSCName);
	FString SelfSSCName = this->GetClass()->GetSuperClass()->GetSuperClass()->GetFName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("SelfSSClassName %s"), *SelfSSCName);
	//单独加载GeneratedClass测试
	UE_LOG(LogTemp, Warning, TEXT("——————————单独加载GeneratedClass测试—————————"));
	UObject* ChildObj = LoadObject<UObject>(nullptr,TEXT("WidgetBlueprint'/Game/UMG/UMG_Child.UMG_Child_C'"));
	UObject* ParentObj = LoadObject<UObject>(nullptr,TEXT("WidgetBlueprint'/Game/UMG/UMG_Parent.UMG_Parent_C'"));
	UObject* ActorObj = LoadObject<UObject>(nullptr,TEXT("Blueprint'/Game/BP/BP_MyActor.BP_MyActor_C'"));
	if (ChildObj && ParentObj)
	{
		FString ChildObjName = ChildObj->GetFName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("ChildObjName %s"), *ChildObjName);
		FString ParentObjName = ParentObj->GetFName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("ParentObjName %s"), *ParentObjName);
	}
	if (this->GetClass() == ChildObj)
	{
		UE_LOG(LogTemp, Warning, TEXT("运行时的UClass和蓝图的GeneratedClass相同"),);
	}
	if(ChildObj != ParentObj)
	{
		UE_LOG(LogTemp, Warning, TEXT("Child和Parent不同"),);
	}
	if(ChildObj->GetClass() == ParentObj->GetClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Child的UClass和Parent的UClass相同"),);
	}
	if(ChildObj->GetClass()->GetClass() == ActorObj->GetClass()->GetClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Child的UClass的UClass和Actor的UClass的UClass相同"),);
	}
	//加载蓝图进行测试
	UE_LOG(LogTemp, Warning, TEXT("——————————加载蓝图进行测试测试—————————"));
	UObject* Obj = LoadObject<UObject>(nullptr,TEXT("WidgetBlueprint'/Game/UMG/UMG_Child.UMG_Child'"));
	if (UBlueprint* BpObj = Cast<UBlueprint>(Obj))
	{
		//蓝图父类
		if (auto BpSuper = BpObj->GetClass()->GetSuperClass())
		{
			FString ObjName = BpSuper->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("SuperClassName %s"), *ObjName);
		}
		//蓝图的ParentClass
		if (auto BpParent = BpObj->ParentClass)
		{
			FString ObjName = BpParent->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("ParentName %s"), *ObjName);
		}
		//蓝图的GeneratedClass
		if (auto BpGen = BpObj->GeneratedClass)
		{
			if (Cast<UWidgetBlueprintGeneratedClass>(BpGen))
			{
				FString ObjName = BpGen->GetFName().ToString();
				UE_LOG(LogTemp, Warning, TEXT("GenClassName %s"), *ObjName);
			}
			//蓝图的GeneratedClass的父类
			UClass* BpGenSuperClass = BpGen->GetSuperClass();
			if (Cast<UWidgetBlueprintGeneratedClass>(BpGenSuperClass))
			{
				FString GenSuperClassName = BpGenSuperClass->GetFName().ToString();
				UE_LOG(LogTemp, Warning, TEXT("GenSuperClassName %s"), *GenSuperClassName);
			}
			//蓝图的GeneratedClass的父类的父类
			UClass* BpGenSSClass = BpGen->GetSuperClass()->GetSuperClass();
			if (Cast<UWidgetBlueprintGeneratedClass>(BpGenSSClass) == nullptr)
			{
				FString GenSSClassName = BpGenSSClass->GetFName().ToString();
				UE_LOG(LogTemp, Warning, TEXT("GenSSClassName %s"), *GenSSClassName);
			}
		}
		//蓝图的GeneratedClass一开始从哪个C++类派生
		if (auto BpGenNative = GetParentNativeClass(BpObj->GeneratedClass))
		{
			FString ObjName = BpGenNative->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("ParentGenNativeName %s"), *ObjName);
		}
		//蓝图的通用GenerateClass类型
		if (auto BpClass = BpObj->GetBlueprintClass())
		{
			FString ObjName = BpClass->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("BlueprintClassName %s"), *ObjName);
		}
		//蓝图类，返回WidgetBlueprint
		if (auto CClass = Obj->GetClass())
		{
			FString ObjName = CClass->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("BpClassName %s"), *ObjName);
		}
		//蓝图一开始从哪个C++类派生，返回WidgetBlueprint。没有意义，因为蓝图类不是由蓝图生成的类，所以它自己就是纯血NativeC++类。
		if (auto ObjNative = GetParentNativeClass(Obj->GetClass()))
		{
			FString ObjName = ObjNative->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("BpParentNativeName %s"), *ObjName);
		}
	}
}
