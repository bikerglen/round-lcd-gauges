module lcd_spi_serializer
(
    input   wire            clk,
    input   wire            rst,
    
    input   wire            d8_empty,
    input   wire    [7:0]   d8_data,
    output  reg             d8_read,

    input   wire            d16_empty,
    input   wire    [15:0]  d16_data,
    output  reg             d16_read,
    
    output  wire            lcd_busy,
    
    output  reg             lcd_sclk,
    output  reg             lcd_data
); 

    localparam IDLE = 0, BUSY = 1;
    
    reg state;
    reg [4:0] bitnum;
    reg [4:0] txbits;
    reg [14:0] txdata;
    reg txphase;
    
    always @ (posedge clk)
    begin
        if (rst)
        begin
            d8_read <= 0;
            d16_read <= 0;            
            lcd_sclk <= 0;
            lcd_data <= 0;
            state <= IDLE;
            bitnum <= 0;
            txbits <= 0;
            txdata <= 0;
            txphase <= 0;
        end
        else
        begin
            // defaults
            d8_read <= 0;
            d16_read <= 0;            

            if (state == IDLE)
            begin
                if (!d16_empty)
                begin
                    d16_read <= 1;
                    state <= BUSY;
                    bitnum <= 0;
                    txbits <= 16;
                    lcd_sclk <= 0;
                    lcd_data <= d16_data[15];
                    txdata[14:0] <= d16_data[14:0]; 
                    txphase <= 1;                   
                end
                else if (!d8_empty)
                begin
                    d8_read <= 1;
                    state <= BUSY;
                    bitnum <= 0;
                    txbits <= 8;
                    lcd_sclk <= 0;
                    lcd_data <= d8_data[7];
                    txdata[14:8] <= d8_data[6:0];
                    txdata[7:0] <= 0;                    
                    txphase <= 1;                   
                end
                else 
                begin
                    lcd_sclk <= 0;
                    lcd_data <= 0;
                    state <= IDLE;
                end
            end
            else
            begin
                if (bitnum >= txbits)
                begin
                    if (!d16_empty)
                    begin
                        d16_read <= 1;
                        state <= BUSY;
                        bitnum <= 0;
                        txbits <= 16;
                        lcd_sclk <= 0;
                        lcd_data <= d16_data[15];
                        txdata[14:0] <= d16_data[14:0]; 
                        txphase <= 1;                   
                    end
                    else if (!d8_empty)
                    begin
                        d8_read <= 1;
                        state <= BUSY;
                        bitnum <= 0;
                        txbits <= 8;
                        lcd_sclk <= 0;
                        lcd_data <= d8_data[7];
                        txdata[14:8] <= d8_data[6:0];
                        txdata[7:0] <= 0;                    
                        txphase <= 1;                   
                    end
                    else 
                    begin
                        lcd_sclk <= 0;
                        lcd_data <= 0;
                        state <= IDLE;
                    end
                    end
                else
                begin
                    if (txphase == 1)
                    begin
                        lcd_sclk <= 1;
                        txphase <= 0;
                        bitnum <= bitnum + 1;
                    end
                    else
                    begin
                        lcd_sclk <= 0;
                        lcd_data <= txdata[14];
                        txdata[14:0] <= { txdata[13:0], 1'b0 };
                        txphase <= 1; 
                    end
                end
            end
        end
    end
    
    assign lcd_busy = state == IDLE;
        
endmodule


/*
module ckca_uart
(
	input	wire			clk,				// system clock
	input	wire			rst,				// synchronous, active-high system reset
	input	wire			baudEn,				// enable at baud rate
	input	wire			avail,				// new data byte to transmit available
	input	wire	[12:0]	data,				// 20/21b flag + 12 data bits
	output	reg				ack,				// data byte moved to transmit hold register
	output	reg				txd					// transmit data out
);

localparam TXIDLE = 0, TXBRK = 1, TXMAB = 2, TXDAT = 3;

reg  [1:0] txbusy;								// transmit state
reg  [4:0] bitnum;								// current tx bit number
reg  [4:0] txbits;								// number of bits to transmit minus one
reg	[19:0] txdata;								// tranmsit data hold register

always @ (posedge clk)
begin
	if (rst)
	begin
		ack <= 0;
		txd <= 0;
		txbusy <= 0;
		bitnum <= 0;
		txbits <= 0;
		txdata <= 0;
	end
	else
	begin
		ack <= 0;

		if (baudEn)
		begin
			case (txbusy)

				TXIDLE: begin
					if (avail)
					begin
						ack <= 1;
						txd <= 1;
						txbusy <= TXDAT;
						bitnum <= 0;
						txbits <= data[12] ? 20 : 19;
						txdata <= { 8'h00, data[11:0] };
					end
					else
					begin
						txd <= 0;
						bitnum <= 0;
						txbusy <= TXIDLE;
					end
				end

				TXDAT: begin
					if (bitnum >= txbits)
					begin
						if (avail)
						begin
							ack <= 1;
							txd <= 1;
							txbusy <= TXDAT;
							bitnum <= 0;
							txbits <= data[12] ? 20 : 19;
							txdata <= { 8'h00, data[11:0] };
						end
						else
						begin
							txd <= 0;
							bitnum <= 0;
							txbusy <= TXIDLE;
						end
					end
					else
					begin
						txd <= txdata[0];
						txdata <= { 1'b0, txdata[19:1] };
						bitnum <= bitnum + 1;
					end
				end
		
			endcase
		end
	end
end

endmodule
*/
