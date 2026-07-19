//
// Created by mooming on 18/07/2026.
//

#include "System.h"
#include "StorageIOSystem.h"


namespace hs::system
{

System& System::GetInstance()
{
	static System instance;
	return instance;
}

System::System()
	: storageIOSystem_(std::make_unique<StorageIOSystem>())
	, initialized_(false)
{
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
	initialized_ = true;
}

void System::Shutdown()
{
	if (!initialized_)
	{
		return;
	}
	
	storageIOSystem_->Shutdown();
	initialized_ = false;
}

StorageIOSystem& System::GetStorageIOSystem()
{
	return *storageIOSystem_;
}

bool System::IsInitialized() const
{
	return initialized_;
}

} // namespace hs::system
