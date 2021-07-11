module segment_display_rom (
	input [3:0] char,
	output reg [7:0] segs 
);

always @(*) begin
	segs = 8'd0;

	case (char)
		0: segs = 8'b00111111;
		1: segs = 8'b00000110;
		2: segs = 8'b01011011;
		3: segs = 8'b01001111;
		4: segs = 8'b01100110;
		5: segs = 8'b01101101;
		6: segs = 8'b01111101;
		7: segs = 8'b00000111;
		8: segs = 8'b01111111;
		9: segs = 8'b01100111;
		10: segs = 8'b10000000;
		default: segs = 8'b00000000;
	endcase
end

endmodule