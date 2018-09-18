#this program uses the value stored in register t0 
    #and calculated the Fibonaccci number from it 
        addi $a0, $t0, 0x0000       #store the number in a0 = 10
        andi $a1, $a1, 0x0000       #set a1 to 0 so we can have a counter 
        andi $t1, $t1, 0x0000       #clear t0 register to hold our value 
        andi $t2, t2, 0x00001       #F1 of fibonacci sequence 
        andi $t3, %t3, 0x0001       #F2 of fibonocci sequence 
        beq $a0, $a1, DONE          #if the counter is equal to the jump to end 
        add $t1, $t1, $t2           #add the previous number of sequence



DONE:   #answer in t0
