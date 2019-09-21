// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Top Level Module

module top (
  input clock,
  input reset,

  input [31:0] instruction,
  output [31:0] wb_data
);


/******************************************************************************
*                      Start Your Code Here
******************************************************************************/
wire wb_sel, wEn, mem_wEn, op_B_sel;
wire [4:0] read_sel1, read_sel2, write_sel;
wire [5:0] ALU_control;
wire [31:0] read_data, read_data1, read_data2, write_data, ALU_result, imm32, mux1_out, mux2_out;

decode decode_inst (
  .instruction(instruction),
  .wb_sel(wb_sel),
  .read_sel1(read_sel1),
  .read_sel2(read_sel2),
  .write_sel(write_sel),
  .wEn(wEn),
  .imm32(imm32),
  .op_B_sel(op_B_sel),
  .ALU_Control(ALU_control),
  .mem_wEn(mem_wEn)
);

regFile regFile_inst (
  .clock(clock),
  .reset(reset),
  .wEn(wEn), // Write Enable
  .write_data(mux2_out),
  .read_sel1(read_sel1),
  .read_sel2(read_sel2),
  .write_sel(write_sel),
  .read_data1(read_data1),
  .read_data2(read_data2)
);

assign mux1_out = op_B_sel ? imm32 : read_data2;

ALU alu_inst(
  .ALU_Control(ALU_control),
  .operand_A(read_data1),
  .operand_B(mux1_out),
  .ALU_result(ALU_result)
);

memory mem_inst (
  .clock(clock),
  .wEn(mem_wEn),
  .address(ALU_result[16:0]),
  .write_data(read_data2),
  .read_data(read_data)
);

assign mux2_out = wb_sel ? read_data : ALU_result;
assign wb_data = mux2_out;

endmodule
