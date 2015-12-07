// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// set totalwords var amount of allowed "words"
// used to determine if screen is filled yet
@8192 // number of words for the screen I/O map (32*256)
D=A
@totalwords
M=D

// sets initial variables and starts listening
@RESET
0;JMP

(LISTEN)
  @KBD
  D=M
  @BLACK
  D;JGT // if key pressed, color black
  @WHITE
  0;JMP // else color white

// set color to black and fill
(BLACK)
  @color
  M=-1
  @FILL
  0;JMP

// set color to white and fill
(WHITE)
  @color
  M=0
  @FILL
  0;JMP

// fills @word to @color
(FILL)
  @word
  D=M
  @totalwords
  D=M-D // D = words remaining
  @RESET
  D;JEQ // if words remaining == 0, screen full, reset and listen

  // addr = screen + word pos
  @word
  D=M
  @SCREEN
  D=A+D
  @addr // memory address to fill
  M=D

  // set addr to color
  @color
  D=M
  @addr
  A=M
  M=D

  @word
  M=M+1 // word += 1

  @FILL
  0;JMP // recurse to fill whole screen

(RESET)
  @word
  M=0
  @LISTEN
  0;JMP
