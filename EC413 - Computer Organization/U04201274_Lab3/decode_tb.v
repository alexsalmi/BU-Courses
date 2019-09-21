// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Decode Test Bench

module decode_tb();

parameter NOP = 32'b000000000000_00000_000_00000_0010011; // addi zero, zero, 0

reg [31:0] instruction;

wire wb_sel;
wire [4:0] read_sel1;
wire [4:0] read_sel2;
wire [4:0] write_sel;
wire wEn;
wire [31:0] imm32;
wire ob_B_sel;
wire [5:0] ALU_Control;
wire mem_wEn;

decode uut (
  .instruction(instruction),
  .wb_sel(wb_sel),
  .read_sel1(read_sel1),
  .read_sel2(read_sel2),
  .write_sel(write_sel),
  .wEn(wEn),
  .imm32(imm32),
  .op_B_sel(op_B_sel),
  .ALU_Control(ALU_Control),
  .mem_wEn(mem_wEn)
);

task print_state;
  begin
    $display("Time:\t%0d", $time);
    $display("instruction:\t%b", instruction);
    $display("wb_sel:\t%b", wb_sel);
    $display("read_sel1:\t%d", read_sel1);
    $display("read_sel2:\t%d", read_sel2);
    $display("write_sel:\t%d", write_sel);
    $display("wEn:\t%b", wEn);
    $display("imm32:\t%b", imm32);
    $display("op_B_sel:\t%b", op_B_sel);
    $display("ALU_Control:\t%b", ALU_Control);
    $display("mem_wEn:\t%b", mem_wEn);
    $display("--------------------------------------------------------------------------------");
    $display("\n\n");
  end
endtask



initial begin
  $display("Starting Decode Test");
  $display("--------------------------------------------------------------------------------");
  instruction = NOP;
  #10
  // Display output of NOP instruction
  $display("addi zero, zero, 0");
  print_state();
  // Test a new instruction
  instruction = 32'b111111111111_00000_000_01011_0010011; // addi a1, zero, -1

  #10
  $display("addi a1, zero, -1");
  print_state();
  instruction = NOP;

  /**********************
  * Add Test Cases Here *
  **********************/
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
  
  instruction = 32'h00d02023; // sw a3, 0(zero)
  #10
  $display("sw a3, 0(a6)");
  print_state();
  
  instruction = 32'h00002883; // lw a7, 0(zero)
  #10
  $display("lw a7, 0(a6)");
  print_state();
  
  instruction = 32'h00f82823; // sw a5, 0(a6)
  #10
  $display("sw a5, 0(a6)");
  print_state();
  
  instruction = 32'h01082883; // lw a7, 0(a6)
  #10
  $display("lw a7, 0(a6)");
  print_state();













  #10
  $stop();
end

endmodule
