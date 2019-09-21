int_multiply:
	addi  sp,sp,-48  ; Increase the stack size for this function call
	sw  s0,44(sp)    ; Store the old frame pointer on the stack for later
	addi  s0,sp,48   ; Update the frame pointer
	sw  a0,-36(s0)   ; Store function argumetns a based on the frame pointer
	sw  a1,-32(s0)   ; Store function argumetns b based on the frame pointer
	sw  zero,-20(s0) ; Set sum = 0
	sw  zero,-24(s0) ; Set i=0
	j .L2            ; Jump to the start of the for loop
.L3: ; This is the body of the for loop
	lw  a4,-20(s0) ; Load sum
	lw  a5,-36(s0) ; Load a (the function argument)
	add a5,a4,a5   ; a4 = sum + a
	sw  a5,-20(s0) ; Store sum (a5)
	lw  a5,-24(s0) ; Load i
	addi  a5,a5,1  ; i++
	sw  a5,-24(s0) ; Store i
.L2:
	lw  a4,-24(s0) ; Load i
	lw  a5,-32(s0) ; Load b (the function argument)
	blt a4,a5,.L3  ; Compare i and b
	lw  a5,-20(s0) ; If this point is reached, we have exited the for loop.
	               ; Load sum into a register
	mv  a0,a5      ; Move sum into the return value register
	lw  s0,44(sp)  ; Restore old frame pointer
	addi  sp,sp,48 ; Restore old stack pointer
	jr  ra         ; Jump to the return address (set by Call)
	
main:
	addi  sp,sp,-16   ; Increase the stack size for this function call
	sw  ra,12(sp)     ; Store the old return address on the stack for later
	sw  s0,8(sp)      ; Store the old frame pointer on the stack for later
	addi  s0,sp,16    ; Update the frame pointer
	li  a0,-2          ; Set a argument for function call
	li  a1, 2         ; Set b argument for function call
	call  int_multiply ; Call int_multiply function with psudo instruction
	mv  a5,a0         ; Move the return value to a general purpouse register
	mv  a0,a5         ; Move it back to return the value again...
	                  ; All optimizations are turned off
	lw  ra,12(sp)     ; Restore old return address
	lw  s0,8(sp)      ; Restore old frame pointer
	addi  sp,sp,16    ; Restore old stack pointer
	jr  ra            ; Jump to old return address
