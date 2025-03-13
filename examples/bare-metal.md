# Bare-Metal Programming with COIL/HOIL

This document provides practical examples of using COIL/HOIL for bare-metal programming scenarios, including bootloaders, firmware, and device drivers. These examples rely solely on core COIL functionality with zero runtime dependencies.

## 1. Simple Bootloader

This example shows a basic bootloader that initializes hardware and loads a kernel from storage.

```
MODULE "bootloader";

// Hardware memory-mapped registers
CONSTANT UART_BASE: u64 = 0xF000_1000;
CONSTANT UART_DATA: u8 = 0x00;
CONSTANT UART_STATUS: u8 = 0x04;
CONSTANT UART_CONTROL: u8 = 0x08;

CONSTANT DISK_BASE: u64 = 0xF000_2000;
CONSTANT DISK_DATA: u8 = 0x00;
CONSTANT DISK_SECTOR: u8 = 0x04;
CONSTANT DISK_COUNT: u8 = 0x08;
CONSTANT DISK_STATUS: u8 = 0x0C;
CONSTANT DISK_COMMAND: u8 = 0x10;

CONSTANT DISK_COMMAND_READ: u8 = 0x01;
CONSTANT DISK_STATUS_READY: u8 = 0x01;
CONSTANT DISK_STATUS_BUSY: u8 = 0x02;

// Kernel loading parameters
CONSTANT KERNEL_LOAD_ADDR: u64 = 0x0010_0000;
CONSTANT KERNEL_START_SECTOR: u32 = 16;
CONSTANT KERNEL_SECTOR_COUNT: u32 = 256;

// Entry point
FUNCTION _start() -> void {
    ENTRY:
        // Initialize UART for debugging output
        CALL init_uart();
        
        // Print boot message
        CALL print_string("BOOTING...\n");
        
        // Load kernel from disk
        CALL load_kernel();
        
        // Print success message
        CALL print_string("KERNEL LOADED\n");
        
        // Jump to kernel entry point
        entry_point = CONVERT KERNEL_LOAD_ADDR, ptr<void>;
        JMP entry_point;
        
        // Should never reach here
        HLT;
}

// Initialize UART
FUNCTION init_uart() -> void {
    ENTRY:
        uart_base = LOAD_U64 UART_BASE;
        
        // Set control register (enable TX/RX, 8N1)
        control_addr = ADD uart_base, UART_CONTROL;
        STORE [control_addr], 0x03;
        
        RET;
}

// Output a character to UART
FUNCTION putchar(c: u8) -> void {
    ENTRY:
        uart_base = LOAD_U64 UART_BASE;
        
        // Calculate register addresses
        status_addr = ADD uart_base, UART_STATUS;
        data_addr = ADD uart_base, UART_DATA;
        
    WAIT_LOOP:
        // Check if UART is ready for transmission
        status = LOAD [status_addr];
        is_ready = AND status, 0x01;
        ready = CMP_NE is_ready, 0;
        BR ready, SEND_CHAR, WAIT_LOOP;
        
    SEND_CHAR:
        // Send the character
        STORE [data_addr], c;
        RET;
}

// Print a null-terminated string
FUNCTION print_string(str: ptr<u8>) -> void {
    ENTRY:
        ptr = str;
        BR ALWAYS, CHECK_CHAR;
        
    CHECK_CHAR:
        c = LOAD [ptr];
        is_null = CMP_EQ c, 0;
        BR is_null, DONE, PRINT_CHAR;
        
    PRINT_CHAR:
        CALL putchar(c);
        ptr = ADD ptr, 1;
        BR ALWAYS, CHECK_CHAR;
        
    DONE:
        RET;
}

// Load kernel from disk
FUNCTION load_kernel() -> void {
    ENTRY:
        disk_base = LOAD_U64 DISK_BASE;
        
        // Calculate register addresses
        sector_addr = ADD disk_base, DISK_SECTOR;
        count_addr = ADD disk_base, DISK_COUNT;
        command_addr = ADD disk_base, DISK_COMMAND;
        status_addr = ADD disk_base, DISK_STATUS;
        data_addr = ADD disk_base, DISK_DATA;
        
        // Set starting sector
        start_sector = LOAD_U32 KERNEL_START_SECTOR;
        STORE [sector_addr], start_sector;
        
        // Set sector count
        sector_count = LOAD_U32 KERNEL_SECTOR_COUNT;
        STORE [count_addr], sector_count;
        
        // Set destination address
        dest_ptr = CONVERT KERNEL_LOAD_ADDR, ptr<u8>;
        
        // Issue read command
        STORE [command_addr], DISK_COMMAND_READ;
        
    WAIT_LOOP:
        // Check if disk operation completed
        status = LOAD [status_addr];
        is_busy = AND status, DISK_STATUS_BUSY;
        busy = CMP_NE is_busy, 0;
        BR busy, WAIT_LOOP, READ_DATA;
        
    READ_DATA:
        // Check if there's data available
        is_ready = AND status, DISK_STATUS_READY;
        ready = CMP_NE is_ready, 0;
        BR ready, DATA_AVAILABLE, DONE;
        
    DATA_AVAILABLE:
        // Read data byte
        data = LOAD [data_addr];
        
        // Store to memory
        STORE [dest_ptr], data;
        
        // Increment destination pointer
        dest_ptr = ADD dest_ptr, 1;
        
        // Check for more data
        status = LOAD [status_addr];
        BR ALWAYS, WAIT_LOOP;
        
    DONE:
        RET;
}
```

