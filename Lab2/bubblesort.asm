	#Put gp value into gp register
lui $gp, 1001
	
	#Load values into memory and into registers
addiu $t0, $zero, 5
sw $t0, 0($gp)
addiu $t1, $zero, 3
sw $t1, 4($gp)
addiu $t2, $zero, 6
sw $t2, 8($gp) 
addiu $t3, $zero, 8
sw $t3, C($gp)			#20
addiu $t4, $zero, 9
sw $t4, 10($gp)
addiu $t5, $zero, 1 
sw $t5, 14($gp)
addiu $t6, $zero, 4
sw $t6, 18($gp)
addiu $t7, $zero, 7
sw $t7, 1C($gp) 		#40
addiu $t8, $zero, 2
sw $t8, 20($gp)
addiu $t9, $zero, A
sw $t9, 24($gp)			#50

	#Set jump value
addiu $s0, $zero, 58	#PC = 0x54

	#Start comparing
sub $s1, $t1, $t0	#58		if [t1] < [t0], swap
blez $s1, 54		#Swap
sub $s1, $t2, $t1	#60
blez $s1, 5C
sub $s1, $t3, $t2	#68
blez $s1, 64
sub $s1, $t4, $t3	#70
blez $s1, 6C
sub $s1, $t5, $t4	#78
blez $s1, 74
sub $s1, $t6, $t5	#80
blez $s1, 7C
sub $s1, $t7, $t6	#88
blez $s1, 84
sub $s1, $t8, $t7	#90
blez $s1, 8C
sub $s1, $t9, $t8	#98
blez $s1, 94
sub $s0, $s0, 8 #a0
addiu $s2, $zero, 8
beq $s0, $s2, 98	#done sorting
j 58    			#next loop
addiu $s2, $t0, 0 #b0
addiu $t0, $t1, 0
addiu $t1, $s2, 0
j 60
addiu $s2, $t1, 0
addiu $t1, $t2, 0
addiu $t2, $s2, 0
j 68
addiu $s2, $t2, 0
addiu $t2, $t3, 0
addiu $t3, $s2, 0
j 70
addiu $s2, $t3, 0
addiu $t3, $t4, 0
addiu $t4, $s2, 0
j 78
addiu $s2, $t4, 0 #f0
addiu $t4, $t5, 0
addiu $t5, $s2, 0
j 80
addiu $s2, $t5, 0 #100
addiu $t5, $t6, 0
addiu $t6, $s2, 0
j 88
addiu $s2, $t6, 0 #110
addiu $t6, $t7, 0
addiu $t7, $s2, 0
j 90
addiu $s2, $t7, 0 #120
addiu $t7, $t8, 0
addiu $t8, $s2, 0
j 98
addiu $s2, $t8, 0 #130
addiu $t8, $t9, 0
addiu $t9, $s2, 0
j a0
sw $t0, 0($gp)			#140	#Save sorted values
sw $t1, 4($gp)
sw $t2, 8($gp) 
sw $t3, C($gp)
sw $t4, 10($gp)
sw $t5, 14($gp)
sw $t6, 18($gp)
sw $t7, 1C($gp) 
sw $t8, 20($gp)
sw $t9, 24($gp)					
syscall 