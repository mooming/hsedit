//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


namespace hs::module
{

class EventBus final
{
public:
	using Handler = std::function<void(const std::string& data)>;

	/// @brief Subscribe to an event, returns handler ID for later unsubscription
	[[nodiscard]] size_t Subscribe(const std::string& event, Handler handler);

	/// @brief Publish an event
	void Publish(const std::string& event, const std::string& data = "");

	/// @brief Unsubscribe by handler ID
	void Unsubscribe(const std::string& event, size_t handler_id);

	/// @brief Clear all subscriptions
	void Clear();

private:
	std::unordered_map<std::string, std::vector<Handler>> handlers_;
};

} // namespace hs::module
