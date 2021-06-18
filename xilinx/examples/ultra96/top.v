`default_nettype none
`timescale 1 ps / 1 ps

module top (output led);

	wire clk, led;
	reg [31:0] ctr;

	always @(posedge clk) {led, ctr} <= ctr + 32'd1;
	wire [3:0] plclk;
	PS8 ps_i(
		.PLCLK(plclk)
	);
	BUFG_PS bfg_i(
		.I(plclk[0]),
		.O(clk)
	);
endmodule
