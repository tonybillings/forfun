namespace TIDE.Code
{
    public static class VerilogTemplates
    {
        public static readonly string DEFAULT_CLOCK_SPEED = "50";

        public static readonly int[] CLOCK_SPEEDS = { 5, 10, 25, 50, 100, 125 };

        public static readonly string PROGRAM_ROM =
@"module prog_rom_$ (
	input [7:0] addr,
	output reg [23:0] inst
);

always @(*) begin
	inst = 24'd0;

	case (addr)
///Instructions///
	endcase
end

endmodule";

        public static readonly string CORE_INSTANTIATION =
@"// Core $
wire core_read_$, core_write_$;
wire [7:0] core_addr_$, core_dout_$;
reg [7:0] core_din_$;
wire [7:0] core_prog_addr_$;
wire [23:0] core_prog_inst_$;

core core_$(
    .clk(clk),
    .clk_125(clk_125),
    .rst(rst),
    .read(core_read_$),
    .write(core_write_$),
    .addr(core_addr_$),
    .dout(core_dout_$),
    .din(core_din_$),
    .prog_addr(core_prog_addr_$),
    .prog_inst(core_prog_inst_$)
);";

        public static readonly string PROGRAM_ROM_INSTANTIATION =
@"// Program ROM for Core $
prog_rom_$ rom_$(
	.addr(core_prog_addr_$),
	.inst(core_prog_inst_$)
);";

        public static readonly string EXTENDED_MEMORY_INSTANTIATION =
@"// Extended memory for $ST $N
reg core_mem_we_$C1;
reg [7:0] core_mem_addr_$C1;
reg [7:0] core_mem_din_$C1;
wire [7:0] core_mem_dout_$C1;

reg core_mem_we_$C2$0;
reg [7:0] core_mem_addr_$C2$1;
reg [7:0] core_mem_din_$C2$1;
wire [7:0] core_mem_dout_$C2$1;

block_memory mem_$N(
    .clka(clk),
    .wea(core_mem_we_$C1),
    .addra(core_mem_addr_$C1),
    .dina(core_mem_din_$C1),
    .douta(core_mem_dout_$C1),
    .clkb(clk),
    .web(core_mem_we_$C2),
    .addrb(core_mem_addr_$C2),
    .dinb(core_mem_din_$C2),
    .doutb(core_mem_dout_$C2)
);";

        public static readonly string SEGMENT_DISPLAY_DECODER_INSTANTIATION =
@"// Segment display decoder for Core $
reg[3:0] seg_disp_idx_$;
wire[3:0] seg_disp_sel_$;

segment_display_decoder seg_decoder_$(
    .index(seg_disp_idx_$),
    .select(seg_disp_sel_$)
);";

        public static readonly string SEGMENT_DISPLAY_ROM_INSTANTIATION =
@"// Segment display ROM for Core $
reg [3:0] seg_disp_char_$;
wire [7:0] seg_disp_segs_$;

segment_display_rom seg_rom_$ (
	.char(seg_disp_char_$),
	.segs(seg_disp_segs_$)
);";

        public static readonly string PROCESSOR_COMBINATIONAL_LOGIC =
@"	// Defaults for Core $
	seg_disp_char_$ = 4'bxxxx;
	seg_disp_idx_$ = 4'bxxxx;
    core_din_$ = 8'bxxxxxxxx;
	core_mem_we_$ = `LOW;
	core_mem_addr_$ = 8'b00000000;
	core_mem_din_$ = 8'b00000000;

	// Handle memory-mapped IO for Core $
	case (core_addr_$)
	    224:
		begin
	        if (core_write_$)
	            led_d = core_dout_$;
	        if (core_read_$)
	            core_din_$ = led_q;
	    end
		225:
		begin
			if (core_write_$)
	            io_led_d[0+:8] = core_dout_$;
	        if (core_read_$)
	            core_din_$ = io_led_q[0+:8];
		end
		226:
		begin
			if (core_write_$)
	            io_led_d[8+:8] = core_dout_$;
	        if (core_read_$)
	            core_din_$ = io_led_q[8+:8];
		end
		227:
		begin
			if (core_write_$)
	            io_led_d[16+:8] = core_dout_$;
	        if (core_read_$)
	            core_din_$ = io_led_q[16+:8];
		end
		228:
		begin
			if (core_write_$) begin
				seg_disp_char_$ = core_dout_$[0+:4];
				io_seg_d = seg_disp_segs_$;
			end
		end
		229:
		begin
			if (core_write_$) begin
				seg_disp_idx_$ = core_dout_$[0+:4];
	            io_sel_d = seg_disp_sel_$;
			end
		end
		230:
		begin
	        if (core_read_$)
	            core_din_$ = io_dip[0+:8];
		end
		231:
		begin
	        if (core_read_$)
	            core_din_$ = io_dip[8+:8];
		end
		232:
		begin
	        if (core_read_$)
	            core_din_$ = io_dip[16+:8];
		end
		233:
		begin
	        if (core_read_$)
	            core_din_$ = {7'b0, io_btn[0]};
		end
		234:
		begin
	        if (core_read_$)
	            core_din_$ = {7'b0, io_btn[1]};
		end
		235:
		begin
	        if (core_read_$)
	            core_din_$ = {7'b0, io_btn[2]};
		end
		236:
		begin
	        if (core_read_$)
	            core_din_$ = {7'b0, io_btn[3]};
		end
		237:
		begin
	        if (core_read_$)
	            core_din_$ = {7'b0, io_btn[4]};
		end
///ProcComboLogicCases///
		default:
		begin
			if (core_write_$) begin
				core_mem_we_$ = `HIGH;
				core_mem_addr_$ = core_addr_$;
				core_mem_din_$ = core_dout_$;
			end

			if (core_read_$) begin
				core_mem_addr_$ = core_addr_$;
				core_din_$ = core_mem_dout_$;
			end
		end
	endcase
";

    }
}
