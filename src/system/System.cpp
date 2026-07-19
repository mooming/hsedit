//
// Created by mooming on 18/07/2026.
//

#include "System.h"
#include "StorageIOSystem.h"
#include "VirtualTextBufferSystem.h"


namespace hs::system
{

System& System::GetInstance()
{
	static System instance;
	return instance;
}

System::System()
	: virtualTextBufferSystem()
{
	Initialize();
}

System::~System()
{
	Shutdown();
}

System::System(System&&) noexcept = default;

System& System::operator=(System&&) noexcept = default;

void System::Initialize()
{
	if (initialized_)
	{
		return;
	}
	
	storageIOSystem_->Initialize();
	virtualTextBufferSystem_->Initialize();
	initialized_ = true;
}

void System::Shutdown()
{
	if (!initialized_)
	{
		return;
	}
	
	virtualTextBufferSystem_->Shutdown();
	storageIOSystem_->Shutdown();
	initialized_ = false;
}

void System::Update(float deltaTime)
{
	if (!initialized_)
	{
		return;
	}
	
	virtualTextBufferSystem_->Update(deltaTime);
	storageIOSystem_->Update(deltaTime);
}

StorageIOSystem& System::GetStorageIOSystem()
{
	return *storageIOSystem_;
}

VirtualTextBufferSystem& System::GetVirtualTextBufferSystem()
{
	return *virtualTextBufferSystem_;
}

bool System::IsInitialized() const
{
	return initialized_;
}

} // namespace hs::system
