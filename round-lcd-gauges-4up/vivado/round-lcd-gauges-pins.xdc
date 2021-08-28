set_clock_groups -asynchronous -group { clk_fpga_0 } -group { clk_out1_design_1_clk_wiz_0_0 }

## Pmod Header JA
set_property -dict { PACKAGE_PIN Y18 IOSTANDARD LVCMOS33 } [get_ports { lcd0_sclk  }]; #JA[0] IO_L17P_T2_34 Sch=ja_p[1]
set_property -dict { PACKAGE_PIN Y19 IOSTANDARD LVCMOS33 } [get_ports { lcd0_data  }]; #JA[1] IO_L17N_T2_34 Sch=ja_n[1]
set_property -dict { PACKAGE_PIN Y16 IOSTANDARD LVCMOS33 } [get_ports { lcd0_cs_n  }]; #JA[2] IO_L7P_T1_34 Sch=ja_p[2]
set_property -dict { PACKAGE_PIN Y17 IOSTANDARD LVCMOS33 } [get_ports { lcd0_dc    }]; #JA[3] IO_L7N_T1_34 Sch=ja_n[2]
set_property -dict { PACKAGE_PIN U18 IOSTANDARD LVCMOS33 } [get_ports { lcd0_rst_n }]; #JA[4] IO_L12P_T1_MRCC_34 Sch=ja_p[3]
#set_property -dict { PACKAGE_PIN U19 IOSTANDARD LVCMOS33 } [get_ports { rx_lrck  }]; #JA[5] IO_L12N_T1_MRCC_34 Sch=ja_n[3]
#set_property -dict { PACKAGE_PIN W18 IOSTANDARD LVCMOS33 } [get_ports { rx_sclk  }]; #JA[6] IO_L22P_T3_34 Sch=ja_p[4]
#set_property -dict { PACKAGE_PIN W19 IOSTANDARD LVCMOS33 } [get_ports { rx_sdin  }]; #JA[7] IO_L22N_T3_34 Sch=ja_n[4]

## Pmod Header JB
set_property -dict { PACKAGE_PIN W14 IOSTANDARD LVCMOS33 } [get_ports { lcd1_sclk   }]; # JB[0] IO_L8P_T1_34 Sch=jb_p[1]
set_property -dict { PACKAGE_PIN Y14 IOSTANDARD LVCMOS33 } [get_ports { lcd1_data   }]; # JB[1] IO_L8N_T1_34 Sch=jb_n[1]
set_property -dict { PACKAGE_PIN T11 IOSTANDARD LVCMOS33 } [get_ports { lcd1_cs_n   }]; # JB[2] IO_L1P_T0_34 Sch=jb_p[2]
set_property -dict { PACKAGE_PIN T10 IOSTANDARD LVCMOS33 } [get_ports { lcd1_dc     }]; # JB[3] IO_L1N_T0_34 Sch=jb_n[2]
set_property -dict { PACKAGE_PIN V16 IOSTANDARD LVCMOS33 } [get_ports { lcd1_rst_n  }]; # JB[4] IO_L18P_T2_34 Sch=jb_p[3]
#set_property -dict { PACKAGE_PIN W16 IOSTANDARD LVCMOS33 } [get_ports { row[2]   }]; # JB[5] IO_L18N_T2_34 Sch=jb_n[3]
#set_property -dict { PACKAGE_PIN V12 IOSTANDARD LVCMOS33 } [get_ports { blank    }]; # JB[6] IO_L4P_T0_34 Sch=jb_p[4]
#set_property -dict { PACKAGE_PIN W13 IOSTANDARD LVCMOS33 } [get_ports { sclk     }]; # JB[7] IO_L4N_T0_34 Sch=jb_n[4]

## ChipKit Outer Analog Header - as Digital I/O
set_property -dict { PACKAGE_PIN F17   IOSTANDARD LVCMOS33 } [get_ports { lcd2_sclk }]; #IO_L6N_T0_VREF_35 Sch=ck_a[0]
set_property -dict { PACKAGE_PIN J19   IOSTANDARD LVCMOS33 } [get_ports { lcd2_data }]; #IO_L10N_T1_AD11N_35 Sch=ck_a[1]
set_property -dict { PACKAGE_PIN K17   IOSTANDARD LVCMOS33 } [get_ports { lcd2_cs_n }]; #IO_L12P_T1_MRCC_35 Sch=ck_a[2]
set_property -dict { PACKAGE_PIN L16   IOSTANDARD LVCMOS33 } [get_ports { lcd2_dc }]; #IO_L11P_T1_SRCC_35 Sch=ck_a[3]
set_property -dict { PACKAGE_PIN N16   IOSTANDARD LVCMOS33 } [get_ports { lcd2_rst_n }]; #IO_L21N_T3_DQS_AD14N_35 Sch=ck_a[4]
#set_property -dict { PACKAGE_PIN P14   IOSTANDARD LVCMOS33 } [get_ports { ck_a5 }]; #IO_L6P_T0_34 Sch=ck_a[5]

## ChipKit Inner Analog Header - as Digital I/O
set_property -dict { PACKAGE_PIN C20   IOSTANDARD LVCMOS33 } [get_ports { lcd3_sclk }]; #IO_L1P_T0_AD0P_35 Sch=ad_p[0]
set_property -dict { PACKAGE_PIN B20   IOSTANDARD LVCMOS33 } [get_ports { lcd3_data }]; #IO_L1N_T0_AD0N_35 Sch=ad_n[0]
set_property -dict { PACKAGE_PIN F19   IOSTANDARD LVCMOS33 } [get_ports { lcd3_cs_n }]; #IO_L15P_T2_DQS_AD12P_35 Sch=ad_p[12]
set_property -dict { PACKAGE_PIN F20   IOSTANDARD LVCMOS33 } [get_ports { lcd3_dc }]; #IO_L15N_T2_DQS_AD12N_35 Sch=ad_n[12]
set_property -dict { PACKAGE_PIN B19   IOSTANDARD LVCMOS33 } [get_ports { lcd3_rst_n }]; #IO_L2P_T0_AD8P_35 Sch=ad_p[8]
#set_property -dict { PACKAGE_PIN A20   IOSTANDARD LVCMOS33 } [get_ports { ck_a11 }]; #IO_L2N_T0_AD8N_35 Sch=ad_n[8]