module reset_conditioner (
	input clk,
	input in,
	output reg out
  );
  
localparam STAGES = 3'h4;
  
reg [3:0] stage_d, stage_q = 4'hf;
  
always @* begin
	stage_d = stage_q;
    
	stage_d = {stage_q[2-:3], 1'h0};
	out = stage_q[3-:1];
end
  
always @(posedge clk) begin
	if (in == 1'b1) begin
		stage_q <= 4'hf;
	end else begin
		stage_q <= stage_d;
	end
end
  
endmodule
