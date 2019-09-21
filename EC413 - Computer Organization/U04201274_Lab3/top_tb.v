// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Top Level Module Test Bench

module top_tb();

parameter NOP = 32'b000000000000_00000_000_00000_0010011; // addi zero, zero, 0

reg clock;
reg reset;
reg [31:0] instruction;

wire [31:0] result;

integer x;

top dut (
  .clock(clock),
  .reset(reset),
  .instruction(instruction),
  .wb_data(result)
);

always #5 clock = ~clock;

task print_state;
  begin
    $display("Time:\t%0d", $time);
    for( x=0; x<32; x=x+1) begin
      $display("Register %d: %h", x, dut.regFile_inst.reg_file[x]);
    end
    $display("--------------------------------------------------------------------------------");
    $display("\n\n");
  end
endtask


initial begin
  clock = 1'b1;
  reset = 1'b1;
  instruction = 32'h00000000;
  #1
  #10
  reset = 1'b0;
  #20
  instruction = 32'b000000000001_00000_000_01011_0010011; // addi a1, zero, 1
  #10
  instruction = 32'b000000000010_00000_000_01100_0010011; // addi a2, zero, 2
  #10
  instruction = 32'b000000000101_00000_000_01101_0010011; // addi a3, zero, 5
  #10
  instruction = 32'b000000000110_00000_000_01110_0010011; // addi a4, zero, 6
  #10
  instruction = 32'b111111111111_00000_000_01111_0010011; // addi a5, zero, -1
  #10
  // Make sure you set instruction to NOP when your processor is idle.
  // Otherwise, your register file state will keep changing when the processor
  // tries to execute instruction each clock cycle.
  instruction = NOP;

  #10
  instruction = 32'b0000000_01100_01011_000_10000_0110011; // add a6, a1, a2

  #10
  // Here we are printing the state of the register file.
  // We should see the result of the add a6, a1, a2 instruction but not the
  // sub a7, a2, a4 instruction because there has not been a posedge clock yet
  $display("add a6, a1, a2");
  print_state();
  instruction = 32'b0100000_01110_01100_000_10001_0110011; // sub a7, a2, a4

  #10
  $display("sub a7, a2, a4");
  print_state();
  /***************************
  * Add more test cases here *
  ***************************/
	
  instruction = 32'h00f685b3; // add a1, a3, a5
  #10
  $display("add a1, a3, a5");
  print_state();

  instruction = 32'h40f70733; // sub a4, a4, a5
  #10
  $display("sub a4, a4, a5");
  print_state();

  instruction = 32'h00d725b3; // slt a1, a4, a3
  #10
  $display("slt a1, a4, a3");
  print_state();
  
  instruction = 32'h00e6a5b3; // slt a1, a3, a4
  #10
  $display("slt a1, a3, a4");
  print_state();
  
  instruction = 32'h00d745b3; // xor a1, a4, a3
  #10
  $display("xor a1, a4, a3");
  print_state();
  
  instruction = 32'h010745b3; // xor a1, a4, a6
  #10
  $display("xor a1, a4, a6");
  print_state();

  instruction = 32'h00d775b3; // and a1, a4, a3
  #10
  $display("and a1, a4, a3");
  print_state();
  
  instruction = 32'h010775b3; // and a1, a4, a6
  #10
  $display("and a1, a4, a6");
  print_state();
  
  instruction = 32'h00180813; // addi a6, a6, 1
  #10
  $display("addi a6, a6, 1");
  print_state();
  
  instruction = 32'h00d82023; // sw a3, 0(a6)
  #10
  $display("sw a3, 0(a6)");
  print_state();
  
  instruction = 32'h00082883; // lw a7, 0(a6)
  #10
  $display("lw a7, 0(a6)");
  print_state();
  
  instruction = 32'h00f82823; // sw a5, 16(a6)
  #10
  $display("sw a5, 0(a6)");
  print_state();
  
  instruction = 32'h01082883; // lw a7, 16(a6)
  #10
  $display("lw a7, 0(a6)");
  print_state();
  
  #10
  $stop();

end

endmodule

