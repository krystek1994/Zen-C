window.BENCHMARK_DATA = {
  "lastUpdate": 1793200231410,
  "repoUrl": "https://github.com/krystek1994/Zen-C",
  "entries": {
    "Zen Compiler Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "zuhaitz.zechhub@gmail.com",
            "name": "Zuhaitz"
          },
          "committer": {
            "email": "zuhaitz.zechhub@gmail.com",
            "name": "Zuhaitz"
          },
          "distinct": true,
          "id": "b3f550e938be3fd395e9ebf657669f89dbb3453b",
          "message": "fix(tests): update unit AST/LSP tests after syntax removal\n\nThe unless node was removed from the AST, but the ast_children test still created NODE_UNLESS and the LSP index test used the old column for 'unless v'. Use NODE_IF and the new column; all 50 unit tests pass.",
          "timestamp": "2026-10-25T10:06:59+02:00",
          "tree_id": "c5df43c37cf45ca39a2b87fcae8d4049d1fa7075",
          "url": "https://github.com/krystek1994/Zen-C/commit/b3f550e938be3fd395e9ebf657669f89dbb3453b"
        },
        "date": 1793200231398,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Compiler (Full Suite Transpilation)",
            "value": 25717,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_bigint_factorial)",
            "value": 2,
            "unit": "ms"
          },
          {
            "name": "Compile (test_bigint_factorial)",
            "value": 191,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_comptime)",
            "value": 0,
            "unit": "ms"
          },
          {
            "name": "Compile (test_comptime)",
            "value": 50,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_fib_recursive)",
            "value": 66,
            "unit": "ms"
          },
          {
            "name": "Compile (test_fib_recursive)",
            "value": 47,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_hashmap)",
            "value": 105,
            "unit": "ms"
          },
          {
            "name": "Compile (test_hashmap)",
            "value": 83,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_hello)",
            "value": 0,
            "unit": "ms"
          },
          {
            "name": "Compile (test_hello)",
            "value": 48,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_json_parse)",
            "value": 195,
            "unit": "ms"
          },
          {
            "name": "Compile (test_json_parse)",
            "value": 768,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_mandelbrot)",
            "value": 17,
            "unit": "ms"
          },
          {
            "name": "Compile (test_mandelbrot)",
            "value": 50,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_sha256)",
            "value": 98,
            "unit": "ms"
          },
          {
            "name": "Compile (test_sha256)",
            "value": 603,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_sort_large)",
            "value": 53,
            "unit": "ms"
          },
          {
            "name": "Compile (test_sort_large)",
            "value": 77,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_string_concat)",
            "value": 1,
            "unit": "ms"
          },
          {
            "name": "Compile (test_string_concat)",
            "value": 391,
            "unit": "ms"
          },
          {
            "name": "Runtime (test_vec_ops)",
            "value": 11,
            "unit": "ms"
          },
          {
            "name": "Compile (test_vec_ops)",
            "value": 126,
            "unit": "ms"
          }
        ]
      }
    ]
  }
}