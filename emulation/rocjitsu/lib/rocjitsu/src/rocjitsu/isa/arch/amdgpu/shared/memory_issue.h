// Copyright (c) 2026 Advanced Micro Devices, Inc.
// SPDX-License-Identifier: MIT

#ifndef ROCJITSU_ISA_ARCH_AMDGPU_SHARED_MEMORY_ISSUE_H_
#define ROCJITSU_ISA_ARCH_AMDGPU_SHARED_MEMORY_ISSUE_H_

#include "rocjitsu/isa/arch/amdgpu/shared/wait_counter.h"

#include <cstdint>
#include <optional>

namespace rocjitsu::amdgpu {

/// @brief Completion ordering associated with an AMDGPU memory issue.
/// @details Counter membership and completion ordering are separate: operations
/// can share a wait counter without completing in one usable FIFO order.
enum class MemoryCompletionClass : uint8_t {
  UNCLASSIFIED,
  VMEM,
  LDS,
  UNORDERED,
};

/// @brief Typed description of an AMDGPU instruction's memory-issue semantics.
/// @details Generic FLAT instructions can consume either their primary counter
/// or the local-memory counter depending on the address resolved during
/// execution. In that case alternate_wait_counter_type names the counter to use
/// when routing resolves to local memory; neither domain alone can impose
/// provable pre-operand backpressure until that choice is known. exec_masked
/// distinguishes ordinary vector memory operations from scalar memory and the
/// few vector operations that execute independently of EXEC.
struct MemoryIssueInfo {
  WaitCounterType wait_counter_type = WaitCounterType::VMCNT;
  MemoryCompletionClass completion_class = MemoryCompletionClass::UNCLASSIFIED;
  std::optional<WaitCounterType> alternate_wait_counter_type;
  bool exec_masked = true;
};

} // namespace rocjitsu::amdgpu

#endif // ROCJITSU_ISA_ARCH_AMDGPU_SHARED_MEMORY_ISSUE_H_
