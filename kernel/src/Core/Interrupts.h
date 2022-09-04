#pragma once

struct InterruptFrame_t;
typedef struct InterruptFrame_t InterruptFrame;

__attribute__((interrupt)) void PageFault_Handler(InterruptFrame* frame);