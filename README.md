# RiscV simulator

### Build and run

```
mkdir build && cd build
cmake .. && make -j4

/BUILD_DIR/utils/tests_instructions - tests of creating instructions
/BUILD_DIR/stages/tests_stages - tests of each stage
/BUILD_DIR/tests_pipeline - tests of supported scenarious
```

`/tests/tests_pipeline.cpp` contains supported scenarious. Simulator is not supporting branches currently.

Almost all sources are located now in .h files.

Contant:

1. `/elements` - units used in programm
2. `/stages` - pipeline stages
3. `/utils` - instruction creator (ins.h), control registers (registers.h), some helper structures (simulation_state.h)
