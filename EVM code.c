#include <reg51.h>

// Function prototypes
void lcd_init();
void lcd_command(unsigned char command);
void lcd_data(unsigned char data);
void lcd_print(const char *string);
void keypad_init();
unsigned char keypad_scan();

void main() {
  unsigned char vote;

  lcd_init();
  keypad_init();

  while (1) {
    lcd_command(0x01);  // Clear display
    lcd_print("Vote: 1-Candidate1");
    lcd_command(0xC0);  // Move cursor to the second line
    lcd_print("2-Candidate2");

    vote = keypad_scan();

    if (vote == '1') {
      lcd_command(0x01);  // Clear display
      lcd_print("Thank you for");
      lcd_command(0xC0);  // Move cursor to the second line
      lcd_print("voting! -Candidate1");
    } else if (vote == '2') {
      lcd_command(0x01);  // Clear display
      lcd_print("Thank you for");
      lcd_command(0xC0);  // Move cursor to the second line
      lcd_print("voting! -Candidate2");
    } else {
      lcd_command(0x01);  // Clear display
      lcd_print("Invalid vote!");
    }
  }
}

void lcd_init() {
  lcd_command(0x38);  // 2 lines, 5x7 matrix
  lcd_command(0x0C);  // Display on, cursor off
  lcd_command(0x06);  // Increment cursor
  lcd_command(0x01);  // Clear display
}

void lcd_command(unsigned char command) {
  P2 = command;
  P1 &= ~0x01;  // RS = 0 for command
  P1 &= ~0x02;  // RW = 0 for write
  P1 |= 0x04;   // EN = 1 to enable
  P1 &= ~0x04;  // EN = 0 to latch command
}

void lcd_data(unsigned char data) {
  P2 = data;
  P1 |= 0x01;   // RS = 1 for data
  P1 &= ~0x02;  // RW = 0 for write
  P1 |= 0x04;   // EN = 1 to enable
  P1 &= ~0x04;  // EN = 0 to latch data
}

void lcd_print(const char *string) {
  while (*string) {
    lcd_data(*string++);
  }
}

void keypad_init() {
  P1 = 0xF0;     // Configure columns as inputs and rows as outputs
  P2 = 0x0F;     // Enable internal pull-ups for columns
}

unsigned char keypad_scan() {
  unsigned char row, col;
  unsigned char keys[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
  };

  while (1) {
    for (col = 0; col < 4; col++) {
      P1 = ~(0x01 << col);  // Ground one column at a time
      for (row = 0; row < 4; row++) {
        if (!(P1 & (0x10 << row))) {
          while (!(P1 & (0x10 << row)));  // Wait until key is released
          return keys[row][col];  // Return the pressed key
        }
      }
    }
  }
}
