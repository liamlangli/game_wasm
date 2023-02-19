.PHONY: all editor

CC = clang -Os \
	--target=wasm32-wasi \
	--no-standard-libraries \
	-nostartfiles \
	-Wl,--import-memory \
	-Wl,--no-entry \
	-Wl,--export-all \

all: clean editor jump

clean:
	rm -f public/wasm/*.wasm

jump:
	$(CC) -o public/wasm/jump.wasm core.c example/jump.c
	wasm2wat ./public/wasm/jump.wasm -o ./public/wasm/jump.wast

editor:
	$(CC) -o public/wasm/editor.wasm core.c editor/editor.c
	wasm2wat ./public/wasm/editor.wasm -o ./public/wasm/editor.wast