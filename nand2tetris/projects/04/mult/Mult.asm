// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

  // value = R0
  @R0
  D=M
  @value
  M=D

  // multiple = R1
  @R1
  D=M
  @multiple
  M=D

  // result = 0
  @result
  M=0

(LOOP)
  @multiple
  D=M
  @FINISH
  D;JLE // if multiple <= 0; goto finish

  // result += value
  @value
  D=M
  @result
  M=D+M

  // multiple -= 1
  @multiple
  M=M-1

  @LOOP
  0;JMP

(FINISH)
  // R2 = result
  @result
  D=M
  @R2
  M=D

(END)
  @END
  0;JMP
