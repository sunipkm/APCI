all: isp_fpga test_idio8 axiodac axiodac_irq pcie_dio_48s mpcie_quad_8 mpcie-ii-16-irq pcie-idio-24-irq mpcie-dio-24s-irq mpcie_aio16_16f_dma mpcie_aio16_16f pcie_iiro_16_irq irq check check_dma dac

ifneq ("$(wildcard /etc/redhat-release)","")
    REDHAT_VERSION = $(shell cat /etc/redhat-release | grep  -o "[0-9]*" | head -1)
else
    REDHAT_VERSION = 0
endif

ifeq ($(REDHAT_VERSION), 7)
	GCC = gcc -std=c11 -D_XOPEN_SOURCE=700 -D _BSD_SOURCE
else
	GCC = gcc
endif

isp_fpga: isp-fpga.c apcilib.h apcilib.c
	$(GCC) -o isp_fpga isp-fpga.c apcilib.c -lm -lpthread -O3

test_idio8: test-idio8.c apcilib.h apcilib.c
	$(GCC) -o test_idio8 test-idio8.c apcilib.c -lm -lpthread -O3

axiodac_irq: axiodac_irq.c apcilib.h apcilib.c
	$(GCC) -o axiodac_irq axiodac_irq.c apcilib.c -lm -lpthread

axiodac: axiodac.c apcilib.h apcilib.c
	$(GCC) -o axiodac axiodac.c apcilib.c -lm -lpthread -O3

pcie_dio_48s: pcie-dio-48s-irq.c apcilib.h apcilib.c
	$(GCC) -o pcie_dio_48s pcie-dio-48s-irq.c apcilib.c

mpcie_quad_4:mpcie_quad_8.c apcilib.h apcilib.c
	$(GCC) -o mpcie_quad_8 mpcie_quad_8.c apcilib.c

mpcie_quad_8: mpcie_quad_8.c apcilib.h apcilib.c
	$(GCC) -o mpcie_quad_8 mpcie_quad_8.c apcilib.c

pcie_iiro_16_irq: pcie-iiro-16.c apcilib.h apcilib.c
	$(GCC) -o pcie_iiro_16_irq pcie-iiro-16.c apcilib.c -lm -lpthread -O3

irq: irq.c apcilib.h apcilib.c
	$(GCC) -o irq irq.c apcilib.c -lm -lpthread -O3

mpcie_iiro_8: mpcie-iiro-8.c apcilib.h apcilib.c
	$(GCC) -o mpcie-iiro-8 mpcie-iiro-8.c apcilib.c -lm -lpthread -O3

mpcie-ii-16-irq: mpcie-ii-16-irq.c apcilib.h apcilib.c
	$(GCC) -o mpcie-ii-16-irq mpcie-ii-16-irq.c apcilib.c

pcie-idio-24-irq: pcie-idio-24-irq.c apcilib.h apcilib.c
	$(GCC) -o pcie-idio-24-irq pcie-idio-24-irq.c apcilib.c

mpcie-dio-24s-irq: mpcie-dio-24s-irq.c apcilib.h apcilib.c
	$(GCC) -o mpcie-dio-24s-irq mpcie-dio-24s-irq.c apcilib.c

mpcie_aio16_16f_dma: mpcie_aio16_16f_dma.c apcilib.h apcilib.c
	$(GCC) -o mpcie_aio16_16f_dma mpcie_aio16_16f_dma.c apcilib.c -lm -lpthread -O3

mpcie_aio16_16f: mpcie_aio16_16f.c apcilib.h apcilib.c
	$(GCC) -o mpcie_aio16_16f mpcie_aio16_16f.c apcilib.c -lm -lpthread -O3

check: check.c apcilib.h apcilib.c
	$(GCC) -o check check.c apcilib.c -lm -lpthread -O3

check_dma: check_dma.c apcilib.h apcilib.c
	$(GCC) -g -o check_dma check_dma.c apcilib.c -lm -lpthread -O3

dac: dac.c apcilib.h apcilib.c
	$(GCC) -o dac dac.c apcilib.c


clean:
	rm isp_fpga test_idio8 axiodac axiodac_irq pcie_dio_48s mpcie_quad_8 mpcie-ii-16-irq pcie-idio-24-irq mpcie-dio-24s-irq mpcie_aio16_16f_dma mpcie_aio16_16f pcie_iiro_16_irq irq check check_dma dac

