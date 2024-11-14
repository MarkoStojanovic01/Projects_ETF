
.global handler
.section my_code
handler:
  ld $0 ,%r1
  ld $4, %r2
  ld $array, %r3
  ld $3, %r4
  ld $1, %r5
label1:
  beq %r1, %r2, label2
  st %r4, [%r3+1] 
  add %r4, %r4
  add %r5, %r3
  add %r5, %r1
  jmp label1
label2:
  halt
.section data
array:
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
.end
