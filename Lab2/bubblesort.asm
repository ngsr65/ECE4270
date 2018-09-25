#Store the array in memory

addiu $t1, $zero, 5 #0
sw $t1, 0($gp)
addiu $t1, $zero, 3
sw $t1, 4($gp)
addiu $t1, $zero, 6
sw $t1, 8($gp) #20
addiu $t1, $zero, 8
sw $t1, 12($gp)
addiu $t1, $zero, 9
sw $t1, 16($gp)
addiu $t1, $zero, 1 #40
sw $t1, 20($gp)
addiu $t1, $zero, 4
sw $t1, 24($gp)
addiu $t1, $zero, 7
sw $t1, 28($gp) #60
addiu $t1, $zero, 2
sw $t1, 32($gp)
addiu $t1, $zero, 10
sw $t1, 36($gp)

	#Put 0 in $t0 and 9 in $t1 as an indexes

addiu $t0, $zero, 0 #80
addiu $t1, $zero, 8

	#Get starting address
addiu $t2, $zero, 0


	#Check if we're at the top
beq $t0, $t1, 148

	#If not, load in numbers
lw $t4, $t2($gp)
addiu $t3, $t2, 4		#100
lw $t5, $t3($gp)

	#Compare numbers
sub $t6, $t4, $t5
blez $t6, 128			#$t4 is smaller, so jump to 128

	#$t5 is bigger
addiu $t0, $t0, 1
addiu $t2, $t2, 4		#120
j 92					#Jumps to spot 92


	#, Swap numbers
sw $t4, $t3($gp)
sw $t5, $t2($gp)

addiu $t0, $t0, 1
addiu $t2, $t2, 4		#140
j 92					#Jumps to spot 92

sub $t1, $t1, 1			#Remove last index
beq $t1, 0, 168 		#check if done

addiu $t0, $zero, 0
addiu $t2, $zero, 0		#160

j 92


SYSCALL



