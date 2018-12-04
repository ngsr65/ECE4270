#ifndef IF_H
#define IF_H

void IF(){
  ID_IF.IR = mem_read_32( CURRENT_STATE.PC );
  NEXT_STATE.PC += 4;
}

#endif
