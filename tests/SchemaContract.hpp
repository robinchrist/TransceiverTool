// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <nlohmann/json.hpp>

namespace SchemaContract {
    void validate8472(const nlohmann::json& document);
    void validate8636(const nlohmann::json& document);
}  // namespace SchemaContract
