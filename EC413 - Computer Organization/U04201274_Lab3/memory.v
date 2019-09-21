// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Memory

module memory (
  input clock,
  input wEn,
  input [15:0] address,
  input [31:0] write_data,
  output [31:0] read_data
);

reg [31:0] mem [(2**14)-1:0]; // 2^16 bytes or 2^14 words of memory

/******************************************************************************
*                      Start Your Code Here
*******************************************************************************/
always@(posedge clock)begin
	if(wEn)begin
		mem[address[15:2]] <= write_data;
	end
end //always

assign read_data = mem[address[15:2]];


endmodule
