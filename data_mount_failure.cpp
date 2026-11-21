// SPDX-FileCopyrightText: The halogenOS Project
// SPDX-License-Identifier: Apache-2.0

#include <fs_mgr/data_mount_failure.h>

#include <algorithm>

namespace android {
namespace fs_mgr {

namespace {

constexpr std::string_view kMountAllReason = "fs_mgr_mount_all";

struct FailureToken {
    DataMountFailure failure;
    std::string_view token;
};

constexpr FailureToken kFailureTokens[] = {
#define FS_MGR_DATA_MOUNT_FAILURE_TOKEN(failure, token) {DataMountFailure::failure, token},
        FS_MGR_DATA_MOUNT_FAILURES(FS_MGR_DATA_MOUNT_FAILURE_TOKEN)
#undef FS_MGR_DATA_MOUNT_FAILURE_TOKEN
};

}  // namespace

std::string ToRecoveryReason(std::optional<DataMountFailure> failure) {
    std::string reason(kMountAllReason);
    if (failure) {
        reason += ':';
        reason += std::ranges::find(kFailureTokens, *failure, &FailureToken::failure)->token;
    }
    return reason;
}

std::optional<DataMountFailure> ParseRecoveryReason(std::string_view reason) {
    for (const FailureToken& entry : kFailureTokens) {
        if (reason == ToRecoveryReason(entry.failure)) return entry.failure;
    }
    return std::nullopt;
}

}  // namespace fs_mgr
}  // namespace android
