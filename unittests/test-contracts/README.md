test_ram_limit contract was compiled with enumivo.cdt v1.4.1

That contract was ported to compile with enumivo.cdt v1.5.0, but the test that uses it is very sensitive to stdlib/enulib changes, compilation flags and linker flags.

The remaining contracts have been ported to compile with enumivo.cdt v1.6.x. They were compiled with a patched version of enumivo.cdt v1.6.0-rc1 (commit 1c9180ff5a1e431385180ce459e11e6a1255c1a4).
