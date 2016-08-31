file gen/msp430fr5969.out
target remote 127.0.0.1:2000
break bsp_setup.c:BSP_init
load gen/msp430fr5969.out
c