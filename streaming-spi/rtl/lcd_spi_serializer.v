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
    
    output  reg             lcd_busy,
    
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
            lcd_busy <= 0;            
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

            // state machine
            if (state == IDLE)
            begin
                if (!d16_empty)
                begin
                    d16_read <= 1;
                    state <= BUSY;
                    lcd_busy <= 1;
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
                    lcd_busy <= 1;
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
                    lcd_busy <= 0;
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
                        lcd_busy <= 1;
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
                        lcd_busy <= 1;
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
                        lcd_busy <= 0;
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
    
endmodule
