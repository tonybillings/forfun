`timescale 1ns / 1ps

module synchronizer #(parameter BIT_LEN=1) (
	input clk,
	input [BIT_LEN-1:0] data_in,
	output [BIT_LEN-1:0] data_out
);

reg [BIT_LEN-1:0] sync_1;
reg [BIT_LEN-1:0] sync_2;

always @(posedge clk) begin
	sync_1 <= data_in;
	sync_2 <= sync_1;
end 

assign data_out = sync_2;  

endmodule
