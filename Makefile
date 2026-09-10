CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS ?= -ldl -rdynamic
BIN := jester
PLUGIN_DIR := build/plugins
PLUGINS := $(patsubst builtins/%.c,$(PLUGIN_DIR)/%.so,$(wildcard builtins/*.c))

all: $(BIN) plugins
$(BIN): src/LPIM.c src/RRC.c
	$(CC) $(CFLAGS) -o $@ src/LPIM.c $(LDFLAGS)
plugins: $(PLUGINS)
$(PLUGIN_DIR)/%.so: builtins/%.c | $(PLUGIN_DIR)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<
$(PLUGIN_DIR):
	mkdir -p $@
package: all
	rm -rf .package && mkdir -p .package
	cp $(BIN) run.sh .package/
	cp -R builtins .package/builtins
	cp -R .jesterlibs_template .package/
	(cd .package && zip -qr ../jester.zip .)

test: all
	./run.sh examples/bot.jr
clean:
	rm -f $(BIN) jester.zip
	rm -rf build .package
.PHONY: all plugins package test clean
