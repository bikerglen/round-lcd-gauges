#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include "ap_int.h"
#include "hls_stream.h"
#include "Blend4.h"

int Blend4 (hls::stream<uint16_t> &pixels0, hls::stream<uint16_t> &pixels1,
		    hls::stream<uint16_t> &pixels2, hls::stream<uint16_t> &pixels3,
		    uint32_t *background, uint32_t *foreground)
{
#pragma HLS INTERFACE axis port=pixels0
#pragma HLS INTERFACE axis port=pixels1
#pragma HLS INTERFACE axis port=pixels2
#pragma HLS INTERFACE axis port=pixels3
#pragma HLS INTERFACE m_axi port=background max_widen_bitwidth=32 offset=slave bundle=background
#pragma HLS INTERFACE m_axi port=foreground max_widen_bitwidth=32 offset=slave bundle=foreground
#pragma HLS INTERFACE s_axilite port=background bundle=control
#pragma HLS INTERFACE s_axilite port=foreground bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

	int         row, lcd, col;
	uint32_t    fgLineBuff[240];
	uint32_t    bgLineBuff[240];
	ap_uint<32> fgInt;
	ap_uint<32> bgInt;
	ap_uint<8>  bgr, bgg, bgb;
	ap_uint<8>  fgr, fgg, fgb, fga;
	ap_uint<8>  blr, blg, blb;
	ap_uint<16> pixel;

	for (row = 0; row < 240; row++) {
		for (lcd = 0; lcd < 4; lcd++) {
			memcpy (bgLineBuff, background + lcd*57600 + row*240, 4*240);
			memcpy (fgLineBuff, foreground + lcd*57600 + row*240, 4*240);
			for (col = 0; col < 240; col++) {
				// pick background pixel from background line buffer
				bgInt = bgLineBuff[col];

				// pick foreground pixel from foreground line buffer
				fgInt = fgLineBuff[col];

				// pull out r, g, b, a channels from background pixel
				bgr = bgInt.range(31,24);
				bgg = bgInt.range(23,16);
				bgb = bgInt.range(15, 8);

				// pull out r, g, b, a channels from foreground pixel
				fgr = fgInt.range(31,24);
				fgg = fgInt.range(23,16);
				fgb = fgInt.range(15, 8);
				fga = fgInt.range( 7, 0);

				float alpha = (float)fga / 255.0;

				blr = ((1.0 - alpha) * bgr + alpha * fgr);
				blg = ((1.0 - alpha) * bgg + alpha * fgg);
				blb = ((1.0 - alpha) * bgb + alpha * fgb);

				// convert blended output to r5g6b5 pixel
				pixel.range(15,11) = blr.range(7,3); // 5 bits
				pixel.range(10, 5) = blg.range(7,2); // 6 bits
				pixel.range( 4, 0) = blb.range(7,3); // 5 bits

				// write pixel to output
				if (lcd == 0) pixels0.write (pixel);
				else if (lcd == 1) pixels1.write (pixel);
				else if (lcd == 2) pixels2.write (pixel);
				else if (lcd == 3) pixels3.write (pixel);
			}
		}
	}

	return 0;
}

