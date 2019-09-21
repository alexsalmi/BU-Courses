// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 2 Problem 2: ALU

module ALU (
  input [5:0]  ALU_Control,
  input [31:0] operand_A,
  input [31:0] operand_B,
  output [31:0] ALU_result
);

/******************************************************************************
*                      Start Your Code Here
******************************************************************************/
reg [31:0] result;

always@(*)begin
	case(ALU_Control)
		6'b000000: result <= (operand_A + operand_B); // ADD & ADDI
		6'b001000: result <= (operand_A - operand_B); // SUB
		6'b000010: result <= ($signed(operand_A) < $signed(operand_B)); // SLT
		6'b000100: result <= (operand_A ^ operand_B); // XOR
		6'b000111: result <= (operand_A & operand_B); // AND
	endcase
end //always

assign ALU_result = result;

endmodule

