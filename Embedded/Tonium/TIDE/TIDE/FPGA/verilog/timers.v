`timescale 1ns / 1ps

// *****************************************************
// Timer module used for delaying in microseconds
// *****************************************************

module timer_us #(parameter BIT_LEN=1) (
	input clk,
	input rst,
	input en,
	input [BIT_LEN-1:0] delay,
	output done
);

wire [BIT_LEN-1:0] delay_sync;
synchronizer #(.BIT_LEN(BIT_LEN)) sync1(
	.clk(clk),
	.data_in(delay),
	.data_out(delay_sync)
);

wire en_sync;
synchronizer #(.BIT_LEN(1)) sync2(
	.clk(clk),
	.data_in(en),
	.data_out(en_sync)
);

reg [BIT_LEN-1:0] ctr_d, ctr_q;
reg [7:0] pass_d, pass_q;

reg done_d, done_q;
assign done = done_q;

always @(*) begin
	ctr_d = ctr_q;
	pass_d = pass_q;
	done_d = done_q;

	if (en_sync) begin
		if (pass_q == 8'd124) begin
			if (ctr_q + 1'd1 == delay_sync) begin
				done_d = 1'b1;
			end else begin
				pass_d = 8'd0;
				ctr_d = ctr_q + 1'd1;
			end
		end else begin
			pass_d = pass_q + 1'd1;
		end
	end else begin
		ctr_d = {BIT_LEN{1'b0}};
		pass_d = 8'b0;
		done_d = 1'b0;
	end
end

always @(posedge clk) begin
	if (rst) begin
		ctr_q <= {BIT_LEN{1'b0}};
		pass_q <= 8'd0;
		done_q <= 1'b0;
	end else begin
		ctr_q <= ctr_d;
		pass_q <= pass_d;
		done_q <= done_d;
	end
end

endmodule

// *****************************************************
// Timer module used for delaying in milliseconds
// *****************************************************

module timer_ms #(parameter BIT_LEN=1) (
	input clk,
	input rst,
	input en,
	input [BIT_LEN-1:0] delay,
	output done
);

wire [BIT_LEN-1:0] delay_sync;
synchronizer #(.BIT_LEN(BIT_LEN)) sync1(
	.clk(clk),
	.data_in(delay),
	.data_out(delay_sync)
);

wire en_sync;
synchronizer #(.BIT_LEN(1)) sync2(
	.clk(clk),
	.data_in(en),
	.data_out(en_sync)
);

reg [BIT_LEN-1:0] ctr_d, ctr_q;
reg [7:0] pass1_d, pass1_q;
reg [9:0] pass2_d, pass2_q;

reg done_d, done_q;
assign done = done_q;

always @(*) begin
	ctr_d = ctr_q;
	pass1_d = pass1_q;
	pass2_d = pass2_q;
	done_d = done_q;

	if (en_sync) begin
		if (pass1_q == 8'd124) begin
			if (pass2_q == 10'd999) begin
				if (ctr_q + 1'd1 == delay_sync) begin
					done_d = 1'b1;
				end else begin
					pass1_d = 8'd0;
					pass2_d = 10'd0;
					ctr_d = ctr_q + 1'd1;
				end
			end else begin
				pass1_d = 8'd0;
				pass2_d = pass2_q + 1'b1;
			end
		end else begin
			pass1_d = pass1_q + 1'd1;
		end
	end else begin
		ctr_d = {BIT_LEN{1'b0}};
		pass1_d = 8'b0;
		pass2_d = 10'b0;
		done_d = 1'b0;
	end
end

always @(posedge clk) begin
	if (rst) begin
		ctr_q <= {BIT_LEN{1'b0}};
		pass1_q <= 8'd0;
		pass2_q <= 10'd0;
		done_q <= 1'b0;
	end else begin
		ctr_q <= ctr_d;
		pass1_q <= pass1_d;
		pass2_q <= pass2_d;
		done_q <= done_d;
	end
end

endmodule
