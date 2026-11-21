// SPDX-FileCopyrightText: The halogenOS Project
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace android {
namespace fs_mgr {

// Every reason why fs_mgr_mount_all() left /data unmounted and sends the device to recovery
// without formatting it, as X(value, token). The token names the value in the recovery reason.
// C++ cannot list the values of an enum, so DataMountFailure and the token table are both
// generated from this list, and every value has a token.
#define FS_MGR_DATA_MOUNT_FAILURES(X)                                                            \
    /* The device restarted while vold was encrypting /data. */                                  \
    X(EncryptionInterrupted, "encryption_interrupted")                                           \
    /* /metadata does not contain the key for /data, and fs_mgr cannot format /data: its start   \
       does not look erased, or its fstab entry is not formattable. */                           \
    X(KeyMissing, "key_missing")                                                                 \
    /* fs_mgr could not tell whether the key for /data exists: the partition with the key        \
       directory is not mounted and its start does not look erased, or reading the directory     \
       failed. */                                                                                \
    X(KeyDirectoryUnreadable, "key_directory_unreadable")                                        \
    /* The start of /data looks erased while /metadata still contains the key for /data. */      \
    X(DataLooksErased, "data_looks_erased")                                                      \
    /* Formatting /data, whose start looked erased, failed. */                                   \
    X(FormatFailed, "format_failed")                                                             \
    /* vold could not set up /data with the key that /data needs. */                             \
    X(KeyUnusable, "key_unusable")                                                               \
    /* vold set up /data with the key that /data needs, but its file system did not mount. */    \
    X(FilesystemUnreadable, "filesystem_unreadable")

enum class DataMountFailure {
#define FS_MGR_DATA_MOUNT_FAILURE_VALUE(failure, token) failure,
    FS_MGR_DATA_MOUNT_FAILURES(FS_MGR_DATA_MOUNT_FAILURE_VALUE)
#undef FS_MGR_DATA_MOUNT_FAILURE_VALUE
};

// Returns the value of the --reason argument with which init starts recovery after
// fs_mgr_mount_all(), naming `failure` when there is one.
std::string ToRecoveryReason(std::optional<DataMountFailure> failure);

// Returns the failure named by a --reason value from ToRecoveryReason(), or nothing for any
// other value.
std::optional<DataMountFailure> ParseRecoveryReason(std::string_view reason);

}  // namespace fs_mgr
}  // namespace android
