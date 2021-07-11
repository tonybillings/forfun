module segment_display_decoder (
	input [3:0] index,
	output reg [3:0] select
);

always @(*) begin
	select = 4'd0;

	case (index)
		0: select = 4'b0000;
		1: select = 4'b0001;
		2: select = 4'b0010;
		3: select = 4'b0100;
		4: select = 4'b1000;
		default: select = 4'b0000;
	endcase
end

endmodule