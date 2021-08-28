//=============================================================================================
// LCD SPI
// Copyright 2020 by Glen Akins.
// All rights reserved.
// 
// Set editor tab stop to 4.
//=============================================================================================

`timescale 1ns / 1ps

module lcd_spi
(
	// 64 MHz clock and pll locked indicator
	input	wire            mclk,
	input	wire            mclk_pll_locked,
	
	// lcd interface
	output  wire            lcd_sclk,
	output  wire            lcd_data,
	output  wire            lcd_rst_n,
	output  wire            lcd_dc,
	output  wire            lcd_cs_n,

    // axi4-stream target interface for lcd data
    input   wire            s01_axis_aclk,
    input   wire            s01_axis_aresetn,
    input   wire            s01_axis_tvalid,
    output  wire            s01_axis_tready,
    input   wire     [15:0] s01_axis_tdata,
    
	// axi4-lite target interface for register accesses and lcd controller commands
	input   wire            s00_axi_aclk,
	input   wire            s00_axi_aresetn,
	input   wire      [7:0] s00_axi_awaddr,
	input   wire      [2:0] s00_axi_awprot,
	input   wire            s00_axi_awvalid,
	output  wire            s00_axi_awready,
	input   wire     [31:0] s00_axi_wdata,
	input   wire      [3:0] s00_axi_wstrb,
	input   wire            s00_axi_wvalid,
	output  wire            s00_axi_wready,
	output  wire      [1:0] s00_axi_bresp,
	output  wire            s00_axi_bvalid,
	input   wire            s00_axi_bready,
	input   wire      [7:0] s00_axi_araddr,
	input   wire      [2:0] s00_axi_arprot,
	input   wire            s00_axi_arvalid,
	output  wire            s00_axi_arready,
	output  wire     [31:0] s00_axi_rdata,
	output  wire      [1:0] s00_axi_rresp,
	output  wire            s00_axi_rvalid,
	input   wire            s00_axi_rready
);


//----------------------------------------
// assert mclk_rst until s00_axi_aresetn is deasserted and mclk PLL is locked
//----------------------------------------

reg mclk_rst, mclk_rst_0, mclk_rst_1, mclk_rst_2;

always @ (posedge mclk or negedge s00_axi_aresetn)
begin
	if (!s00_axi_aresetn)
	begin
		mclk_rst_0 <= 1; 
		mclk_rst_1 <= 1;
		mclk_rst_2 <= 1;
		mclk_rst <= 1;
	end
	else
	begin
		mclk_rst_0 <= !mclk_pll_locked; 
		mclk_rst_1 <= mclk_rst_0;
		mclk_rst_2 <= mclk_rst_1;
		mclk_rst <= mclk_rst_2;
	end
end


//----------------------------------------
// AXI4-Lite Target Interface
//----------------------------------------

wire [23:0]	timebase_reset_value;
reg       	timebase_flag;
wire      	timebase_flag_clear;

wire d8_write, d8_full;
wire [7:0] d8_wr_data;

reg d8_empty_sync;
reg d16_empty_sync;
reg lcd_busy_sync;
reg lcd_inuse;

lcd_spi_axil_tgt lcd_spi_axil_tgt
(
	.S_AXI_ACLK(s00_axi_aclk),
	.S_AXI_ARESETN(s00_axi_aresetn),
	.S_AXI_AWADDR(s00_axi_awaddr),
	.S_AXI_AWPROT(s00_axi_awprot),
	.S_AXI_AWVALID(s00_axi_awvalid),
	.S_AXI_AWREADY(s00_axi_awready),
	.S_AXI_WDATA(s00_axi_wdata),
	.S_AXI_WSTRB(s00_axi_wstrb),
	.S_AXI_WVALID(s00_axi_wvalid),
	.S_AXI_WREADY(s00_axi_wready),
	.S_AXI_BRESP(s00_axi_bresp),
	.S_AXI_BVALID(s00_axi_bvalid),
	.S_AXI_BREADY(s00_axi_bready),
	.S_AXI_ARADDR(s00_axi_araddr),
	.S_AXI_ARPROT(s00_axi_arprot),
	.S_AXI_ARVALID(s00_axi_arvalid),
	.S_AXI_ARREADY(s00_axi_arready),
	.S_AXI_RDATA(s00_axi_rdata),
	.S_AXI_RRESP(s00_axi_rresp),
	.S_AXI_RVALID(s00_axi_rvalid),
	.S_AXI_RREADY(s00_axi_rready),

    .d8_write                   (d8_write),
    .d8_full                    (d8_full),
    .d8_wr_data                 (d8_wr_data),
    
    .d8_empty_sync              (d8_empty_sync),
    .d16_empty_sync             (d16_empty_sync),
    .lcd_busy_sync              (lcd_busy_sync),
    .lcd_inuse                  (lcd_inuse),

    .lcd_rst_n                  (lcd_rst_n),
	.lcd_dc                     (lcd_dc),
	.lcd_cs_n                   (lcd_cs_n),

	.timebase_reset_value		(timebase_reset_value),
	.timebase_flag				(timebase_flag),
	.timebase_flag_clear		(timebase_flag_clear)
);


//----------------------------------------
// CPU main loop tasks timebase timer
//----------------------------------------

reg [23:0] timebase_counter;

always @ (posedge s00_axi_aclk or negedge s00_axi_aresetn)
begin
    if (!s00_axi_aresetn)
    begin
        timebase_counter <= 0;
        timebase_flag <= 0;
    end
    else
    begin
        if (timebase_counter == timebase_reset_value)
        begin
            timebase_counter <= 0;
            timebase_flag <= 1;
        end
        else
        begin
            timebase_counter <= timebase_counter + 1;
            if (timebase_flag_clear)
            begin
                timebase_flag <= 0;
            end
        end
    end
end


//----------------------------------------
// axil spi data fifo
//----------------------------------------

wire d8_empty, d8_read;
wire [7:0] d8_rd_data;

afifo16x8 afifo16x8
(
    .rst            (mclk_rst),
    
    .wr_clk         (s00_axi_aclk),
    .wr_en          (d8_write),
    .din            (d8_wr_data),
    .full           (d8_full),
    
    .rd_clk         (mclk),
    .rd_en          (d8_read),
    .dout           (d8_rd_data),
    .empty          (d8_empty)
);


//----------------------------------------
// axis spi data fifo
//----------------------------------------

wire d16_empty, d16_read, d16_full;
wire [15:0] d16_rd_data;
    
afifo256x16 afifo256x16
(
    .rst            (mclk_rst),
    
    .wr_clk         (s01_axis_aclk),
    .wr_en          (s01_axis_tvalid && s01_axis_tready),
    .din            (s01_axis_tdata),
    .full           (d16_full),
    
    .rd_clk         (mclk),
    .rd_en          (d16_read),
    .dout           (d16_rd_data),
    .empty          (d16_empty)
);

assign s01_axis_tready = !d16_full;


//----------------------------------------
// spi parallel to serial covnerter
//----------------------------------------

wire lcd_busy;

lcd_spi_serializer lcd_spi_serializer
(
    .clk            (mclk),
    .rst            (mclk_rst),
    
    .d8_empty       (d8_empty),
    .d8_data        (d8_rd_data),
    .d8_read        (d8_read),

    .d16_empty      (d16_empty),
    .d16_data       (d16_rd_data),
    .d16_read       (d16_read),
    
    .lcd_busy       (lcd_busy),
    
    .lcd_sclk       (lcd_sclk),
    .lcd_data       (lcd_data)
); 


//----------------------------------------
// sycnhronizers for d8_empty, d16_full, d16_empty, and lcd_busy back to s00_axi_aclk domain and register interface
//----------------------------------------

reg d8_empty_z, d16_empty_z, lcd_busy_z, lcd_busy_sync_z;

always @ (posedge mclk)
begin
    if (mclk_rst)
    begin
        d8_empty_z <= 0;
        d8_empty_sync <= 0;
        d16_empty_z <= 0;
        d16_empty_sync <= 0;
        lcd_busy_z <= 0;
        lcd_busy_sync <= 0;
        lcd_busy_sync_z <= 0;
        lcd_inuse <= 0;
    end
    else
    begin
        d8_empty_z <= d8_empty;
        d8_empty_sync <= d8_empty_z;
        d16_empty_z <= d16_empty;
        d16_empty_sync <= d16_empty_z;
        lcd_busy_z <= lcd_busy;
        lcd_busy_sync <= lcd_busy_z;
        lcd_busy_sync_z <= lcd_busy_sync;
        if (d8_write)
        begin
            lcd_inuse <= 1;
        end
        else if (lcd_busy_sync_z && !lcd_busy_sync)
        begin
            lcd_inuse <= 0;
        end
    end
end

endmodule
