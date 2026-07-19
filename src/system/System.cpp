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
	: initialized_(false)
{
	Initialize();
}

System::~System()
{
	Shutdown();
}

void System::Initialize()
{
	if (initialized_)
	{
		return;
	}

	storageIOSystem.Initialize();
	virtualTextBufferSystem.Initialize();
	initialized_ = true;
}

void System::Shutdown()
{
	if (!initialized_)
	{
		return;
	}

	virtualTextBufferSystem.Shutdown();
	storageIOSystem.Shutdown();
	initialized_ = false;
}

void System::Update(float deltaTime)
{
	if (!initialized_)
	{
		return;
	}

	// Subsystems handle their own updates internally
}

StorageIOSystem& System::GetStorageIOSystem()
{
	return storageIOSystem;
}

VirtualTextBufferSystem& System::GetVirtualTextBufferSystem()
{
	return virtualTextBufferSystem;
}

bool System::IsInitialized() const
{
	return initialized_;
}

} // namespace hs::system
