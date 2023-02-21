Game WebAssembly
------------------------

Remaster old school games with WebAssembly.

# Build
- clone llvm project `git clone https://github.com/llvm/llvm-project.git`   
    - [Optional] On Windows Platform `git clone --config core.autocrlf=false  https://github.com/llvm/llvm-project.git`

- Build Tools install    
    - Create build Folder   
    `cd llvm-project`    
    `mkdir build && cd build  `   
    - Configurate llvm project   
        - MacOS or Linux   
        `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" ../llvm`   
        - windows   
        `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="D:\Program Files\LLVM\bin" -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" -G "Visual Studio 16 2019" ../llvm`  
    - Build   
    `make && make install`

- Build example [run dev server and listen at port 3000]   
    `git clone git@github.com:liamlangli/game_wasm.git`    
    `cd game_wasm`   
    `make`
    `yarn && yarn`
