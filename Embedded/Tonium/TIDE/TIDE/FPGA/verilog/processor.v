`include "globals.txt"

// Multi-core processor
module processor(
	input clk,
	input clk_125,
	input rst,
///ProcessorPorts///
	output [7:0] led,
	output [23:0] io_led,
	output [7:0] io_seg,
	output [3:0] io_sel,
	input [23:0] io_dip,
	input [4:0] io_btn
);

// IO registers
reg [7:0] led_d, led_q;
reg [23:0] io_led_d, io_led_q;
reg [7:0] io_seg_d, io_seg_q;
reg [3:0] io_sel_d, io_sel_q;

assign led = led_q;
assign io_led = io_led_q;
assign io_seg = ~io_seg_q;
assign io_sel = ~io_sel_q;

// IO pin registers
///ProcessorRegisters///

///Cores///
///ProgramRoms///
///ExtendedMemoryModules///
///SegmentDisplayDecoders///
///SegmentDisplayRoms///
// Combinational logic
always @(*) begin
	led_d = led_q;
	io_led_d = io_led_q;
	io_seg_d = io_seg_q;
	io_sel_d = io_sel_q;
	///ProcComboLogicDefaults///

///CombinationalLogic///
end

// Sequential circuit
always @(posedge clk) begin
    if (rst) begin
        led_q <= 0;
        io_led_q <= 0;
        io_seg_q <= 0;
        io_sel_q <= 0;
///SequentialLogicOnClockEdgeReset///
    end else begin
        led_q <= led_d;
        io_led_q <= io_led_d;
        io_seg_q <= io_seg_d;
        io_sel_q <= io_sel_d;
///SequentialLogicOnClockEdge///
    end
end

endmodule