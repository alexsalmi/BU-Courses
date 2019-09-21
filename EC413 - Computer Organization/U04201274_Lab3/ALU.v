// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: ALU

module ALU (
  input [5:0]  ALU_Control,
  input [31:0] operand_A,
  input [31:0] operand_B,
  output [31:0] ALU_result
);

/******************************************************************************
*                      Start Your Code Here
******************************************************************************/
wire signed [31:0] s_op_a, s_op_b, s_alu_result;

assign s_op_a = operand_A;
assign s_op_b = operand_B;

assign s_alu_result = s_op_a < s_op_b;

assign ALU_result = ALU_Control == 6'b000000 ? operand_A + operand_B :
						  ALU_Control == 6'b001000 ? operand_A - operand_B :
						  ALU_Control == 6'b000010 ? s_alu_result :
						  ALU_Control == 6'b000100 ? operand_A ^ operand_B :
						  ALU_Control == 6'b000111 ? operand_A & operand_B :
						  0;

endmodule