## 2. GPIO Driver

This example implements a simple GPIO driver for bare-metal hardware access.

```
MODULE "gpio_driver";

// GPIO controller base address
CONSTANT GPIO_BASE: u64 = 0xF000_3000;

// Register offsets
CONSTANT GPIO_DATA: u8 = 0x00;   // Data register
CONSTANT GPIO_DIR: u8 = 0x04;    // Direction register (1=output, 0=input)
CONSTANT GPIO_PULL: u8 = 0x08;   // Pull-up/down register
CONSTANT GPIO_INT_EN: u8 = 0x0C; // Interrupt enable register
CONSTANT GPIO_INT_STAT: u8 = 0x10; // Interrupt status register
CONSTANT GPIO_INT_CLR: u8 = 0x14; // Interrupt clear register

// Direction values
CONSTANT GPIO_DIR_INPUT: u32 = 0;
CONSTANT GPIO_DIR_OUTPUT: u32 = 1;

// Pull values
CONSTANT GPIO_PULL_NONE: u32 = 0;
CONSTANT GPIO_PULL_UP: u32 = 1;
CONSTANT GPIO_PULL_DOWN: u32 = 2;

// Initialize GPIO pin
FUNCTION gpio_init(pin: u32, direction: u32, pull: u32) -> void {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        
        // Set direction register
        dir_addr = ADD gpio_base, GPIO_DIR;
        dir_val = LOAD [dir_addr];
        
        // Calculate bit position
        bit_pos = LOAD_U32 1;
        bit_pos = SHL bit_pos, pin;
        
        // Clear existing bit
        mask = NOT bit_pos;
        dir_val = AND dir_val, mask;
        
        // Set direction bit if output
        is_output = CMP_EQ direction, GPIO_DIR_OUTPUT;
        BR is_output, SET_OUTPUT, SET_INPUT;
        
    SET_OUTPUT:
        dir_val = OR dir_val, bit_pos;
        BR ALWAYS, SET_DIRECTION;
        
    SET_INPUT:
        // Direction bit already cleared
        BR ALWAYS, SET_DIRECTION;
        
    SET_DIRECTION:
        // Write updated direction register
        STORE [dir_addr], dir_val;
        
        // Set pull-up/down register
        pull_addr = ADD gpio_base, GPIO_PULL;
        pull_val = LOAD [pull_addr];
        
        // Each pin has 2 bits for pull config, calculate position
        pull_shift = MUL pin, 2;
        pull_mask = LOAD_U32 3;
        pull_mask = SHL pull_mask, pull_shift;
        
        // Clear existing pull bits
        pull_val = AND pull_val, NOT pull_mask;
        
        // Set new pull bits
        pull_bits = SHL pull, pull_shift;
        pull_val = OR pull_val, pull_bits;
        
        // Write updated pull register
        STORE [pull_addr], pull_val;
        
        RET;
}

// Set GPIO pin output value
FUNCTION gpio_write(pin: u32, value: u32) -> void {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        data_addr = ADD gpio_base, GPIO_DATA;
        
        // Read current value
        data_val = LOAD [data_addr];
        
        // Calculate bit position
        bit_pos = LOAD_U32 1;
        bit_pos = SHL bit_pos, pin;
        
        // Clear existing bit
        mask = NOT bit_pos;
        data_val = AND data_val, mask;
        
        // Set bit if value is high
        is_high = CMP_NE value, 0;
        BR is_high, SET_HIGH, SET_LOW;
        
    SET_HIGH:
        data_val = OR data_val, bit_pos;
        BR ALWAYS, WRITE_DATA;
        
    SET_LOW:
        // Bit already cleared
        BR ALWAYS, WRITE_DATA;
        
    WRITE_DATA:
        // Write updated data register
        STORE [data_addr], data_val;
        RET;
}

// Read GPIO pin input value
FUNCTION gpio_read(pin: u32) -> u32 {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        data_addr = ADD gpio_base, GPIO_DATA;
        
        // Read current value
        data_val = LOAD [data_addr];
        
        // Calculate bit position
        bit_pos = LOAD_U32 1;
        bit_pos = SHL bit_pos, pin;
        
        // Extract bit
        bit_val = AND data_val, bit_pos;
        
        // Return 1 if bit is set, 0 otherwise
        is_set = CMP_NE bit_val, 0;
        BR is_set, RETURN_ONE, RETURN_ZERO;
        
    RETURN_ONE:
        result = LOAD_U32 1;
        RET result;
        
    RETURN_ZERO:
        result = LOAD_U32 0;
        RET result;
}

// Configure interrupt for GPIO pin
FUNCTION gpio_interrupt_enable(pin: u32) -> void {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        int_addr = ADD gpio_base, GPIO_INT_EN;
        
        // Read current value
        int_val = LOAD [int_addr];
        
        // Calculate bit position
        bit_pos = LOAD_U32 1;
        bit_pos = SHL bit_pos, pin;
        
        // Set interrupt bit
        int_val = OR int_val, bit_pos;
        
        // Write updated interrupt register
        STORE [int_addr], int_val;
        RET;
}

// Clear interrupt for GPIO pin
FUNCTION gpio_interrupt_clear(pin: u32) -> void {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        clr_addr = ADD gpio_base, GPIO_INT_CLR;
        
        // Calculate bit position
        bit_pos = LOAD_U32 1;
        bit_pos = SHL bit_pos, pin;
        
        // Write to clear register
        STORE [clr_addr], bit_pos;
        RET;
}

// GPIO interrupt handler
FUNCTION gpio_interrupt_handler() -> void {
    ENTRY:
        gpio_base = LOAD_U64 GPIO_BASE;
        stat_addr = ADD gpio_base, GPIO_INT_STAT;
        
        // Read interrupt status
        int_status = LOAD [stat_addr];
        
        // Handle each interrupted pin
        // For brevity, this example only handles pin 0
        pin0_mask = LOAD_U32 1;
        pin0_int = AND int_status, pin0_mask;
        is_pin0 = CMP_NE pin0_int, 0;
        BR is_pin0, HANDLE_PIN0, CHECK_NEXT;
        
    HANDLE_PIN0:
        // Handle pin 0 interrupt
        CALL pin0_handler();
        CALL gpio_interrupt_clear(0);
        
    CHECK_NEXT:
        // Check other pins...
        
        RET;
}

// Pin 0 interrupt handler (application specific)
FUNCTION pin0_handler() -> void {
    ENTRY:
        // Toggle an output pin in response
        val = CALL gpio_read(5);
        new_val = XOR val, 1;
        CALL gpio_write(5, new_val);
        RET;
}
```

