//
// Created by mooming on 18/07/2026.
//

#pragma once
#include "HSTypes.h"

namespace hs
{
template<typename TContainer>
class IndexedItem final
{
private:
    TIndex index = 0;
    TContainer& container;

public:
    IndexedItem(TContainer& container, TIndex index) : container(container), index(index)
    {
    }

	[[nodiscard]] bool IsValid() const { return index < container.size(); }

	[[nodiscard]] TIndex GetIndex() const { return index; }
	[[nodiscard]] TContainer& GetContainer() { return container; }

	using TItem = decltype(container[0]);
	[[nodiscard]] TItem& Get() { return container[index]; }
	[[nodiscard]] const TItem& Get() const { return container[index]; }

	[[nodiscard]] TItem& operator* () { return container[index]; }
	[[nodiscard]] const TItem& operator* () const { return container[index]; }

	[[nodiscard]] bool operator == (const IndexedItem& other) const { return index == other.index && &container == &other.container; }
	[[nodiscard]] bool operator != (const IndexedItem& other) const { return  *this != other; }
};
} // hs