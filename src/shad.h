#pragma once

struct shad_opts {
    unsigned int canvas_width;
    unsigned int canvas_height;
    const char *filename;
};

void init(const struct shad_opts opts);
void run(void);
void quit(void);
