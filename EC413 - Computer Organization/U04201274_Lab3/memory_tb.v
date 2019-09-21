// Name: Alexander Salmi
// BU ID: U04201274
// EC413 Lab 3: Memory Test Bench

module memory_tb();

reg clock;
reg wEn;
reg [15:0] address;
reg [31:0] write_data;

wire [31:0] read_data;
integer i, val;

memory uut (
  .clock(clock),
  .wEn(wEn),
  .address(address),
  .write_data(write_data),
  .read_data(read_data)
);

always #5 clock = ~clock;


initial begin
  clock = 1'b1;
  address = 16'd0;
  write_data = 16'd0;
  wEn = 1'b0;

  #1
  #10
  wEn = 1'b1;
  #10
  $display("Address %d: %h", address, read_data);
  write_data = 1;
  address = 4;
  #10
  $display("Address %d: %h", address, read_data);
  write_data = 2;
  address = 8;
  #10
  $display("Address %d: %h", address, read_data);



  /***************************
  * Add more test cases here *
  ***************************/
  write_data = 10;
  address = 4;
  #10
  $display("Address %d: %h", address, read_data);
  write_data = 20;
  address = 16;
  #10
  $display("Address %d: %h", address, read_data);
  address = 12;
  #10
  $display("Address %d: %h", address, read_data);
  val = 0;
  for(i=0; i<(2**14)-1; i=i+4)begin
	  write_data = val;
	  address = i;
	  #10
	  $display("Address %d: %h", address, read_data);
	  val = val + 16;
  end

  #100
  $stop();

end

endmodule
