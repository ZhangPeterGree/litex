// SPDX-License-Identifier: BSD-2-Clause
// HDMI framebuffer color bars (LiteX VideoFrameBuffer rgb888: R bits 0–7, G 8–15, B 16–23).

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include <generated/csr.h>
#include <system.h>

void hdmi_pattern_cmd(void);

#ifndef VIDEO_FRAMEBUFFER_HRES

void hdmi_pattern_cmd(void)
{
	puts("This SoC has no video framebuffer (rebuild SoC with framebuffer).\n");
}

#else

void hdmi_pattern_cmd(void)
{
	volatile uint32_t *fb =
		(volatile uint32_t *)(uintptr_t)VIDEO_FRAMEBUFFER_BASE;
	const int W = VIDEO_FRAMEBUFFER_HRES;
	const int H = VIDEO_FRAMEBUFFER_VRES;
	int x, y;

	printf("Drawing %dx%d color bars (LiteX rgb888)...\n", W, H);

	for (y = 0; y < H; y++) {
		for (x = 0; x < W; x++) {
			uint32_t c;

			if (x < W / 8)
				c = 0x000000ffU; /* red   */
			else if (x < W * 2 / 8)
				c = 0x0000ff00U; /* green */
			else if (x < W * 3 / 8)
				c = 0x00ff0000U; /* blue  */
			else if (x < W * 4 / 8)
				c = 0x0000ffffU; /* yellow */
			else if (x < W * 5 / 8)
				c = 0x00ffff00U; /* cyan */
			else if (x < W * 6 / 8)
				c = 0x00ff00ffU; /* magenta */
			else if (x < W * 7 / 8)
				c = 0x00ffffffU; /* white */
			else
				c = 0x00000000U; /* black */

			fb[y * W + x] = c;
		}
	}

#ifdef CONFIG_CPU_HAS_DCACHE
	flush_cpu_dcache_range((void *)(uintptr_t)VIDEO_FRAMEBUFFER_BASE,
		(size_t)W * (size_t)H * 4U);
#endif
	puts("Done.\n");
}

#endif
