#include "file_operations.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void log_event(const char* format, ...) {
    FILE* f = fopen("logs/chess.log", "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(f, "[%s] ", time_str);
    va_list args; // need to iterate arguments after format
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
}

int save_game(const ChessState* state, const char* filepath) {
    FILE* f = fopen(filepath, "wb"); //write binary
    if (!f) return 0;
    int ok = (fwrite(state, sizeof(ChessState), 1, f) == 1);
    fclose(f);
    if (ok) log_event("Game saved to %s", filepath);
    return ok;
}

int load_game(ChessState* state, const char* filepath) {
    FILE* f = fopen(filepath, "rb");  //read binary
    if (!f) return 0;
    int ok = (fread(state, sizeof(ChessState), 1, f) == 1);
    fclose(f);
    if (ok) log_event("Game loaded from %s", filepath);
    return ok;
}
