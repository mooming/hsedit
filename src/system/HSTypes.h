//
// Created by mooming on 17/07/2026.
//

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace hs
{
using TLine = std::u8string;
using TLines = std::vector<TLine>;
using TIndex = size_t;
using TLineIndex = TIndex;
using TPageIndex = TIndex;
using TFilePath = std::filesystem::path;
using TTimestamp = std::chrono::steady_clock::time_point;
} // hs
