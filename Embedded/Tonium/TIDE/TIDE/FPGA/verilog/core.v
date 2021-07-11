`include "globals.txt"

// CPU core
module core (
	input clk,					// core clock
	input clk_125,				// timer clock
	input rst,					// reset
	output reg read,			// CPU read request
	output reg write,			// CPU write request
	output reg [7:0] addr,		// read/write address
	output reg [7:0] dout,		// write data
	input [7:0] din,			// read data
	output [7:0] prog_addr,		// program ROM address 
	input [23:0] prog_inst		// program instruction
);

// L1 cache
reg [127:0] mem_d, mem_q;

// Program address
assign prog_addr = mem_q[7:0];

// Instruction components
wire [7:0] op = prog_inst[23-:8];
wire [3:0] arg0 = prog_inst[15-:4];
wire [3:0] arg1 = prog_inst[11-:4];
wire [3:0] arg2 = prog_inst[7-:4];
wire [7:0] const = prog_inst[7-:8];

// Wait signal
reg wait_d, wait_q;

// Timers
reg tmr_us_en_d, tmr_us_en_q;
reg tmr_ms_en_d, tmr_ms_en_q;
reg [31:0] tmr_delay_d, tmr_delay_q;
wire tmr_us_done, tmr_ms_done;

timer_us #(.BIT_LEN(32)) tmr_us(
	.clk(clk_125),
	.rst(rst),
	.en(tmr_us_en_q),
	.delay(tmr_delay_q),
	.done(tmr_us_done)
);

timer_ms #(.BIT_LEN(32)) tmr_ms(
	.clk(clk_125),
	.rst(rst),
	.en(tmr_ms_en_q),
	.delay(tmr_delay_q),
	.done(tmr_ms_done)
);

wire tmr_us_done_sync;
synchronizer #(.BIT_LEN(1)) tmr_us_sync(
	.clk(clk),
	.data_in(tmr_us_done),
	.data_out(tmr_us_done_sync)
);

wire tmr_ms_done_sync;
synchronizer #(.BIT_LEN(1)) tmr_ms_sync(
	.clk(clk),
	.data_in(tmr_ms_done),
	.data_out(tmr_ms_done_sync)
);

// Combinational circuit
always @(*) begin
	mem_d = mem_q;
	
	read = `LOW;
	write = `LOW;
	addr = 8'd0;
	dout = 8'd0;

	tmr_us_en_d = tmr_us_en_q;
	tmr_ms_en_d = tmr_ms_en_q;
	tmr_delay_d = tmr_delay_q;

	wait_d = wait_q;

	mem_d[7:0] = mem_q[7:0] + 8'b1;

	case (op)
		`IN: begin
			read = `HIGH;
			mem_d[(arg0<<3)+7-:8] = din;
			addr = mem_q[(arg1<<3)+7-:8] + const;

			if (wait_q == `LOW) begin
				wait_d = `HIGH;
				mem_d[7:0] = mem_q[7:0];
			end else begin
				wait_d = `LOW;
			end
      	end
		`INC: begin
			read = `HIGH;
			mem_d[(arg0<<3)+7-:8] = din;
			addr = const;

			if (wait_q == `LOW) begin
				wait_d = `HIGH;
				mem_d[7:0] = mem_q[7:0];
			end else begin
				wait_d = `LOW;
			end
      	end
		`OUT: begin
			write = `HIGH;
			dout = mem_q[(arg0<<3)+7-:8];
			addr = mem_q[(arg1<<3)+7-:8] + const;
      	end
		`OUTC: begin
			write = `HIGH;
			dout = mem_q[(arg0<<3)+7-:8];
			addr = const;
      	end
      	`SET: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8];
		end
		`SETC: begin
			mem_d[(arg0<<3)+7-:8] = const;
		end
		`EQ: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] == mem_q[(arg2<<3)+7-:8];
		end
		`EQC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] == const;
		end
		`NEQ: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] != mem_q[(arg2<<3)+7-:8];
		end
		`NEQC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] != const;
		end
		`LT: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] < mem_q[(arg2<<3)+7-:8];
		end
		`LTC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] < const;
		end
		`LTEQ: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] <= mem_q[(arg2<<3)+7-:8];
		end
		`LTEQC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] <= const;
		end
		`GT: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] > mem_q[(arg2<<3)+7-:8];
		end
		`GTC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] > const;
		end
		`GTEQ: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] >= mem_q[(arg2<<3)+7-:8];
		end
		`GTEQC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] >= const;
		end
		`ADD: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] + mem_q[(arg2<<3)+7-:8];
		end
		`ADDC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] + const;
		end
		`SUB: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] - mem_q[(arg2<<3)+7-:8];
		end
		`SUBC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] - const;
		end
		`MUL: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] * mem_q[(arg2<<3)+7-:8];
		end
		`MULC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] * const;
		end
		`SHL: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] << mem_q[(arg2<<3)+7-:8];
		end
		`SHLC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] << const;
		end
		`SHR: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] >> mem_q[(arg2<<3)+7-:8];
		end
		`SHRC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] >> const;
		end
		`LAND: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] && mem_q[(arg2<<3)+7-:8];
		end
		`LANDC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] && const;
		end
		`AND: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] & mem_q[(arg2<<3)+7-:8];
		end
		`ANDC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] & const;
		end
		`NAND: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] & mem_q[(arg2<<3)+7-:8]);
		end
		`NANDC: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] & const);
		end
		`LOR: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] || mem_q[(arg2<<3)+7-:8];
		end
		`LORC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] || const;
		end
		`OR: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] | mem_q[(arg2<<3)+7-:8];
		end
		`ORC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] | const;
		end
		`NOR: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] | mem_q[(arg2<<3)+7-:8]);
		end
		`NORC: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] | const);
		end
		`XOR: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] ^ mem_q[(arg2<<3)+7-:8];
		end
		`XORC: begin
			mem_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8] ^ const;
		end
		`XNOR: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] ^ mem_q[(arg2<<3)+7-:8]);
		end
		`XNORC: begin
			mem_d[(arg0<<3)+7-:8] = ~(mem_q[(arg1<<3)+7-:8] ^ const);
		end
		`INV: begin
			mem_d[(arg0<<3)+7-:8] = ~mem_q[(arg1<<3)+7-:8];
		end
		`BEQ: begin
			if (mem_q[(arg0<<3)+7-:8] == mem_q[(arg1<<3)+7-:8]) begin
				mem_d[7:0] = mem_q[7:0] + 8'd2;
			end
		end
		`BEQC: begin
			if (mem_q[(arg0<<3)+7-:8] == const) begin
				mem_d[7:0] = mem_q[7:0] + 8'd2;
			end
		end
		`BNEQ: begin
			if (mem_q[(arg0<<3)+7-:8] != mem_q[(arg1<<3)+7-:8]) begin
				mem_d[7:0] = mem_q[7:0] + 8'd2;
			end
		end
		`BNEQC: begin
			if (mem_q[(arg0<<3)+7-:8] != const) begin
				mem_d[7:0] = mem_q[7:0] + 8'd2;
			end
		end
		`DLYUS: begin
			if (tmr_us_en_q == 1'b0) begin
				tmr_us_en_d = 1'b1;
				mem_d[7:0] = mem_q[7:0];
			end else if (tmr_us_done_sync == 1'b1) begin
				tmr_us_en_d = 1'b0;
			end else begin
				mem_d[7:0] = mem_q[7:0];
			end
		end
		`DLYMS: begin
			if (tmr_ms_en_q == 1'b0) begin
				tmr_ms_en_d = 1'b1;
				mem_d[7:0] = mem_q[7:0];
			end else if (tmr_ms_done_sync == 1'b1) begin
				tmr_ms_en_d = 1'b0;
			end else begin
				mem_d[7:0] = mem_q[7:0];
			end
		end
		`SDLY: begin
			tmr_delay_d[(arg0<<3)+7-:8] = mem_q[(arg1<<3)+7-:8];
		end
		`SDLYC: begin
			tmr_delay_d[(arg0<<3)+7-:8] = const;
		end
	endcase
end

// Sequential circuit
always @(posedge clk) begin
	if (rst) begin
		mem_q <= 128'd0;
		wait_q <= `LOW;
		tmr_us_en_q <= 1'b0;
		tmr_ms_en_q <= 1'b0;
		tmr_delay_q <= 32'd0;
	end else begin
		mem_q <= mem_d;
		wait_q <= wait_d;
		tmr_us_en_q <= tmr_us_en_d;
		tmr_ms_en_q <= tmr_ms_en_d;
		tmr_delay_q <= tmr_delay_d;
	end
end

endmodule