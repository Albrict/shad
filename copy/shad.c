#include "shad.h"

int main ()
{
    init_shad();
    while (true) {
        procces_input();
        update();
    }
    return 0;
}
