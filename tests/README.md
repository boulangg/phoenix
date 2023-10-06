# How to run the unit tests

For now it's only possible to run them from the Linux machine.
- In VS 2022:
  - Build **libunittestsuite.so** with linux-default-build configuration for WSL
- In Ubuntu terminal:
  - go to the `~/.vs/phoenix/build/linux-default/tests`
  - run `make test` or directly with `./dependency/UnitTestFramework/unittest_runner ./libunittestsuite.so`

To debug the unit test, it is doable directly in VS but you can use gdb as well
- In VS 2022:
  - Run **_debug_unit_test** target with linux-default-build configuration for WSL
- Alternatively in Ubuntu terminal
  - `gdb ./dependency/UnitTestFramework/unittest_runner`
  - `b main` or `b <unit_test_name>` (might be available only after the first run so the unit test DLL symbol are loaded in GDB)
  - `r ./libunittestsuite.so`
