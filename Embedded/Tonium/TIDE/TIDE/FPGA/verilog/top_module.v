module top_module (
    // 50MHz clock input
    input clk_50,

    // Input from reset button (active low)
    input rst_n,

    // cclk input from AVR, high when AVR is ready
    input cclk,

	// IO pins to external devices
///TopModulePorts///

    // Outputs to the 8 onboard LEDs
    output [7:0] led,

	// Outputs to the 24 LEDs on IO shield
	output [23:0] io_led,

	// Output to the selected seven-segment display
    output [7:0] io_seg,

    // Used to select a seven-segment display
    output [3:0] io_sel,

	// 24 dip switches
    input [23:0] io_dip,

	// Up, down, left, right, and center buttons
    input [4:0] io_btn,

    // AVR SPI connections
    output spi_miso,
    input spi_ss,
    input spi_mosi,
    input spi_sck,

    // AVR ADC channel select
    output [3:0] spi_channel,

    // Serial connections
    input avr_tx,       // AVR Tx => FPGA Rx 
    output avr_rx,      // AVR Rx => FPGA Tx 
    input avr_rx_busy   // AVR Rx buffer full 
);

// These signals should be high-z when not used
assign spi_miso = 1'bz;
assign avr_rx = 1'bz;
assign spi_channel = 4'bzzzz;

// Top wires
wire rst;
wire clk = ///CustomClockWire///;

// Clock
wire cc_5;
wire cc_10;
wire cc_25;
wire cc_50;
wire cc_100;
wire cc_125;
custom_clock cc(
	.CLK_IN(clk_50),
	.CLK_OUT_5(cc_5),
	.CLK_OUT_10(cc_10),
	.CLK_OUT_25(cc_25),
	.CLK_OUT_50(cc_50),
	.CLK_OUT_100(cc_100),
	.CLK_OUT_125(cc_125)
);

// Reset conditioner
reset_conditioner rst_cond(
	.clk(clk),
	.in(~rst_n),
	.out(rst)
 );

// Processor
processor proc(
	.clk(clk),
	.clk_125(cc_125),
	.rst(rst),
	.led(led),
///ProcessorInstantiation///
	.io_led(io_led),
	.io_seg(io_seg),
	.io_sel(io_sel),
	.io_dip(io_dip),
	.io_btn(io_btn)
);

endmodule