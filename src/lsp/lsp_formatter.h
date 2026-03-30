#ifndef LSP_FORMATTER_H
#define LSP_FORMATTER_H

/**
 * @brief Formats Zen C source code.
 *
 * @param src The original source code.
 * @return char* The formatted source code (must be freed by caller).
 */
char *lsp_format_source(const char *src);

#endif
