fib_nr:
	addi  sp,sp,-48  ; Increase the stack size for this function call
	sw  s0,44(sp)    ; Store the old frame pointer on the stack for later
	addi  s0,sp,48   ; Update the frame pointer
	sw  a0,-36(s0)   ; Store function argument n based on the frame pointer
	sw  zero,-20(s0) ; Set term1 = 0
	addi a1, zero, 1
	sw  a1,-24(s0)   ; Set term2 = 1
	sw a1, -32(s0)   ; Set i = 1
	j .L2            ; Jump to the start of the for loop
.L3: ; This is the body of the for loop
	lw  a4,-20(s0) ; Load term1
	lw  a5,-24(s0) ; Load term2
	add a6, a4, a5 ; term1+term2
	sw a5, -20(s0) ; Store term2 to term1
	sw a6, -24(s0) ; Store new term to term2
	lw  a5,-32(s0) ; Load i
	addi  a5,a5,1  ; i++
	sw  a5,-32(s0) ; Store i
.L2:
	lw  a4,-32(s0) ; Load i
	lw  a5,-36(s0) ; Load n (the function argument)
	blt a4,a5,.L3  ; Compare i and n
	lw  a5,-24(s0) ; If this point is reached, we have exited the for loop.
	               ; Load term2 into a register
	mv  a0,a5      ; Move term2 into the return value register
	lw  s0,44(sp)  ; Restore old frame pointer
	addi  sp,sp,48 ; Restore old stack pointer
	jr  ra         ; Jump to the return address (set by Call)
	
main:
	addi  sp,sp,-16   ; Increase the stack size for this function call
	sw  ra,12(sp)     ; Store the old return address on the stack for later
	sw  s0,8(sp)      ; Store the old frame pointer on the stack for later
	addi  s0,sp,16    ; Update the frame pointer

	li  a0,1          ; Set n argument for function call
	call  fib_nr      ; Call fib_n function with psudo instruction
	mv  t3,a0         ; Move the return value to a general purpouse register

	li  a0,3          ; Set n argument for function call
	call  fib_nr      ; Call fib_n function with psudo instruction
	mv  t4,a0         ; Move the return value to a general purpouse register

	li  a0,8          ; Set n argument for function call
	call  fib_nr      ; Call fib_n function with psudo instruction
	mv  t5,a0         ; Move the return value to a general purpouse register

	li  a0,17         ; Set n argument for function call
	call  fib_nr      ; Call fib_n function with psudo instruction
	mv  t6,a0         ; Move the return value to a general purpouse register

	li a0, 0
	                  ; All optimizations are turned off
	lw  ra,12(sp)     ; Restore old return address
	lw  s0,8(sp)      ; Restore old frame pointer
	addi  sp,sp,16    ; Restore old stack pointer
	jr  ra            ; Jump to old return address
