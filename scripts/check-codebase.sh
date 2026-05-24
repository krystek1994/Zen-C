#!/bin/bash
# Enforces codebase-wide conventions. Run via: make check-codebase
# NOTE: pipefail intentionally NOT set. The while(1) check uses
# tail | head | grep pipelines where SIGPIPE is expected.
set +o pipefail

ROOT="$(dirname "$0")/.."
err_count=0
warn_count=0

red()    { printf "\033[31m%s\033[0m\n" "$*"; }
green()  { printf "\033[32m%s\033[0m\n" "$*"; }
yellow() { printf "\033[33m%s\033[0m\n" "$*"; }

warn() { warn_count=$((warn_count + 1)); yellow "  [WARN] $*"; }
err()  { err_count=$((err_count + 1));   red    "  [FAIL] $*"; }

check_header() { echo ""; echo "--- $* ---"; }

# ===========================================================================
# Rule 1: while(1) loops in parser must guard TOK_EOF
# Accept TOK_EOF, break; after zpanic_at, or implicit break on non-match.
# ===========================================================================
check_header "Rule 1: while(1) loops must guard TOK_EOF"
while IFS= read -r line; do
    f=$(echo "$line" | cut -d: -f1)
    ln=$(echo "$line" | cut -d: -f2)
    body=$(tail -n +"$ln" "$f" | head -40)
    # Accept: explicit TOK_EOF, or break; (safe termination)
    if echo "$body" | grep -q 'TOK_EOF'; then continue; fi
    if echo "$body" | grep -qE '^\s+break;\s*$'; then continue; fi
    warn "$f:$ln"
done < <(grep -rn 'while (1)' "$ROOT/src/parser" "$ROOT/src/analysis" 2>/dev/null)

# ===========================================================================
# Rule 2: No exit(1) outside whitelisted locations
# ===========================================================================
check_header "Rule 2: exit(1) outside whitelist"
while IFS= read -r line; do
    case "$line" in
        *zfatal*)                               continue ;;
        *diagnostics.c*)                        continue ;;
        *utils_plugins.c*)                      continue ;;
        *"// whitelisted"*)                     continue ;;
        *lsp/lsp_project.c*)                    continue ;;
        *codegen_decl_emit.c*|\
        *codegen_decl_preamble.c*)              continue ;;
        *"/* exit */"*)                          continue ;;
    esac
    err "$line"
done < <(grep -rn 'exit(1)' "$ROOT/src" 2>/dev/null)

# ===========================================================================
# Rule 3: No strdup() — use xstrdup()
# ===========================================================================
check_header "Rule 3: strdup() should use xstrdup"
while IFS= read -r line; do
    case "$line" in
        *xstrdup*)  continue ;;
        *define*)   continue ;;
        *'"'*strdup*'"'*) continue ;;
    esac
    err "$line"
done < <(grep -rn '\bstrdup\b' "$ROOT/src" 2>/dev/null)

# ===========================================================================
# Rule 4: Use TOKEN_UNKNOWN not bare (Token){0}
# ===========================================================================
check_header "Rule 4: (Token){0} should use TOKEN_UNKNOWN"
while IFS= read -r line; do
    case "$line" in
        *TOKEN_UNKNOWN*)         continue ;;
        *"= {0}"*)               continue ;;
        *token.h*)               continue ;;
        *".o:"*)                 continue ;;
        *zen_facts.c*)           continue ;;
    esac
    err "$line"
done < <(grep -rn '(Token){0}' "$ROOT/src" 2>/dev/null)

# ===========================================================================
# Rule 5: Plugin Token must set .col
# ===========================================================================
check_header "Rule 5: Plugin Token.col must be set"
while IFS= read -r line; do
    f=$(echo "$line" | cut -d: -f1)
    ln=$(echo "$line" | cut -d: -f2)
    if ! tail -n +"$ln" "$f" | head -5 | grep -q '\.col'; then
        err "$f:$ln"
    fi
done < <(grep -rn 'Token t = {0};' "$ROOT/src/plugins" 2>/dev/null)

# ===========================================================================
# Rule 6: No zfree() on realpath() or malloc() allocations
# realpath() returns system-heap memory which must be freed with free(),
# not zfree() (which is a no-op for arena memory).
# ===========================================================================
check_header "Rule 6: zfree() on realpath() pointers"
if grep -rn 'zfree(real_path\|zfree(realpath' "$ROOT/src" 2>/dev/null; then
    err "zfree() used on realpath() return value — must use free()"
fi

# ===========================================================================
# Rule 7: No g_compiler or g_config access in parser/analysis/codegen
# These should go through ctx->compiler or ctx->config for thread safety.
# ===========================================================================
check_header "Rule 7: g_compiler/g_config in parser/analysis/codegen"
while IFS= read -r line; do
    case "$line" in
        *".h:"*)        continue ;;  # header declarations
        *"g_compiler"*"config"*) ;;
        *)              continue ;;
    esac
    warn "$line"
done < <(grep -rn 'g_compiler\.\|g_config\.' "$ROOT/src/parser" "$ROOT/src/analysis" "$ROOT/src/codegen" 2>/dev/null)

# ===========================================================================
# Rule 8: parse_type_formal return value should be NULL-checked
# ===========================================================================
check_header "Rule 8: parse_type_formal() NULL check (heuristic)"
while IFS= read -r line; do
    f=$(echo "$line" | cut -d: -f1)
    ln=$(echo "$line" | cut -d: -f2)
    # Skip the definition and header declarations
    case "$line" in
        *"Type \*parse_type_formal"*) continue ;;
        *".h:"*)                      continue ;;
        *"Type *parse_type_formal("*) continue ;;
    esac
    # Check next 3 lines for a NULL check pattern
    context=$(tail -n +"$ln" "$f" | head -5)
    if ! echo "$context" | grep -qE 'if\s*\([!&]|== NULL|!= NULL|!\w+\s*\)|[?]|parse_type_formal\(ctx, l\)\s*==\s*NULL'; then
        warn "$f:$ln"
    fi
done < <(grep -rn 'parse_type_formal(' "$ROOT/src/parser" "$ROOT/src/analysis" "$ROOT/src/codegen" 2>/dev/null)

# ===========================================================================
# Summary
# ===========================================================================
echo ""
if [ "$err_count" -eq 0 ]; then
    if [ "$warn_count" -eq 0 ]; then
        green "All checks passed."
    else
        yellow "$warn_count warning(s). $err_count error(s)."
        green "All required checks passed."
    fi
    exit 0
else
    red "$err_count violation(s) found (must fix). $warn_count warning(s)."
    exit 1
fi
