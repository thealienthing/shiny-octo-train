#include "../../src/LCDScreen.h"
#include <ncurses.h>

class CursesScreen : public Screen {
public:
    void init() override;
    void send_string(char *str);
    void put_cur(int row, int col) override;
    void cursor_setup(bool cur_on, bool blink_on) override;
    void clear() override;
};