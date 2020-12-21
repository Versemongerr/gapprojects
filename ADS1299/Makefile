# User Test
#------------------------------------

APP              = FFT
APP_SRCS        += ./SRC/test.c ./SRC/ADS1299.c ./SRC/USER_SPI.c ./SRC/USER_UART.c
APP_INC	        += ./INC/.
APP_CFLAGS      += -O3 -g

PMSIS_OS = pulpos

# PLPBRIDGE_FLAGS += -f ./SRC/test.c ./SRC/ADS1299.c ./SRC/USER_SPI.c ./SRC/USER_UART.c -hyper

include $(RULES_DIR)/pmsis_rules.mk