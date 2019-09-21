// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 2 Problem 1: Register File

module regFile (
  input clock,
  input reset,
  input wEn, // Write Enable
  input [31:0] write_data,
  input [4:0] read_sel1,
  input [4:0] read_sel2,
  input [4:0] write_sel,
  output [31:0] read_data1,
  output [31:0] read_data2
);

reg   [31:0] reg_file[0:31];

/******************************************************************************
*                      Start Your Code Here
******************************************************************************/
integer i;
reg [31:0] data1, data2;

always@(posedge clock)begin
	if(reset)begin
		for(i=0; i<32; i=i+1)begin
			reg_file[i] <= 0;
		end
		data1 <= 0;
		data2 <= 0;
	end
	else if(wEn)begin
		data1 <= reg_file[read_sel1];
		data2 <= reg_file[read_sel2];
		reg_file[write_sel] <= write_data;
	end
end //always

assign read_data1 = wEn ? data1 : reg_file[read_sel1];
assign read_data2 = wEn ? data2 : reg_file[read_sel2];

endmodule
