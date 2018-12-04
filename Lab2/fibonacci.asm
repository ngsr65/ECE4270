#this program uses the value stored in register t0 
    #and calculated the Fibonaccci number from it 
        addi $a0, $t0, 0     ;0x0000 
		andi $a1, $a1, 3     ;0x0004
        andi $t1, $t1, 1     ;0x0008
        andi $t2, $t2, 1     ;0x000c 12   
        andi $t3, $t3, 1     ;0x000F 16 
        beq $a0, $a1, 44     ;0x0014 20  
        add $t1, $t2, $t3    ;0x0018 24 
		and $t2, $t3, $t3	 ;0x001C 28		
		and $t3, $t1, $t1	 ;0x001F 32
		addi $a1, $a1, 1	 ;0x0024 36	
		j 20				 ;0x0028 40
		syscall 			 ;0x002C 44
