`timescale 1ns / 1ns

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   14:21:18 11/20/2019
// Design Name:   ZYBO_TOP
// Module Name:   D:/work/CPU/8051/light52_iseprj/tb_top.v
// Project Name:  light52_iseprj
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: ZYBO_TOP
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module tb_top;

	// Inputs
	reg clk_50MHz_i;
	reg [1:0] buttons_i;
	reg [1:0] switches_i;
	reg pmod_e_3_rxd_i;

	// Outputs
	wire [3:0] leds_o;
	wire pmod_e_2_txd_o;
reg reset_async;
reg reset;
wire [7:0] SEG_o ;  //           std_logic_vector(7 downto 0);
wire [1:0] COM_o ;  // .         std_logic_vector(7 downto 0);
//reg [7:0] p2_in ;    // .         std_logic_vector(7 downto 0);
//reg [7:0] p3_in ;   //           std_logic_vector(7 downto 0);
	// Instantiate the Unit Under Test (UUT)
	ZYBO_TOP uut (
		.clk_50MHz_i(clk_50MHz_i), 
		.buttons_i(buttons_i), 
		.switches_i(switches_i), 
		.leds_o(leds_o), 
		.reset_async(reset_async),
		.SEG_o(SEG_o),
		.COM_o(COM_o),
		.pmod_e_2_txd_o(pmod_e_2_txd_o), 
		.pmod_e_3_rxd_i(pmod_e_3_rxd_i)
	);

	initial                 
	begin
		clk_50MHz_i <= 1'b0;
		#100;
		forever
		begin	 
			clk_50MHz_i <= 1'b0;
			#10;
			clk_50MHz_i <= 1'b1;
			#10;
		end
	end
always @ (posedge clk_50MHz_i) begin
	reset_async <= reset;
end	
	initial begin
		buttons_i <= 4'd0;
		switches_i <= {1'b1,1'b0,2'b00};
		// Initialize Inputs
		pmod_e_3_rxd_i = 1;
		//p2_in <= 8'h00;
		//p3_in <= 8'h00;
		// Wait 100 ns for global reset to finish
		reset <= 1'b0;
		#10000;
		reset <= 1'b1;
		switches_i <= {1'b0,1'b0,2'b00};
		// Add stimulus here
	end
      
endmodule

