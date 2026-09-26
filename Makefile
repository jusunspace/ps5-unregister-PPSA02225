PS5_HOST ?= ps5
PS5_PORT ?= 9021

ifdef PS5_PAYLOAD_SDK
include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
else
$(error PS5_PAYLOAD_SDK is undefined)
endif

BIN := unregister-PPSA02225.elf
CFLAGS := -Os -Wall -Wextra
LDADD := -lSceIpmi -lSceAppInstUtil

all: $(BIN)

$(BIN): main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDADD)

clean:
	rm -f $(BIN)

test: $(BIN)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^

.PHONY: all clean test
