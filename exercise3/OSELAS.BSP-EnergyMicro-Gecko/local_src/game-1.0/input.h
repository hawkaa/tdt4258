#ifndef INPUT_H_
#define INPUT_H_

extern int
input_init(void);

extern void
process_input(void);

extern void
register_button_down_handler(int button, void (*handler)(void));

#endif
