#pragma once

struct InterruptFrame;

__attribute__((interrupt)) void PageFault_Handler(struct InterruptFrame* frame);
__attribute__((interrupt)) void DoubleFault_Handler(struct InterruptFrame* frame);
__attribute__((interrupt)) void KeyboardInterrupt_Handler(struct InterruptFrame* frame);
__attribute__((interrupt)) void GeneralProtectionFault_Handler(struct InterruptFrame* frame);
