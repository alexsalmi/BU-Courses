fib_r:
	addi  sp,sp,-28   ; Increase the stack size for this function call
	sw  s0,24(sp)      ; Store the old frame pointer on the stack for later
	addi  s0,sp,28    ; Update the frame pointer
	sw ra,-24(s0)    ; Stored return address
	sw  a0,-8(s0)   ; Store function argument n based on the frame pointer
	lw a2,-8(s0)    ; Load n
	addi a1,zero,2	; i = 2
	sw a1,-4(s0)	; store i
	lw a1,-4(s0)	; load i
	blt a2,a1,.IF	 ; If n<=1
.ELSE:
	addi a2,a2,-1	;subtract 1 from n
	mv  a0,a2        ; Set n argument for function call
	call  fib_r      ; Call fib_r function with psudo instruction
	mv  a3,a0         ; Move the return value to a general purpouse register
	sw a3,-12(s0)	; store return value
	lw a2,-8(s0)    ; Load original n
	addi a2,a2,-2   ; subtract 2 from n
	mv  a0,a2        ; Set n argument for function call
	call  fib_r      ; Call fib_n function with psudo instruction
	mv  a4,a0         ; Move the return value to a general purpouse register
	sw a4,-16(s0)	; store return value 
	lw a3,-12(s0)   ; load first return value
	lw a4,-16(s0)	;load second return value
	add a2,a4,a3      ;fib_r(n-1) + fib_r(n-2) into return register
	j .FIB_END
.IF: ;This is our base case, return n
	lw  a0,-8(s0) ; Load n into return register
.FIB_END:
	addi sp,sp,28   ; Restore old stack pointer
	lw ra,-24(s0)   ; Restore old return address
	lw s0,-4(s0)	; Restore old frame pointer
	jr ra

main:
	addi  sp,sp,-16   ; Increase the stack size for this function call
	sw  ra,12(sp)     ; Store the old return address on the stack for later
	sw  s0,8(sp)      ; Store the old frame pointer on the stack for later
	addi  s0,sp,16    ; Update the frame pointer

	li  a0,1          ; Set n argument for function call
	call  fib_r      ; Call fib_n function with psudo instruction
	mv  t3,a0         ; Move the return value to a general purpouse register

	li  a0,3          ; Set n argument for function call
	call  fib_r      ; Call fib_n function with psudo instruction
	mv  t4,a0         ; Move the return value to a general purpouse register

	li  a0,8          ; Set n argument for function call
	call  fib_r      ; Call fib_n function with psudo instruction
	mv  t5,a0         ; Move the return value to a general purpouse register

	li a0, 0
	                  ; All optimizations are turned off
	lw  ra,12(sp)     ; Restore old return address
	lw  s0,8(sp)      ; Restore old frame pointer
	addi  sp,sp,16    ; Restore old stack pointer
	jr  ra            ; Jump to old return address
