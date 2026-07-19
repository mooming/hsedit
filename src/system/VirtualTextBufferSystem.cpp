//
// Created by mooming on 18/07/2026.
//

#include "VirtualTextBufferSystem.h"


namespace hs::system
{

VirtualTextBufferSystem::VirtualTextBufferSystem()
	: running(false)
{
}

VirtualTextBufferSystem::~VirtualTextBufferSystem()
{
	if (running.load(std::memory_order_acquire))
	{
		Shutdown();
	}
}

void VirtualTextBufferSystem::Initialize()
{
	running.store(true, std::memory_order_release);
	workerThread = std::thread([this]()
	{
		while (running.load(std::memory_order_acquire))
		{
			// Worker thread loop
		}
	});
}

void VirtualTextBufferSystem::Shutdown()
{
	running.store(false, std::memory_order::release);

	if (workerThread.joinable())
	{
		workerThread.join();
	}
}

} // namespace hs::system
