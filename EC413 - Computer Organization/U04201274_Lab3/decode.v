// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Decode

module decode (
  input [31:0] instruction,
  output [4:0] read_sel1,
  output [4:0] read_sel2,
  output [4:0] write_sel,
  output wb_sel,
  output wEn,
  output [31:0] imm32,
  output op_B_sel,
  output [5:0] ALU_Control,
  output mem_wEn

);

localparam [6:0] R_TYPE = 7'b0110011;
localparam [6:0] I_TYPE = 7'b0010011; // Note: this is not the opcode for all
                                      // I-Type instructions, only the
                                      // arithmatic ones. Other I-Type
                                      // instructions  (like load word) exist.
localparam [6:0] STORE  = 7'b0100011;
localparam [6:0] LOAD   = 7'b0000011;

wire [6:0] opcode;
wire [6:0] funct7;
wire [2:0] funct3;

wire [11:0] imm12;
wire [11:0] i_imm; // Immidiate for I-Type instructions
wire [11:0] s_imm; // Immidiate for S-Type instructions

assign opcode = instruction[6:0];
assign funct7 = instruction[31:25];
assign funct3 = instruction[14:12];

// Sign extension
assign imm32 = { {20{imm12[11]}}, imm12};

assign read_sel1 = instruction[19:15];
assign read_sel2 = instruction[24:20];
assign write_sel = instruction[11:7];


/******************************************************************************
* Start Your Code Here
******************************************************************************/
assign ALU_Control = (opcode == LOAD) ? 6'b000000 :				//LW
							(opcode == STORE) ? 6'b000000 :				//SW
							(opcode == I_TYPE) ? 6'b000000 :				//ADDI
							((funct7 == 7'b0000000) && 
							(funct3 == 3'b000)) ? 6'b000000 :				//ADD
							(funct7 == 7'b0100000) ? 6'b001000 :			//SUB
							(funct3 == 3'b010) ? 6'b000010 :				//SLT
							(funct3 == 3'b100) ? 6'b000100 :				//XOR
							(funct3 == 3'b111) ? 6'b000111 : 				//AND
							0;

assign wb_sel = (opcode == LOAD) ? 1 : 0;
assign op_B_sel = ((opcode == I_TYPE) || (opcode == STORE) || (opcode == LOAD)) ? 1 : 0;
assign mem_wEn = (opcode == STORE) ? 1 : 0;
assign wEn = (opcode !== STORE) ? 1 : 0;

assign i_imm = instruction[31:20];
assign s_imm = {instruction[31:25], instruction[11:7]};

assign imm12 = (opcode == LOAD) ? i_imm :
					(opcode == I_TYPE) ? i_imm:
					(opcode == STORE) ? s_imm : 0;
					
endmodule