## 3. Real-Time Scheduler

This example implements a basic real-time task scheduler for bare-metal environments.

```
MODULE "rtos_scheduler";

// Maximum number of tasks
CONSTANT MAX_TASKS: i32 = 16;

// Task states
CONSTANT TASK_STATE_IDLE: i32 = 0;
CONSTANT TASK_STATE_READY: i32 = 1;
CONSTANT TASK_STATE_RUNNING: i32 = 2;
CONSTANT TASK_STATE_BLOCKED: i32 = 3;

// Task priorities
CONSTANT PRIORITY_HIGH: i32 = 0;
CONSTANT PRIORITY_MEDIUM: i32 = 1;
CONSTANT PRIORITY_LOW: i32 = 2;

// Task context structure (architecture specific for register saving)
TYPE task_context {
    r0: u32,
    r1: u32,
    r2: u32,
    r3: u32,
    r4: u32,
    r5: u32,
    r6: u32,
    r7: u32,
    r8: u32,
    r9: u32,
    r10: u32,
    r11: u32,
    r12: u32,
    sp: u32,
    lr: u32,
    pc: u32,
    psr: u32
}

// Task control block
TYPE task_tcb {
    id: i32,
    state: i32,
    priority: i32,
    stack_pointer: ptr<u8>,
    stack_size: i32,
    context: task_context,
    wake_time: i32,
    function: ptr<function() -> void>
}

// Task array
GLOBAL tasks: array<task_tcb, MAX_TASKS>;
GLOBAL task_count: i32 = 0;
GLOBAL current_task: i32 = -1;
GLOBAL system_time: i32 = 0;

// Initialize the scheduler
FUNCTION scheduler_init() -> void {
    ENTRY:
        // Reset task count
        STORE task_count, 0;
        
        // Initialize tasks array
        i = LOAD_I32 0;
        BR ALWAYS, INIT_CHECK;
        
    INIT_CHECK:
        max = LOAD_I32 MAX_TASKS;
        is_done = CMP_GE i, max;
        BR is_done, INIT_DONE, INIT_TASK;
        
    INIT_TASK:
        // Get pointer to task
        idx = MUL i, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Initialize task fields
        id_ptr = ADD task_ptr, OFFSETOF(task_tcb, id);
        STORE [id_ptr], i;
        
        state_ptr = ADD task_ptr, OFFSETOF(task_tcb, state);
        STORE [state_ptr], TASK_STATE_IDLE;
        
        // Increment counter
        i = ADD i, 1;
        BR ALWAYS, INIT_CHECK;
        
    INIT_DONE:
        // Initialize system timer
        CALL timer_init();
        RET;
}

// Create a new task
FUNCTION create_task(function: ptr<function() -> void>, 
                    stack: ptr<u8>, 
                    stack_size: i32,
                    priority: i32) -> i32 {
    ENTRY:
        // Check if we've reached maximum tasks
        count = LOAD task_count;
        max = LOAD_I32 MAX_TASKS;
        is_full = CMP_GE count, max;
        BR is_full, CREATE_FAIL, CREATE_TASK;
        
    CREATE_FAIL:
        result = LOAD_I32 -1;
        RET result;
        
    CREATE_TASK:
        // Get new task ID
        new_id = count;
        
        // Get pointer to task
        idx = MUL new_id, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Set task fields
        id_ptr = ADD task_ptr, OFFSETOF(task_tcb, id);
        STORE [id_ptr], new_id;
        
        state_ptr = ADD task_ptr, OFFSETOF(task_tcb, state);
        STORE [state_ptr], TASK_STATE_READY;
        
        priority_ptr = ADD task_ptr, OFFSETOF(task_tcb, priority);
        STORE [priority_ptr], priority;
        
        stack_ptr_ptr = ADD task_ptr, OFFSETOF(task_tcb, stack_pointer);
        STORE [stack_ptr_ptr], stack;
        
        stack_size_ptr = ADD task_ptr, OFFSETOF(task_tcb, stack_size);
        STORE [stack_size_ptr], stack_size;
        
        func_ptr = ADD task_ptr, OFFSETOF(task_tcb, function);
        STORE [func_ptr], function;
        
        // Initialize task context
        context_ptr = ADD task_ptr, OFFSETOF(task_tcb, context);
        CALL init_task_context(context_ptr, stack, stack_size, function);
        
        // Increment task count
        new_count = ADD count, 1;
        STORE task_count, new_count;
        
        RET new_id;
}

// Initialize task context with initial values
FUNCTION init_task_context(context: ptr<task_context>,
                         stack: ptr<u8>,
                         stack_size: i32,
                         function: ptr<function() -> void>) -> void {
    ENTRY:
        // Calculate initial stack pointer (top of stack)
        sp = ADD stack, stack_size;
        sp = SUB sp, 4;  // 4-byte aligned
        
        // Initialize context with default values
        psr_ptr = ADD context, OFFSETOF(task_context, psr);
        STORE [psr_ptr], 0x01000000;  // Thumb state
        
        pc_ptr = ADD context, OFFSETOF(task_context, pc);
        STORE [pc_ptr], function;
        
        sp_ptr = ADD context, OFFSETOF(task_context, sp);
        STORE [sp_ptr], sp;
        
        // Other registers can be zero
        for_i = LOAD_I32 0;
        BR ALWAYS, REG_INIT_CHECK;
        
    REG_INIT_CHECK:
        is_done = CMP_GE for_i, 13;  // r0-r12
        BR is_done, REG_INIT_DONE, REG_INIT_LOOP;
        
    REG_INIT_LOOP:
        // Calculate register offset
        offset = MUL for_i, 4;  // 4 bytes per register
        reg_ptr = ADD context, offset;
        
        // Set register to zero
        STORE [reg_ptr], 0;
        
        // Next register
        for_i = ADD for_i, 1;
        BR ALWAYS, REG_INIT_CHECK;
        
    REG_INIT_DONE:
        RET;
}

// Start the scheduler
FUNCTION scheduler_start() -> void {
    ENTRY:
        // Find first task to run
        CALL find_next_task();
        
        // If no tasks, just loop
        next_task = LOAD current_task;
        has_tasks = CMP_GE next_task, 0;
        BR has_tasks, START_TASK, IDLE_LOOP;
        
    IDLE_LOOP:
        // No tasks to run, just idle
        BR ALWAYS, IDLE_LOOP;
        
    START_TASK:
        // Load the first task's context
        CALL load_task_context(next_task);
        
        // Never returns - context switch happens
        BR ALWAYS, IDLE_LOOP;  // Shouldn't reach here
}

// Find the next task to run
FUNCTION find_next_task() -> void {
    ENTRY:
        // Start with highest priority (0)
        priority = LOAD_I32 0;
        BR ALWAYS, PRIORITY_CHECK;
        
    PRIORITY_CHECK:
        max_priority = LOAD_I32 3;  // 0, 1, 2 are valid priorities
        is_done = CMP_GE priority, max_priority;
        BR is_done, NO_TASK, CHECK_PRIORITY;
        
    CHECK_PRIORITY:
        // Check all tasks at this priority
        task_idx = LOAD_I32 0;
        BR ALWAYS, TASK_CHECK;
        
    TASK_CHECK:
        count = LOAD task_count;
        is_done = CMP_GE task_idx, count;
        BR is_done, NEXT_PRIORITY, CHECK_TASK;
        
    CHECK_TASK:
        // Get pointer to task
        idx = MUL task_idx, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Check task state and priority
        state_ptr = ADD task_ptr, OFFSETOF(task_tcb, state);
        state = LOAD [state_ptr];
        
        priority_ptr = ADD task_ptr, OFFSETOF(task_tcb, priority);
        task_priority = LOAD [priority_ptr];
        
        is_ready = CMP_EQ state, TASK_STATE_READY;
        is_right_priority = CMP_EQ task_priority, priority;
        
        is_runnable = AND is_ready, is_right_priority;
        runnable = CMP_NE is_runnable, 0;
        
        BR runnable, FOUND_TASK, NEXT_TASK;
        
    FOUND_TASK:
        // Set as current task
        STORE current_task, task_idx;
        
        // Update task state
        STORE [state_ptr], TASK_STATE_RUNNING;
        
        RET;
        
    NEXT_TASK:
        // Check next task
        task_idx = ADD task_idx, 1;
        BR ALWAYS, TASK_CHECK;
        
    NEXT_PRIORITY:
        // Check next priority level
        priority = ADD priority, 1;
        BR ALWAYS, PRIORITY_CHECK;
        
    NO_TASK:
        // No task found, set invalid
        STORE current_task, -1;
        RET;
}

// Timer interrupt handler - called by hardware
FUNCTION timer_handler() -> void {
    ENTRY:
        // Save current task context if a task is running
        curr = LOAD current_task;
        is_running = CMP_GE curr, 0;
        BR is_running, SAVE_CONTEXT, UPDATE_TIME;
        
    SAVE_CONTEXT:
        CALL save_task_context(curr);
        
    UPDATE_TIME:
        // Update system time
        time = LOAD system_time;
        time = ADD time, 1;
        STORE system_time, time;
        
        // Check if any tasks need to wake up
        CALL check_wake_tasks();
        
        // Find next task to run
        CALL find_next_task();
        
        // If a task is found, switch to it
        next = LOAD current_task;
        has_next = CMP_GE next, 0;
        BR has_next, SWITCH_TASK, RETURN;
        
    SWITCH_TASK:
        // Load next task's context
        CALL load_task_context(next);
        
    RETURN:
        // Return from interrupt
        RET;
}

// Save current task context (architecture specific)
FUNCTION save_task_context(task_id: i32) -> void {
    ENTRY:
        // Implementation depends on architecture
        // This would save all registers to the task's context
        
        // Get pointer to task
        idx = MUL task_id, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Get context pointer
        context_ptr = ADD task_ptr, OFFSETOF(task_tcb, context);
        
        // Save registers to context (architecture specific)
        // This is simplified - real implementation would use assembly
        STORE [context_ptr], r0;
        r1_ptr = ADD context_ptr, OFFSETOF(task_context, r1);
        STORE [r1_ptr], r1;
        // ... and so on for other registers
        
        RET;
}

// Load task context and switch to it (architecture specific)
FUNCTION load_task_context(task_id: i32) -> void {
    ENTRY:
        // Implementation depends on architecture
        // This would load all registers from the task's context
        
        // Get pointer to task
        idx = MUL task_id, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Get context pointer
        context_ptr = ADD task_ptr, OFFSETOF(task_tcb, context);
        
        // Load registers from context (architecture specific)
        // This is simplified - real implementation would use assembly
        r0 = LOAD [context_ptr];
        r1_ptr = ADD context_ptr, OFFSETOF(task_context, r1);
        r1 = LOAD [r1_ptr];
        // ... and so on for other registers
        
        // Note: In a real implementation, this function would not return
        // Instead, it would directly switch to the task context
        
        RET;
}

// Sleep the current task for a specified time
FUNCTION sleep(ms: i32) -> void {
    ENTRY:
        // Get current task
        task_id = LOAD current_task;
        
        // Get pointer to task
        idx = MUL task_id, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Set task state to blocked
        state_ptr = ADD task_ptr, OFFSETOF(task_tcb, state);
        STORE [state_ptr], TASK_STATE_BLOCKED;
        
        // Set wake time
        current_time = LOAD system_time;
        wake_time = ADD current_time, ms;
        
        wake_time_ptr = ADD task_ptr, OFFSETOF(task_tcb, wake_time);
        STORE [wake_time_ptr], wake_time;
        
        // Trigger scheduler to switch tasks
        CALL yield();
        
        RET;
}

// Check if any blocked tasks need to wake up
FUNCTION check_wake_tasks() -> void {
    ENTRY:
        current_time = LOAD system_time;
        
        // Check all tasks
        task_idx = LOAD_I32 0;
        BR ALWAYS, WAKE_CHECK;
        
    WAKE_CHECK:
        count = LOAD task_count;
        is_done = CMP_GE task_idx, count;
        BR is_done, WAKE_DONE, CHECK_TASK;
        
    CHECK_TASK:
        // Get pointer to task
        idx = MUL task_idx, SIZEOF(task_tcb);
        task_ptr = LEA tasks;
        task_ptr = ADD task_ptr, idx;
        
        // Check if task is blocked
        state_ptr = ADD task_ptr, OFFSETOF(task_tcb, state);
        state = LOAD [state_ptr];
        
        is_blocked = CMP_EQ state, TASK_STATE_BLOCKED;
        BR is_blocked, CHECK_WAKE_TIME, NEXT_TASK;
        
    CHECK_WAKE_TIME:
        // Get wake time
        wake_time_ptr = ADD task_ptr, OFFSETOF(task_tcb, wake_time);
        wake_time = LOAD [wake_time_ptr];
        
        // Check if it's time to wake up
        should_wake = CMP_GE current_time, wake_time;
        BR should_wake, WAKE_TASK, NEXT_TASK;
        
    WAKE_TASK:
        // Set task to ready state
        STORE [state_ptr], TASK_STATE_READY;
        
    NEXT_TASK:
        // Check next task
        task_idx = ADD task_idx, 1;
        BR ALWAYS, WAKE_CHECK;
        
    WAKE_DONE:
        RET;
}

// Yield current task (trigger a context switch)
FUNCTION yield() -> void {
    ENTRY:
        // In a real implementation, this would trigger a software interrupt
        // or directly call the scheduler
        
        // For this example, we'll simulate it by calling timer_handler
        CALL timer_handler();
        
        RET;
}

// Initialize system timer
FUNCTION timer_init() -> void {
    ENTRY:
        // Architecture-specific timer initialization
        // This would configure a hardware timer to trigger interrupts
        
        // Example for a memory-mapped timer
        CONSTANT TIMER_BASE: u64 = 0xF000_4000;
        CONSTANT TIMER_CONTROL: u8 = 0x00;
        CONSTANT TIMER_LOAD: u8 = 0x04;
        CONSTANT TIMER_ENABLE: u32 = 0x01;
        CONSTANT TIMER_INTERRUPT: u32 = 0x02;
        
        timer_base = LOAD_U64 TIMER_BASE;
        
        // Set timer period (1ms)
        load_addr = ADD timer_base, TIMER_LOAD;
        STORE [load_addr], 1000;  // Assuming 1MHz clock
        
        // Enable timer with interrupts
        control_addr = ADD timer_base, TIMER_CONTROL;
        control_val = OR TIMER_ENABLE, TIMER_INTERRUPT;
        STORE [control_addr], control_val;
        
        RET;
}
```

