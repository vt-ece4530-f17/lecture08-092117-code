module  mymul (
	       output [15:0] per_dout,
	       input 	     mclk,
	       input [13:0]  per_addr,
	       input [15:0]  per_din,
	       input 	     per_en,
	       input [1:0]   per_we,
	       input 	     puc_rst
	       );
   
   reg [15:0] 		     hw_a;
   reg [15:0] 		     hw_b;
   reg [15:0] 		     hw_retvallo;
   reg [15:0] 		     hw_retvalhi;
   reg 			     hw_ctl;
   reg 			     hw_ctl_old;
   wire [31:0] 		     mulresult;
   
   wire 		     write_a;
   wire 		     write_b;
   wire 		     write_retval;
   wire 		     write_ctl;	
   
   always @(posedge mclk or posedge puc_rst)
     if (puc_rst)
       begin
	  hw_a        <= 16'h0;
	  hw_b        <= 16'h0;
	  hw_retvallo <= 16'h0;
	  hw_retvalhi <= 16'h0;
	  hw_ctl      <= 1'h0;
	  hw_ctl_old  <= 1'h0;
       end
     else
       begin
	  hw_a        <= write_a      ? per_din[15:0]   : hw_a;
	  hw_b        <= write_b      ? per_din[15:0]   : hw_b;
	  hw_retvallo <= write_retval ? mulresult[15:0] : hw_retvallo;
	  hw_retvalhi <= write_retval ? mulresult[31:0] : hw_retvalhi;
	  hw_ctl      <= write_ctl    ? per_din[0]      : hw_ctl;
	  hw_ctl_old  <= hw_ctl;
       end
   
   assign mulresult = hw_a * hw_b;
   
   assign write_a      = (per_en & (per_addr == 14'hA0) & per_we[0] & per_we[1]);
   assign write_b      = (per_en & (per_addr == 14'hA1) & per_we[0] & per_we[1]);
   assign write_ctl    = (per_en & (per_addr == 14'hA4) & per_we[0] & per_we[1]);
   assign write_retval = ((hw_ctl == 1'h1) & (hw_ctl ^ hw_ctl_old));
   
   assign per_dout = (per_en & (per_addr == 14'hA2) & ~per_we[0] & ~per_we[1]) ? hw_retvallo : 
	             (per_en & (per_addr == 14'hA3) & ~per_we[0] & ~per_we[1]) ? hw_retvalhi : 16'h0;
   
   
endmodule					   
