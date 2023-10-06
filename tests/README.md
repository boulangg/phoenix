# How to run the unit tests

For now it's only possible to run them from the Linux machine.
- In VS 2022:
  - Build libunittestsuite.so with linux-default-build configuration for WSL
- In Ubuntu terminal:
  - go to the `~/.vs/phoenix/build/linux-default/tests`
  - run `make test` or directly with `./dependency/UnitTestFramework/unittest_runner ./libunittestsuite.so`

To debug the unit test:
- `gdb ./dependency/UnitTestFramework/unittest_runner`
- `b main` or `b <unit_test_name>`
- `r ./libunittestsuite.so`
