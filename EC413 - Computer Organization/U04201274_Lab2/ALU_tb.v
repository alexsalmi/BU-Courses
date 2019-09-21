// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 2 Problem 2: ALU Test Bench

module ALU_tb();

reg [5:0] ctrl;
reg [31:0] opA, opB;

wire [31:0] result;

ALU dut (
  .ALU_Control(ctrl),
  .operand_A(opA),
  .operand_B(opB),
  .ALU_result(result)
);

initial begin
  ctrl = 6'b000000;
  opA = 4;
  opB = 5;
  #10
  $display("ALU Result 4 + 5: %d",result);
  
  #10
  ctrl = 6'b000010;
  opA = 4;
  opB = 5;
  #10
  $display("ALU Result 4 < 5: %d",result);
  
  #10
  ctrl = 6'b000010;
  opA = 4;
  opB = 32'hffffffff;
  #10
  $display("ALU Result 4 < -1: %d",result);
  
  // Add other test cases here
  
  #10
  ctrl = 6'b000100;
  opA = 5;
  opB = 6;
  #10
  $display("ALU Result 5 (0101) XOR 6 (0110): %d",result);
  
  #10
  ctrl = 6'b000111;
  opA = 5;
  opB = 6;
  #10
  $display("ALU Result 5 (0101) AND 6 (0110): %d",result);
  
  #10
  ctrl = 6'b001000;
  opA = 10;
  opB = 5;
  #10
  $display("ALU Result 10 - 5: %d",result);

end

endmodule
