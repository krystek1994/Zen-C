# Zen C TODO

## Codegen
- [ ] Migrate remaining control instructions in codegen_stmt.c to jump tables (break, continue, return, asm, loops).

## Architecture & Modularity
- [ ] Divide the typechecker (src/analysis/typecheck.c) into several files.
- [ ] Split up parser_expr.c into a new folder (src/parser/expr/) with separate files for binary, unary, etc.

## Features
- [ ] Improve traits.
- [ ] Improve comptime mechanics.

## Cleanup
- [ ] Fix residual template and defer scope tracking bugs.
- [ ] Clear temporary build files and logs.
- [ ] Reduce function complexity hotspot warnings.
