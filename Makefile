CC = clang -Ofast \
	--target=wasm32-wasi \
	--no-standard-libraries \
	-nostartfiles \
	-Wl,--import-memory \
	-Wl,--no-entry \
	-Wl,--export-all \

jump:
	$(CC) -o public/wasm/jump.wasm core.c example/jump.c
	wasm2wat ./public/wasm/jump.wasm -o ./public/wasm/jump.wast

all: jump