## 4. Memory Manager

This example implements a simple memory manager for bare-metal environments.

```
MODULE "memory_manager";

// Memory layout constants
CONSTANT HEAP_START: u64 = 0x2000_0000;
CONSTANT HEAP_SIZE: u64 = 0x0010_0000;  // 64KB

// Memory block header
TYPE mem_block {
    size: u32,        // Size of data area (not including header)
    next: ptr<mem_block>,  // Next block in list
    free: u8,         // 1 if block is free, 0 if used
    reserved: u8,     // Reserved for alignment
    checksum: u16     // Simple integrity check
}

// Initialization state
GLOBAL heap_initialized: u8 = 0;
GLOBAL heap_start: ptr<mem_block> = NULL;

// Initialize memory manager
FUNCTION memory_init() -> void {
    ENTRY:
        // Check if already initialized
        initialized = LOAD heap_initialized;
        is_init = CMP_NE initialized, 0;
        BR is_init, INIT_DONE, INIT_HEAP;
        
    INIT_HEAP:
        // Create initial free block covering the entire heap
        heap_ptr = CONVERT HEAP_START, ptr<mem_block>;
        
        // Calculate usable size (heap size minus header size)
        header_size = SIZEOF(mem_block);
        usable_size = SUB HEAP_SIZE, header_size;
        
        // Initialize block
        size_ptr = ADD heap_ptr, OFFSETOF(mem_block, size);
        STORE [size_ptr], usable_size;
        
        next_ptr = ADD heap_ptr, OFFSETOF(mem_block, next);
        STORE [next_ptr], NULL;
        
        free_ptr = ADD heap_ptr, OFFSETOF(mem_block, free);
        STORE [free_ptr], 1;
        
        // Calculate checksum: simple xor of size and free flag
        checksum = XOR usable_size, 1;
        checksum_ptr = ADD heap_ptr, OFFSETOF(mem_block, checksum);
        STORE [checksum_ptr], checksum;
        
        // Set global variables
        STORE heap_start, heap_ptr;
        STORE heap_initialized, 1;
        
    INIT_DONE:
        RET;
}

// Allocate memory
FUNCTION memory_alloc(size: u32) -> ptr<void> {
    ENTRY:
        // Check if initialized
        initialized = LOAD heap_initialized;
        is_init = CMP_NE initialized, 0;
        BR is_init, START_ALLOC, INIT_FIRST;
        
    INIT_FIRST:
        CALL memory_init();
        
    START_ALLOC:
        // Round size up to alignment (4 bytes)
        aligned_size = ADD size, 3;
        aligned_size = AND aligned_size, 0xFFFFFFFC;
        
        // Current block to check
        current = LOAD heap_start;
        BR ALWAYS, SEARCH_LOOP;
        
    SEARCH_LOOP:
        // Check if we reached the end
        is_null = CMP_EQ current, NULL;
        BR is_null, ALLOC_FAIL, CHECK_BLOCK;
        
    CHECK_BLOCK:
        // Check if block is free
        free_ptr = ADD current, OFFSETOF(mem_block, free);
        is_free = LOAD [free_ptr];
        
        // Check if block is large enough
        size_ptr = ADD current, OFFSETOF(mem_block, size);
        block_size = LOAD [size_ptr];
        
        is_large_enough = CMP_GE block_size, aligned_size;
        
        // Block must be free and large enough
        cond = AND is_free, is_large_enough;
        suitable = CMP_NE cond, 0;
        
        BR suitable, FOUND_BLOCK, NEXT_BLOCK;
        
    NEXT_BLOCK:
        // Move to next block
        next_ptr = ADD current, OFFSETOF(mem_block, next);
        current = LOAD [next_ptr];
        BR ALWAYS, SEARCH_LOOP;
        
    FOUND_BLOCK:
        // Check if block should be split
        min_split_size = LOAD_U32 16;  // Minimum useful block size
        remaining = SUB block_size, aligned_size;
        should_split = CMP_GT remaining, min_split_size;
        
        BR should_split, SPLIT_BLOCK, USE_WHOLE_BLOCK;
        
    SPLIT_BLOCK:
        // Split block into allocated part and free remainder
        
        // Calculate address of the new block
        header_size = SIZEOF(mem_block);
        new_block = ADD current, header_size;
        new_block = ADD new_block, aligned_size;
        
        // Set size of new block
        new_size = SUB block_size, aligned_size;
        new_size = SUB new_size, header_size;
        
        new_size_ptr = ADD new_block, OFFSETOF(mem_block, size);
        STORE [new_size_ptr], new_size;
        
        // Set next pointer of new block
        current_next_ptr = ADD current, OFFSETOF(mem_block, next);
        current_next = LOAD [current_next_ptr];
        
        new_next_ptr = ADD new_block, OFFSETOF(mem_block, next);
        STORE [new_next_ptr], current_next;
        
        // Mark new block as free
        new_free_ptr = ADD new_block, OFFSETOF(mem_block, free);
        STORE [new_free_ptr], 1;
        
        // Calculate checksum for new block
        new_checksum = XOR new_size, 1;
        new_checksum_ptr = ADD new_block, OFFSETOF(mem_block, checksum);
        STORE [new_checksum_ptr], new_checksum;
        
        // Update current block
        STORE [size_ptr], aligned_size;
        STORE [current_next_ptr], new_block;
        
    USE_WHOLE_BLOCK:
        // Mark block as used
        STORE [free_ptr], 0;
        
        // Update checksum
        checksum = XOR block_size, 0;
        checksum_ptr = ADD current, OFFSETOF(mem_block, checksum);
        STORE [checksum_ptr], checksum;
        
        // Calculate data pointer
        data_ptr = ADD current, header_size;
        RET data_ptr;
        
    ALLOC_FAIL:
        // Return null if allocation failed
        RET NULL;
}

// Free memory
FUNCTION memory_free(ptr: ptr<void>) -> void {
    ENTRY:
        // Null check
        is_null = CMP_EQ ptr, NULL;
        BR is_null, FREE_DONE, FREE_BLOCK;
        
    FREE_BLOCK:
        // Calculate block header address
        header_size = SIZEOF(mem_block);
        block_ptr = SUB ptr, header_size;
        
        // Verify this is a valid block (simple bounds check)
        heap_start_addr = CONVERT HEAP_START, ptr<void>;
        heap_end_addr = ADD heap_start_addr, HEAP_SIZE;
        
        is_below_start = CMP_LT block_ptr, heap_start_addr;
        is_above_end = CMP_GE ptr, heap_end_addr;
        is_invalid = OR is_below_start, is_above_end;
        
        BR is_invalid, FREE_DONE, MARK_FREE;
        
    MARK_FREE:
        // Mark block as free
        free_ptr = ADD block_ptr, OFFSETOF(mem_block, free);
        STORE [free_ptr], 1;
        
        // Update checksum
        size_ptr = ADD block_ptr, OFFSETOF(mem_block, size);
        size = LOAD [size_ptr];
        checksum = XOR size, 1;
        
        checksum_ptr = ADD block_ptr, OFFSETOF(mem_block, checksum);
        STORE [checksum_ptr], checksum;
        
        // Coalesce adjacent free blocks
        CALL coalesce_free_blocks();
        
    FREE_DONE:
        RET;
}

// Coalesce adjacent free blocks
FUNCTION coalesce_free_blocks() -> void {
    ENTRY:
        // Start from heap start
        current = LOAD heap_start;
        BR ALWAYS, COALESCE_CHECK;
        
    COALESCE_CHECK:
        // Check if we reached the end
        is_null = CMP_EQ current, NULL;
        BR is_null, COALESCE_DONE, CHECK_CURRENT;
        
    CHECK_CURRENT:
        // Check if current block is free
        free_ptr = ADD current, OFFSETOF(mem_block, free);
        is_current_free = LOAD [free_ptr];
        
        BR is_current_free, CHECK_NEXT, NEXT_BLOCK;
        
    CHECK_NEXT:
        // Get next block
        next_ptr = ADD current, OFFSETOF(mem_block, next);
        next = LOAD [next_ptr];
        
        // Check if there is a next block
        is_next_null = CMP_EQ next, NULL;
        BR is_next_null, NEXT_BLOCK, CHECK_NEXT_FREE;
        
    CHECK_NEXT_FREE:
        // Check if next block is free
        next_free_ptr = ADD next, OFFSETOF(mem_block, free);
        is_next_free = LOAD [next_free_ptr];
        
        BR is_next_free, MERGE_BLOCKS, NEXT_BLOCK;
        
    MERGE_BLOCKS:
        // Merge current and next blocks
        
        // Get sizes
        size_ptr = ADD current, OFFSETOF(mem_block, size);
        current_size = LOAD [size_ptr];
        
        next_size_ptr = ADD next, OFFSETOF(mem_block, size);
        next_size = LOAD [next_size_ptr];
        
        // Get next's next pointer
        next_next_ptr = ADD next, OFFSETOF(mem_block, next);
        next_next = LOAD [next_next_ptr];
        
        // Calculate total size (current + next + header)
        header_size = SIZEOF(mem_block);
        total_size = ADD current_size, next_size;
        total_size = ADD total_size, header_size;
        
        // Update current block
        STORE [size_ptr], total_size;
        STORE [next_ptr], next_next;
        
        // Update checksum
        checksum = XOR total_size, 1;
        checksum_ptr = ADD current, OFFSETOF(mem_block, checksum);
        STORE [checksum_ptr], checksum;
        
        // Continue from current block to catch multiple consecutive free blocks
        BR ALWAYS, CHECK_CURRENT;
        
    NEXT_BLOCK:
        // Move to next block
        next_ptr = ADD current, OFFSETOF(mem_block, next);
        current = LOAD [next_ptr];
        BR ALWAYS, COALESCE_CHECK;
        
    COALESCE_DONE:
        RET;
}

// Get total free memory
FUNCTION memory_get_free() -> u32 {
    ENTRY:
        // Check if initialized
        initialized = LOAD heap_initialized;
        is_init = CMP_NE initialized, 0;
        BR is_init, START_COUNT, INIT_FIRST;
        
    INIT_FIRST:
        CALL memory_init();
        
    START_COUNT:
        total_free = LOAD_U32 0;
        current = LOAD heap_start;
        BR ALWAYS, COUNT_LOOP;
        
    COUNT_LOOP:
        // Check if we reached the end
        is_null = CMP_EQ current, NULL;
        BR is_null, COUNT_DONE, CHECK_BLOCK;
        
    CHECK_BLOCK:
        // Check if block is free
        free_ptr = ADD current, OFFSETOF(mem_block, free);
        is_free = LOAD [free_ptr];
        
        BR is_free, ADD_SIZE, NEXT_BLOCK;
        
    ADD_SIZE:
        // Add block size to total
        size_ptr = ADD current, OFFSETOF(mem_block, size);
        size = LOAD [size_ptr];
        
        total_free = ADD total_free, size;
        
    NEXT_BLOCK:
        // Move to next block
        next_ptr = ADD current, OFFSETOF(mem_block, next);
        current = LOAD [next_ptr];
        BR ALWAYS, COUNT_LOOP;
        
    COUNT_DONE:
        RET total_free;
}
```

## 5. Conclusion

These examples demonstrate the power of COIL/HOIL for bare-metal programming scenarios, showcasing:

1. **Direct Hardware Access**: Memory-mapped I/O operations without OS abstraction
2. **Zero Runtime Dependencies**: All code functions without any runtime library
3. **Predictable Execution**: Deterministic behavior critical for real-time applications
4. **Resource Management**: Efficient handling of limited system resources
5. **Low-Level Control**: Complete control over hardware features and behavior

Bare-metal COIL/HOIL programming provides the foundation for operating systems, firmware, bootloaders, and other system-level software where direct hardware control and minimal abstraction are essential.