.text
.global exchange
exchange:
    pushl %ebp      #prologue
    movl %esp, %ebp
    pushl %esi      #save registers
    pushl %edi
    pushl %ebx

    movl $(-1), %esi    #esi is left index
                        #edx is right index
    xorl %ecx, %ecx
  .L0:
    incl %esi
    decl %edx
    cmpl %edx, %esi             #check end of cycle
    jae .end
    movl (%eax, %edx, 4), %edi
    cmpl %edi, (%eax, %esi, 4)      #check swap
    jbe .L0
    movl (%eax, %esi, 4), %ebx      #swap
    movl %edi, (%eax, %esi, 4)
    movl %ebx, (%eax, %edx, 4)
    incl %ecx
    jmp .L0

  .end:
    pop %ebx        #retrieve registers
    pop %edi
    pop %esi
    leave           #epilogue
    ret
