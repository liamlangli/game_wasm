Game WebAssembly
------------------------

Remaster old school games with WebAssembly.

# Build
- clone llvm project `git clone https://github.com/llvm/llvm-project.git`   
    - [Optional] On Windows Platform `git clone --config core.autocrlf=false  https://github.com/llvm/llvm-project.git`

- Build Tools install   
    `cd llvm-project`    
    `mkdir build && cd build  ` 
    `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_PROJECTS="lld" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" ../llvm`
    `make && make install`

- Build example   
    `make`
