#ifndef GST_MATCH_H
#define GST_MATCH_H

#define MATCH_BYTECODES(name_, bp_, code_) BEGIN_MACRO { \
  int arg, arg_lsb, n, num_args, ofs, opcode, opcode_, scopes, super; \
  unsigned char *IP = bp_; \
  unsigned char ATTRIBUTE_UNUSED *IP0 = bp_; \
  arg = arg_lsb = n = num_args = ofs = opcode = opcode_ = scopes = super = 0; \
MATCH_BYTECODES_START_##name_: \
  opcode_ = *IP; \
MATCH_BYTECODES_SWITCH_##name_: \
  switch (opcode_) { \
    case 0: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 256; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 256: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 1: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 257; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 257: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 2: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 258; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 258: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 3: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 259; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 259: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 4: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 260; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 260: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 5: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 261; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 261: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 6: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 262; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 262: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 7: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 263; \
	n = 7; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 263: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 8: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 264; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 264: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 9: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 265; \
	n = 9; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 265: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 10: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 266; \
	n = 10; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 266: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 11: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 267; \
	n = 11; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 267: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 12: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 268; \
	n = 12; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 268: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 13: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 269; \
	n = 13; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 269: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 14: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 270; \
	n = 14; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 270: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 15: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 271; \
	n = 15; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 271: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 16: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 272; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 272: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 17: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 273; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 273: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 18: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 274; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 274: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 19: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 275; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 275: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 20: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 276; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 276: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 21: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 277; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 277: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 22: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 278; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 278: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 23: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 279; \
	n = 7; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 279: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 24: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 280; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 280: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 25: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 281; \
	n = 9; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 281: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 26: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 282; \
	n = 10; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 282: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 27: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 283; \
	opcode = 27; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 283: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 28: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 284; \
	n = arg >> 8; \
	super = 0; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 284: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 29: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 285; \
	n = arg >> 8; \
	super = 1; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 285: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 30: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 286; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 286: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 31: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 287; \
	n = arg | arg_lsb; \
	super = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 287: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 32: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 288; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 288: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 33: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 289; \
	n = arg >> 8; \
	scopes = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 289: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 34: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 290; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 290: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 35: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 291; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 291: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 36: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 292; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 292: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 37: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 293; \
	n = arg >> 8; \
	scopes = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_OUTER_TEMP; \
    case 293: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 38: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 294; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_LIT_VARIABLE; \
    case 294: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 39: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 295; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_RECEIVER_VARIABLE; \
    case 295: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 40: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 296; \
	ofs = IP - IP0 - (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_JUMP; \
    case 296: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 41: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 297; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_JUMP; \
    case 297: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 42: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 298; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_TRUE; \
    case 298: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 43: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 299; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 299: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 44: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 300; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 300: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 45: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 301; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 301: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 46: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 302; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 302: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 47: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 303; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_POP_INTO_NEW_STACKTOP; \
    case 303: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 48: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 304; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 304: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 49: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 305; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 305: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 50: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 306; \
	goto MATCH_BYTECODES_##name_##_RETURN_METHOD_STACK_TOP; \
    case 306: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 51: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 307; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 307: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 52: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 308; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 308: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 53: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 309; \
	goto MATCH_BYTECODES_##name_##_EXIT_INTERPRETER; \
    case 309: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 54: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 310; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 310: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 55: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      arg=(arg | arg_lsb)<< 8; \
      goto MATCH_BYTECODES_START_##name_; \
      goto MATCH_BYTECODES_END_##name_; \
    case 56: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 311; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 311: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 57: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 312; \
	opcode = 57; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 312: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 58: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 313; \
	opcode = 58; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 313: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 59: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 314; \
	opcode = 59; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 314: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 60: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 315; \
	opcode = 60; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 315: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 61: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 316; \
	opcode = 61; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 316: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 62: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 317; \
	opcode = 62; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 317: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 63: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 318; \
	opcode = 63; \
	arg = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_INVALID; \
    case 318: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 64: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 319; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 319: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 65: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 320; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 320: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 66: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 321; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 321: \
	; \
      } while (0); \
      do { \
	opcode_ = 322; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 322: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 67: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 323; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 323: \
	; \
      } while (0); \
      do { \
	opcode_ = 324; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 324: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 68: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 325; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 325: \
	; \
      } while (0); \
      do { \
	opcode_ = 326; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 326: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 69: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 327; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 327: \
	; \
      } while (0); \
      do { \
	opcode_ = 328; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 328: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 70: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 329; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 329: \
	; \
      } while (0); \
      do { \
	opcode_ = 330; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 330: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 71: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 331; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 2; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 331: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 72: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 332; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 332: \
	; \
      } while (0); \
      do { \
	opcode_ = 333; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 333: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 73: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 334; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 334: \
	; \
      } while (0); \
      do { \
	opcode_ = 335; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 335: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 74: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 336; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 336: \
	; \
      } while (0); \
      do { \
	opcode_ = 337; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 337: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 75: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 338; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 338: \
	; \
      } while (0); \
      do { \
	opcode_ = 339; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 339: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 76: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 340; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 340: \
	; \
      } while (0); \
      do { \
	opcode_ = 341; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 341: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 77: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 342; \
	n = arg | arg_lsb; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 342: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 78: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 343; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 343: \
	; \
      } while (0); \
      do { \
	opcode_ = 344; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 344: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 79: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 345; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 345: \
	; \
      } while (0); \
      do { \
	opcode_ = 346; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 346: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 80: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 347; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_RECEIVER_VARIABLE; \
    case 347: \
	; \
      } while (0); \
      do { \
	opcode_ = 348; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 348: \
	; \
      } while (0); \
      do { \
	opcode_ = 349; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 349: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 81: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 350; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 350: \
	; \
      } while (0); \
      do { \
	opcode_ = 351; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 351: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 82: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 352; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 352: \
	; \
      } while (0); \
      do { \
	opcode_ = 353; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 353: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 83: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 354; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 354: \
	; \
      } while (0); \
      do { \
	opcode_ = 355; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 355: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 84: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 356; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 356: \
	; \
      } while (0); \
      do { \
	opcode_ = 357; \
	n = arg >> 8; \
	super = 0; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 357: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 85: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 358; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 358: \
	; \
      } while (0); \
      do { \
	opcode_ = 359; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 359: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 86: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 360; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 360: \
	; \
      } while (0); \
      do { \
	opcode_ = 361; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 361: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 87: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 362; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 362: \
	; \
      } while (0); \
      do { \
	opcode_ = 363; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 363: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 88: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 364; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 3; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 364: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 89: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 365; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 365: \
	; \
      } while (0); \
      do { \
	opcode_ = 366; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 366: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 90: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 367; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 367: \
	; \
      } while (0); \
      do { \
	opcode_ = 368; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 368: \
	; \
      } while (0); \
      do { \
	opcode_ = 369; \
	n = 0; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 369: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 91: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 370; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 370: \
	; \
      } while (0); \
      do { \
	opcode_ = 371; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 371: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 92: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 372; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 372: \
	; \
      } while (0); \
      do { \
	opcode_ = 373; \
	n = 36; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 373: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 93: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 374; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 374: \
	; \
      } while (0); \
      do { \
	opcode_ = 375; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 375: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 94: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 376; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 376: \
	; \
      } while (0); \
      do { \
	opcode_ = 377; \
	n = 1; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 377: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 95: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 378; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 378: \
	; \
      } while (0); \
      do { \
	opcode_ = 379; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 379: \
	; \
      } while (0); \
      do { \
	opcode_ = 380; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 380: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 96: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 381; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 381: \
	; \
      } while (0); \
      do { \
	opcode_ = 382; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 382: \
	; \
      } while (0); \
      do { \
	opcode_ = 383; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 383: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 97: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 384; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 384: \
	; \
      } while (0); \
      do { \
	opcode_ = 385; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 385: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 98: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 386; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 386: \
	; \
      } while (0); \
      do { \
	opcode_ = 387; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 387: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 99: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 388; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 388: \
	; \
      } while (0); \
      do { \
	opcode_ = 389; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 389: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 100: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 390; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 390: \
	; \
      } while (0); \
      do { \
	opcode_ = 391; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 391: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 101: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 392; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 392: \
	; \
      } while (0); \
      do { \
	opcode_ = 393; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 393: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 102: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 394; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 394: \
	; \
      } while (0); \
      do { \
	opcode_ = 395; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 395: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 103: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 396; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 396: \
	; \
      } while (0); \
      do { \
	opcode_ = 397; \
	n = 32; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 397: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 104: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 398; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 398: \
	; \
      } while (0); \
      do { \
	opcode_ = 399; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 399: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 105: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 400; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 400: \
	; \
      } while (0); \
      do { \
	opcode_ = 401; \
	n = arg >> 8; \
	super = 0; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 401: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 106: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 402; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 402: \
	; \
      } while (0); \
      do { \
	opcode_ = 403; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 403: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 107: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 404; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 404: \
	; \
      } while (0); \
      do { \
	opcode_ = 405; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 405: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 108: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 406; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 406: \
	; \
      } while (0); \
      do { \
	opcode_ = 407; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 407: \
	; \
      } while (0); \
      do { \
	opcode_ = 408; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 408: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 109: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 409; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 409: \
	; \
      } while (0); \
      do { \
	opcode_ = 410; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 410: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 110: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 411; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 411: \
	; \
      } while (0); \
      do { \
	opcode_ = 412; \
	n = 38; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 412: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 111: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 413; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 413: \
	; \
      } while (0); \
      do { \
	opcode_ = 414; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 414: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 112: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 415; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 415: \
	; \
      } while (0); \
      do { \
	opcode_ = 416; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 416: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 113: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 417; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 417: \
	; \
      } while (0); \
      do { \
	opcode_ = 418; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 418: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 114: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 419; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 419: \
	; \
      } while (0); \
      do { \
	opcode_ = 420; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_TRUE; \
    case 420: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 115: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 421; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 421: \
	; \
      } while (0); \
      do { \
	opcode_ = 422; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 422: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 116: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 423; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 423: \
	; \
      } while (0); \
      do { \
	opcode_ = 424; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 424: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 117: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 425; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 425: \
	; \
      } while (0); \
      do { \
	opcode_ = 426; \
	n = 49; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 426: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 118: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 427; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 427: \
	; \
      } while (0); \
      do { \
	opcode_ = 428; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 428: \
	; \
      } while (0); \
      do { \
	opcode_ = 429; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 429: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 119: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 430; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 430: \
	; \
      } while (0); \
      do { \
	opcode_ = 431; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 431: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 120: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 432; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_RECEIVER_VARIABLE; \
    case 432: \
	; \
      } while (0); \
      do { \
	opcode_ = 433; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 433: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 121: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 434; \
	n = arg | arg_lsb; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_STORE_OUTER_TEMP; \
    case 434: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 122: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 435; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 435: \
	; \
      } while (0); \
      do { \
	opcode_ = 436; \
	n = 96; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 436: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 123: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 437; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 437: \
	; \
      } while (0); \
      do { \
	opcode_ = 438; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 438: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 124: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 439; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 439: \
	; \
      } while (0); \
      do { \
	opcode_ = 440; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 440: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 125: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 441; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 441: \
	; \
      } while (0); \
      do { \
	opcode_ = 442; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 442: \
	; \
      } while (0); \
      do { \
	opcode_ = 443; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 443: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 126: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 444; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 444: \
	; \
      } while (0); \
      do { \
	opcode_ = 445; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 445: \
	; \
      } while (0); \
      do { \
	opcode_ = 446; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 446: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 127: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 447; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 447: \
	; \
      } while (0); \
      do { \
	opcode_ = 448; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 448: \
	; \
      } while (0); \
      do { \
	opcode_ = 449; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 449: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 128: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 450; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 450: \
	; \
      } while (0); \
      do { \
	opcode_ = 451; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 451: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 129: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 452; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 452: \
	; \
      } while (0); \
      do { \
	opcode_ = 453; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 453: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 130: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 454; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 454: \
	; \
      } while (0); \
      do { \
	opcode_ = 455; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_TRUE; \
    case 455: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 131: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 456; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 456: \
	; \
      } while (0); \
      do { \
	opcode_ = 457; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 457: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 132: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 458; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 458: \
	; \
      } while (0); \
      do { \
	opcode_ = 459; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 459: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 133: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 460; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 460: \
	; \
      } while (0); \
      do { \
	opcode_ = 461; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 461: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 134: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 462; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 462: \
	; \
      } while (0); \
      do { \
	opcode_ = 463; \
	ofs = IP - IP0 - (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_JUMP; \
    case 463: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 135: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 464; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 464: \
	; \
      } while (0); \
      do { \
	opcode_ = 465; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 465: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 136: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 466; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 466: \
	; \
      } while (0); \
      do { \
	opcode_ = 467; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 467: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 137: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 468; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 468: \
	; \
      } while (0); \
      do { \
	opcode_ = 469; \
	n = 2; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 469: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 138: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 470; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 470: \
	; \
      } while (0); \
      do { \
	opcode_ = 471; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 471: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 139: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 472; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 472: \
	; \
      } while (0); \
      do { \
	opcode_ = 473; \
	n = 0; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 473: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 140: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 474; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 474: \
	; \
      } while (0); \
      do { \
	opcode_ = 475; \
	n = 0; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 475: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 141: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 476; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 476: \
	; \
      } while (0); \
      do { \
	opcode_ = 477; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 477: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 142: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 478; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 478: \
	; \
      } while (0); \
      do { \
	opcode_ = 479; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 479: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 143: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 480; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 480: \
	; \
      } while (0); \
      do { \
	opcode_ = 481; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 481: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 144: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 482; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 482: \
	; \
      } while (0); \
      do { \
	opcode_ = 483; \
	n = 3; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 483: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 145: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 484; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 484: \
	; \
      } while (0); \
      do { \
	opcode_ = 485; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 485: \
	; \
      } while (0); \
      do { \
	opcode_ = 486; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 486: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 146: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 487; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 487: \
	; \
      } while (0); \
      do { \
	opcode_ = 488; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 488: \
	; \
      } while (0); \
      do { \
	opcode_ = 489; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 489: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 147: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 490; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 490: \
	; \
      } while (0); \
      do { \
	opcode_ = 491; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_TRUE; \
    case 491: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 148: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 492; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 492: \
	; \
      } while (0); \
      do { \
	opcode_ = 493; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 493: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 149: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 494; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 494: \
	; \
      } while (0); \
      do { \
	opcode_ = 495; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 495: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 150: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 496; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 496: \
	; \
      } while (0); \
      do { \
	opcode_ = 497; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 497: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 151: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 498; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 498: \
	; \
      } while (0); \
      do { \
	opcode_ = 499; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 499: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 152: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 500; \
	n = arg | arg_lsb; \
	scopes = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 500: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 153: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 501; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 501: \
	; \
      } while (0); \
      do { \
	opcode_ = 502; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 502: \
	; \
      } while (0); \
      do { \
	opcode_ = 503; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 503: \
	; \
      } while (0); \
      do { \
	opcode_ = 504; \
	n = 0; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 504: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 154: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 505; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 505: \
	; \
      } while (0); \
      do { \
	opcode_ = 506; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 506: \
	; \
      } while (0); \
      do { \
	opcode_ = 507; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 507: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 155: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 508; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 508: \
	; \
      } while (0); \
      do { \
	opcode_ = 509; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 509: \
	; \
      } while (0); \
      do { \
	opcode_ = 510; \
	n = 40; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 510: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 156: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 511; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 511: \
	; \
      } while (0); \
      do { \
	opcode_ = 512; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 512: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 157: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 513; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 513: \
	; \
      } while (0); \
      do { \
	opcode_ = 514; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 514: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 158: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 515; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 515: \
	; \
      } while (0); \
      do { \
	opcode_ = 516; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 516: \
	; \
      } while (0); \
      do { \
	opcode_ = 517; \
	n = 0; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 517: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 159: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 518; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 518: \
	; \
      } while (0); \
      do { \
	opcode_ = 519; \
	n = 14; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 519: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 160: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 520; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 520: \
	; \
      } while (0); \
      do { \
	opcode_ = 521; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 521: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 161: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 522; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 522: \
	; \
      } while (0); \
      do { \
	opcode_ = 523; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 523: \
	; \
      } while (0); \
      do { \
	opcode_ = 524; \
	n = arg >> 8; \
	super = 0; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 524: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 162: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 525; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 525: \
	; \
      } while (0); \
      do { \
	opcode_ = 526; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 526: \
	; \
      } while (0); \
      do { \
	opcode_ = 527; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 527: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 163: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 528; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 528: \
	; \
      } while (0); \
      do { \
	opcode_ = 529; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 529: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 164: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 530; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 530: \
	; \
      } while (0); \
      do { \
	opcode_ = 531; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 531: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 165: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 532; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 532: \
	; \
      } while (0); \
      do { \
	opcode_ = 533; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 533: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 166: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 534; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 534: \
	; \
      } while (0); \
      do { \
	opcode_ = 535; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 535: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 167: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 536; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 536: \
	; \
      } while (0); \
      do { \
	opcode_ = 537; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 537: \
	; \
      } while (0); \
      do { \
	opcode_ = 538; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 538: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 168: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 539; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 539: \
	; \
      } while (0); \
      do { \
	opcode_ = 540; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 540: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 169: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 541; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 541: \
	; \
      } while (0); \
      do { \
	opcode_ = 542; \
	n = 6; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 542: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 170: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 543; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 543: \
	; \
      } while (0); \
      do { \
	opcode_ = 544; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 544: \
	; \
      } while (0); \
      do { \
	opcode_ = 545; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 545: \
	; \
      } while (0); \
      do { \
	opcode_ = 546; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 546: \
	; \
      } while (0); \
      do { \
	opcode_ = 547; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 547: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 171: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 548; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 548: \
	; \
      } while (0); \
      do { \
	opcode_ = 549; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 549: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 172: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 550; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 550: \
	; \
      } while (0); \
      do { \
	opcode_ = 551; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 551: \
	; \
      } while (0); \
      do { \
	opcode_ = 552; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 552: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 173: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 553; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 553: \
	; \
      } while (0); \
      do { \
	opcode_ = 554; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 554: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 174: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 555; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 555: \
	; \
      } while (0); \
      do { \
	opcode_ = 556; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 556: \
	; \
      } while (0); \
      do { \
	opcode_ = 557; \
	n = 49; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 557: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 175: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 558; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 558: \
	; \
      } while (0); \
      do { \
	opcode_ = 559; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 559: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 176: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 560; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 560: \
	; \
      } while (0); \
      do { \
	opcode_ = 561; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 561: \
	; \
      } while (0); \
      do { \
	opcode_ = 562; \
	n = 84; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 562: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 177: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 563; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 563: \
	; \
      } while (0); \
      do { \
	opcode_ = 564; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 564: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 178: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 565; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 565: \
	; \
      } while (0); \
      do { \
	opcode_ = 566; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 566: \
	; \
      } while (0); \
      do { \
	opcode_ = 567; \
	n = 3; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 567: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 179: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 568; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 568: \
	; \
      } while (0); \
      do { \
	opcode_ = 569; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 569: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 180: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 570; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 570: \
	; \
      } while (0); \
      do { \
	opcode_ = 571; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 571: \
	; \
      } while (0); \
      do { \
	opcode_ = 572; \
	n = 36; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 572: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 181: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 573; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 573: \
	; \
      } while (0); \
      do { \
	opcode_ = 574; \
	n = 1; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 574: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 182: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 575; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 575: \
	; \
      } while (0); \
      do { \
	opcode_ = 576; \
	n = 13; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 576: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 183: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 577; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 577: \
	; \
      } while (0); \
      do { \
	opcode_ = 578; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 578: \
	; \
      } while (0); \
      do { \
	opcode_ = 579; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 579: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 184: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 580; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 580: \
	; \
      } while (0); \
      do { \
	opcode_ = 581; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 581: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 185: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 582; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 582: \
	; \
      } while (0); \
      do { \
	opcode_ = 583; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 583: \
	; \
      } while (0); \
      do { \
	opcode_ = 584; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 584: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 186: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 585; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 585: \
	; \
      } while (0); \
      do { \
	opcode_ = 586; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 586: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 187: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 587; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 587: \
	; \
      } while (0); \
      do { \
	opcode_ = 588; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 588: \
	; \
      } while (0); \
      do { \
	opcode_ = 589; \
	n = 2; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 589: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 188: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 590; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 590: \
	; \
      } while (0); \
      do { \
	opcode_ = 591; \
	n = 7; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 591: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 189: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 592; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 592: \
	; \
      } while (0); \
      do { \
	opcode_ = 593; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 593: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 190: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 594; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 594: \
	; \
      } while (0); \
      do { \
	opcode_ = 595; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 2; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 595: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 191: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 596; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 596: \
	; \
      } while (0); \
      do { \
	opcode_ = 597; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 597: \
	; \
      } while (0); \
      do { \
	opcode_ = 598; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 598: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 192: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 599; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 599: \
	; \
      } while (0); \
      do { \
	opcode_ = 600; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 600: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 193: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 601; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 601: \
	; \
      } while (0); \
      do { \
	opcode_ = 602; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 602: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 194: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 603; \
	n = 0; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 603: \
	; \
      } while (0); \
      do { \
	opcode_ = 604; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 604: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 195: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 605; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 605: \
	; \
      } while (0); \
      do { \
	opcode_ = 606; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 606: \
	; \
      } while (0); \
      do { \
	opcode_ = 607; \
	n = 1; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 607: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 196: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 608; \
	n = 0; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 608: \
	; \
      } while (0); \
      do { \
	opcode_ = 609; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 609: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 197: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 610; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 610: \
	; \
      } while (0); \
      do { \
	opcode_ = 611; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 611: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 198: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 612; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 612: \
	; \
      } while (0); \
      do { \
	opcode_ = 613; \
	n = 41; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 613: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 199: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 614; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 4; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 614: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 200: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 615; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 615: \
	; \
      } while (0); \
      do { \
	opcode_ = 616; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 616: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 201: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 617; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 617: \
	; \
      } while (0); \
      do { \
	opcode_ = 618; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 618: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 202: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 619; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 619: \
	; \
      } while (0); \
      do { \
	opcode_ = 620; \
	n = arg >> 8; \
	super = 0; \
	num_args = arg_lsb; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 620: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 203: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 621; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 621: \
	; \
      } while (0); \
      do { \
	opcode_ = 622; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 622: \
	; \
      } while (0); \
      do { \
	opcode_ = 623; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 623: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 204: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 624; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 624: \
	; \
      } while (0); \
      do { \
	opcode_ = 625; \
	n = 4; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 625: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 205: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 626; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 626: \
	; \
      } while (0); \
      do { \
	opcode_ = 627; \
	n = 0; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 627: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 206: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 628; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 628: \
	; \
      } while (0); \
      do { \
	opcode_ = 629; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 629: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 207: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 630; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 630: \
	; \
      } while (0); \
      do { \
	opcode_ = 631; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 2; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 631: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 208: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 632; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 632: \
	; \
      } while (0); \
      do { \
	opcode_ = 633; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 633: \
	; \
      } while (0); \
      do { \
	opcode_ = 634; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 2; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 634: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 209: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 635; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 635: \
	; \
      } while (0); \
      do { \
	opcode_ = 636; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_POP_INTO_NEW_STACKTOP; \
    case 636: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 210: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 637; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 637: \
	; \
      } while (0); \
      do { \
	opcode_ = 638; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 638: \
	; \
      } while (0); \
      do { \
	opcode_ = 639; \
	n = 5; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 639: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 211: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 640; \
	n = arg | arg_lsb; \
	super = 1; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 640: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 212: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 641; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 641: \
	; \
      } while (0); \
      do { \
	opcode_ = 642; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 642: \
	; \
      } while (0); \
      do { \
	opcode_ = 643; \
	n = 4; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 643: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 213: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 644; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 644: \
	; \
      } while (0); \
      do { \
	opcode_ = 645; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 645: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 214: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 646; \
	n = arg | arg_lsb; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_STORE_OUTER_TEMP; \
    case 646: \
	; \
      } while (0); \
      do { \
	opcode_ = 647; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 647: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 215: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 648; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 648: \
	; \
      } while (0); \
      do { \
	opcode_ = 649; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 649: \
	; \
      } while (0); \
      do { \
	opcode_ = 650; \
	n = 37; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 650: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 216: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 651; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 651: \
	; \
      } while (0); \
      do { \
	opcode_ = 652; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 652: \
	; \
      } while (0); \
      do { \
	opcode_ = 653; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 653: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 217: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 654; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 654: \
	; \
      } while (0); \
      do { \
	opcode_ = 655; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 655: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 218: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 656; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 656: \
	; \
      } while (0); \
      do { \
	opcode_ = 657; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 657: \
	; \
      } while (0); \
      do { \
	opcode_ = 658; \
	n = 130; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 658: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 219: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 659; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 659: \
	; \
      } while (0); \
      do { \
	opcode_ = 660; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 660: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 220: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 661; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 661: \
	; \
      } while (0); \
      do { \
	opcode_ = 662; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 662: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 221: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 663; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 663: \
	; \
      } while (0); \
      do { \
	opcode_ = 664; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 664: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 222: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 665; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 665: \
	; \
      } while (0); \
      do { \
	opcode_ = 666; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 666: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 223: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 667; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 667: \
	; \
      } while (0); \
      do { \
	opcode_ = 668; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 668: \
	; \
      } while (0); \
      do { \
	opcode_ = 669; \
	n = 2; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 669: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 224: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 670; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 670: \
	; \
      } while (0); \
      do { \
	opcode_ = 671; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 671: \
	; \
      } while (0); \
      do { \
	opcode_ = 672; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 672: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 225: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 673; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 673: \
	; \
      } while (0); \
      do { \
	opcode_ = 674; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 674: \
	; \
      } while (0); \
      do { \
	opcode_ = 675; \
	n = 6; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 675: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 226: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 676; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 676: \
	; \
      } while (0); \
      do { \
	opcode_ = 677; \
	goto MATCH_BYTECODES_##name_##_RETURN_METHOD_STACK_TOP; \
    case 677: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 227: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 678; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 678: \
	; \
      } while (0); \
      do { \
	opcode_ = 679; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 679: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 228: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 680; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 680: \
	; \
      } while (0); \
      do { \
	opcode_ = 681; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 681: \
	; \
      } while (0); \
      do { \
	opcode_ = 682; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_RECEIVER_VARIABLE; \
    case 682: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 229: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 683; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 683: \
	; \
      } while (0); \
      do { \
	opcode_ = 684; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 684: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 230: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 685; \
	n = 2; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 685: \
	; \
      } while (0); \
      do { \
	opcode_ = 686; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 686: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 231: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 687; \
	n = arg | arg_lsb; \
	super = 1; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 687: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 232: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 688; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_SPECIAL; \
    case 688: \
	; \
      } while (0); \
      do { \
	opcode_ = 689; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_RECEIVER_VARIABLE; \
    case 689: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 233: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 690; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 690: \
	; \
      } while (0); \
      do { \
	opcode_ = 691; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 691: \
	; \
      } while (0); \
      do { \
	opcode_ = 692; \
	n = 0; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 692: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 234: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 693; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 693: \
	; \
      } while (0); \
      do { \
	opcode_ = 694; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 694: \
	; \
      } while (0); \
      do { \
	opcode_ = 695; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 695: \
	; \
      } while (0); \
      do { \
	opcode_ = 696; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 696: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 235: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 697; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 697: \
	; \
      } while (0); \
      do { \
	opcode_ = 698; \
	n = 8; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 698: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 236: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 699; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 699: \
	; \
      } while (0); \
      do { \
	opcode_ = 700; \
	goto MATCH_BYTECODES_##name_##_MAKE_DIRTY_BLOCK; \
    case 700: \
	; \
      } while (0); \
      do { \
	opcode_ = 701; \
	n = 1; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 701: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 237: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 702; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 702: \
	; \
      } while (0); \
      do { \
	opcode_ = 703; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 703: \
	; \
      } while (0); \
      do { \
	opcode_ = 704; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 704: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 238: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 705; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 705: \
	; \
      } while (0); \
      do { \
	opcode_ = 706; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 706: \
	; \
      } while (0); \
      do { \
	opcode_ = 707; \
	n = arg | arg_lsb; \
	super = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_IMMEDIATE; \
    case 707: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 239: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 708; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 708: \
	; \
      } while (0); \
      do { \
	opcode_ = 709; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 709: \
	; \
      } while (0); \
      do { \
	opcode_ = 710; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 710: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 240: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 711; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 711: \
	; \
      } while (0); \
      do { \
	opcode_ = 712; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 712: \
	; \
      } while (0); \
      do { \
	opcode_ = 713; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 713: \
	; \
      } while (0); \
      do { \
	opcode_ = 714; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_SEND_ARITH; \
    case 714: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 241: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 715; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 715: \
	; \
      } while (0); \
      do { \
	opcode_ = 716; \
	n = 4; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 716: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 242: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 717; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 717: \
	; \
      } while (0); \
      do { \
	opcode_ = 718; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 718: \
	; \
      } while (0); \
      do { \
	opcode_ = 719; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_VARIABLE; \
    case 719: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 243: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 720; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 720: \
	; \
      } while (0); \
      do { \
	opcode_ = 721; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 721: \
	; \
      } while (0); \
      do { \
	opcode_ = 722; \
	n = 1; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 722: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 244: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 723; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_POP_INTO_NEW_STACKTOP; \
    case 723: \
	; \
      } while (0); \
      do { \
	opcode_ = 724; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 1; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 724: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 245: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 725; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 725: \
	; \
      } while (0); \
      do { \
	opcode_ = 726; \
	goto MATCH_BYTECODES_##name_##_POP_STACK_TOP; \
    case 726: \
	; \
      } while (0); \
      do { \
	opcode_ = 727; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 727: \
	; \
      } while (0); \
      do { \
	opcode_ = 728; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 728: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 246: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 729; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_STORE_TEMPORARY_VARIABLE; \
    case 729: \
	; \
      } while (0); \
      do { \
	opcode_ = 730; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 730: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 247: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 731; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 731: \
	; \
      } while (0); \
      do { \
	opcode_ = 732; \
	n = arg | arg_lsb; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 732: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 248: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 733; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_STORE_RECEIVER_VARIABLE; \
    case 733: \
	; \
      } while (0); \
      do { \
	opcode_ = 734; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 734: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 249: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 735; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 735: \
	; \
      } while (0); \
      do { \
	opcode_ = 736; \
	goto MATCH_BYTECODES_##name_##_PUSH_SELF; \
    case 736: \
	; \
      } while (0); \
      do { \
	opcode_ = 737; \
	n = 3; \
	super = 0; \
	num_args = 0; \
	goto MATCH_BYTECODES_##name_##_SEND; \
    case 737: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 250: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 738; \
	n = 5; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 738: \
	; \
      } while (0); \
      do { \
	opcode_ = 739; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 739: \
	; \
      } while (0); \
      do { \
	opcode_ = 740; \
	ofs = IP - IP0 + (arg | arg_lsb); \
	goto MATCH_BYTECODES_##name_##_POP_JUMP_FALSE; \
    case 740: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 251: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 741; \
	goto MATCH_BYTECODES_##name_##_DUP_STACK_TOP; \
    case 741: \
	; \
      } while (0); \
      do { \
	opcode_ = 742; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_LIT_CONSTANT; \
    case 742: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 252: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 743; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 743: \
	; \
      } while (0); \
      do { \
	opcode_ = 744; \
	n = 0; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 744: \
	; \
      } while (0); \
      do { \
	opcode_ = 745; \
	goto MATCH_BYTECODES_##name_##_RETURN_CONTEXT_STACK_TOP; \
    case 745: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 253: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 746; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_INTEGER; \
    case 746: \
	; \
      } while (0); \
      do { \
	opcode_ = 747; \
	n = 1; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 747: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 254: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 748; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_PUSH_TEMPORARY_VARIABLE; \
    case 748: \
	; \
      } while (0); \
      do { \
	opcode_ = 749; \
	n = 3; \
	goto MATCH_BYTECODES_##name_##_SEND_SPECIAL; \
    case 749: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
    case 255: \
      opcode = IP[0]; \
      arg_lsb = IP[1]; \
      bp_ = (IP += 2); \
      do { \
	opcode_ = 750; \
	n = arg | arg_lsb; \
	goto MATCH_BYTECODES_##name_##_LINE_NUMBER_BYTECODE; \
    case 750: \
	; \
      } while (0); \
      do { \
	opcode_ = 751; \
	n = 2; \
	scopes = 1; \
	goto MATCH_BYTECODES_##name_##_PUSH_OUTER_TEMP; \
    case 751: \
	; \
      } while (0); \
      goto MATCH_BYTECODES_END_##name_; \
  } \
  MATCH_BYTECODES_DISPATCH(MATCH_BYTECODES_##name_) \
MATCH_BYTECODES_END_##name_: \
  ;} END_MACRO

#define MATCH_BYTECODES_DISPATCH(name) name

#define MATCH_BYTECODES_PRINT_BYTECODE_NAME \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_RECEIVER_VARIABLE: \
  { \
      printf ("%s\tpush Instance Variable[%d]\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_TEMPORARY_VARIABLE: \
  { \
      printf ("%s\tpush Temporary Variable[%d]\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_LIT_CONSTANT: \
  { \
      printf ("%s\tpush Literal[%d]", prefix, n); \
      if (literal_vec) \
	printf (" = %O", literal_vec[n]); \
      printf ("\n"); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_LIT_VARIABLE: \
  { \
      printf ("%s\tpush Global Variable[%d]", prefix, n); \
      if (literal_vec) \
	printf (" = %+O", literal_vec[n]); \
      printf ("\n"); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_SELF: \
  { \
      printf ("%s\tpush self\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_SPECIAL: \
  { \
      switch (n) { \
	case TRUE_INDEX: printf ("%s\tpush true\n", prefix); break; \
	case FALSE_INDEX: printf ("%s\tpush false\n", prefix); break; \
	case NIL_INDEX: printf ("%s\tpush nil\n", prefix); break; \
      } \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_INTEGER: \
  { \
      printf ("%s\tpush %d\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_RETURN_METHOD_STACK_TOP: \
  { \
      printf ("%s\treturn explicitly from method\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_RETURN_CONTEXT_STACK_TOP: \
  { \
      printf ("%s\treturn stack top\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_LINE_NUMBER_BYTECODE: \
  { \
      printf ("%s\tsource line %d\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_STORE_RECEIVER_VARIABLE: \
  { \
      printf ("%s\tstore into Instance Variable[%d]\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_STORE_TEMPORARY_VARIABLE: \
  { \
      printf ("%s\tstore into Temporary Variable[%d]\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_STORE_LIT_VARIABLE: \
  { \
      printf ("%s\tstore into Global Variable[%d]", prefix, n); \
      if (literal_vec) \
	printf (" = %+O", literal_vec[n]); \
      printf ("\n"); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_SEND: \
  { \
      printf ("%s\tsend selector %d%s, %d args", prefix, n, super ? " to super" : "", num_args); \
      if (literal_vec) \
	printf (" = %O", literal_vec[n]); \
      printf ("\n"); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_POP_INTO_NEW_STACKTOP: \
  { \
      printf ("%s\tpop and store into Instance Variable[%d] of new stack top\n", prefix, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_POP_STACK_TOP: \
  { \
      printf ("%s\tpop stack top\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_DUP_STACK_TOP: \
  { \
      printf ("%s\tduplicate stack top\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_PUSH_OUTER_TEMP: \
  { \
      printf ("%s\tpush outer var scopes = %d varIndex = %d\n", prefix, scopes, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_STORE_OUTER_TEMP: \
  { \
      printf ("%s\tstore outer var scopes = %d varIndex = %d\n", prefix, scopes, n); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_EXIT_INTERPRETER: \
  { \
      printf ("%s\tterminate interpreter\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_JUMP: \
  { \
      printf ("%s\tjump to %d\n", prefix, ip + ofs); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_POP_JUMP_TRUE: \
  { \
      printf ("%s\tpop and jump to %d if true\n", prefix, ip + ofs); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_POP_JUMP_FALSE: \
  { \
      printf ("%s\tpop and jump to %d if false\n", prefix, ip + ofs); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_SEND_ARITH: \
  { \
      printf ("%s\tsend arithmetic message %O\n", prefix, \
	      _gst_builtin_selectors[n].symbol); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_SEND_SPECIAL: \
  { \
      printf ("%s\tsend special message %O\n", prefix, \
	      _gst_builtin_selectors[n + 16].symbol); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_MAKE_DIRTY_BLOCK: \
  { \
      printf ("%s\tmake dirty block\n", prefix); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_SEND_IMMEDIATE: \
  { \
      printf ("%s\tsend special message %O%s\n", prefix, \
	      _gst_builtin_selectors[n].symbol, \
	      super ? " to super" : ""); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME; \
  MATCH_BYTECODES_PRINT_BYTECODE_NAME_INVALID: \
  { \
      printf ("%s\tINVALID BYTECODE %d(%d)\n", prefix, b, arg); \
      prefix = pref; \
    } \
  goto MATCH_BYTECODES_SWITCH_PRINT_BYTECODE_NAME;

#define MATCH_BYTECODES_IS_SIMPLE_RETURN \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_SELF: \
  { maybe = MTH_RETURN_SELF; } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_RECEIVER_VARIABLE: \
  { maybe = (n << 8) | MTH_RETURN_INSTVAR; } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_LIT_CONSTANT: \
  { maybe = (n << 8) | MTH_RETURN_LITERAL; } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_INTEGER: \
  { maybe_object = FROM_INT (n); maybe = MTH_RETURN_LITERAL; } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_SPECIAL: \
  { \
          maybe = MTH_RETURN_LITERAL; \
          switch (n) \
	    { \
	      case NIL_INDEX: maybe_object = _gst_nil_oop; break; \
              case TRUE_INDEX: maybe_object = _gst_true_oop; break; \
              case FALSE_INDEX: maybe_object = _gst_false_oop; break; \
	      default: abort (); \
	    } \
        } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_LINE_NUMBER_BYTECODE: \
  { } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_RETURN_CONTEXT_STACK_TOP: \
  { \
	  if (maybe_object) \
	    _gst_add_forced_object (maybe_object); \
	  return maybe; \
	} \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_OUTER_TEMP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_JUMP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_POP_JUMP_TRUE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_POP_JUMP_FALSE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_SEND: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_POP_STACK_TOP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_DUP_STACK_TOP: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_SEND_IMMEDIATE: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_EXIT_INTERPRETER: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_SEND_ARITH: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_SEND_SPECIAL: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_MAKE_DIRTY_BLOCK: \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_RETURN_METHOD_STACK_TOP: \
  { return (MTH_NORMAL); } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN; \
  MATCH_BYTECODES_IS_SIMPLE_RETURN_INVALID: \
  { abort(); } \
  goto MATCH_BYTECODES_SWITCH_IS_SIMPLE_RETURN;

#define MATCH_BYTECODES_CHECK_KIND_OF_BLOCK \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_SELF: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_STORE_RECEIVER_VARIABLE: \
  { \
	  if (status == 0) \
	    status = 1; \
	} \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK; \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_LIT_CONSTANT: \
  { \
	  newStatus = check_inner_block (literals[n]); \
	  if (newStatus > status) \
	    { \
	      if (newStatus == 31) \
		return (31); \
	      status = newStatus; \
	    } \
        } \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK; \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_OUTER_TEMP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_STORE_OUTER_TEMP: \
  { \
	  if (status < 1 + scopes) status = 1 + scopes; \
	  if (status > 31) \
	    /* ouch! how deep!! */ \
	    return (31); \
	} \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK; \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_JUMP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_POP_JUMP_TRUE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_POP_JUMP_FALSE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_SPECIAL: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_PUSH_INTEGER: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_RETURN_CONTEXT_STACK_TOP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_SEND: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_POP_STACK_TOP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_DUP_STACK_TOP: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_EXIT_INTERPRETER: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_SEND_ARITH: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_SEND_SPECIAL: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_SEND_IMMEDIATE: \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_MAKE_DIRTY_BLOCK: \
  { } \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK; \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_RETURN_METHOD_STACK_TOP: \
  { return (31); } \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK; \
  MATCH_BYTECODES_CHECK_KIND_OF_BLOCK_INVALID: \
  { abort(); } \
  goto MATCH_BYTECODES_SWITCH_CHECK_KIND_OF_BLOCK;

#define MATCH_BYTECODES_THREAD_JUMPS \
  MATCH_BYTECODES_THREAD_JUMPS_MAKE_DIRTY_BLOCK: \
  MATCH_BYTECODES_THREAD_JUMPS_SEND_SPECIAL: \
  MATCH_BYTECODES_THREAD_JUMPS_SEND_ARITH: \
  MATCH_BYTECODES_THREAD_JUMPS_SEND_IMMEDIATE: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_LIT_CONSTANT: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_SELF: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_SPECIAL: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_INTEGER: \
  MATCH_BYTECODES_THREAD_JUMPS_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_THREAD_JUMPS_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_THREAD_JUMPS_SEND: \
  MATCH_BYTECODES_THREAD_JUMPS_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_THREAD_JUMPS_POP_STACK_TOP: \
  MATCH_BYTECODES_THREAD_JUMPS_DUP_STACK_TOP: \
  MATCH_BYTECODES_THREAD_JUMPS_PUSH_OUTER_TEMP: \
  MATCH_BYTECODES_THREAD_JUMPS_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_THREAD_JUMPS_EXIT_INTERPRETER: \
  { } \
  goto MATCH_BYTECODES_SWITCH_THREAD_JUMPS; \
  MATCH_BYTECODES_THREAD_JUMPS_JUMP: \
  { \
	      if (ofs == 2 \
		  && dest[0] == LINE_NUMBER_BYTECODE) \
		{ \
		  /* This could not be optimized to a nop, cause the \
		     jump and line number bytecodes lie in different \
		     basic blocks! So we rewrite it to a functionally \
		     equivalent but optimizable bytecode sequence.  */ \
		  dest[-2] = dest[0]; \
		  dest[-1] = dest[1]; \
		} \
	      else if (ofs == 4 \
		  && IS_PUSH_BYTECODE (dest[0]) \
		  && dest[2] == POP_STACK_TOP) \
		{ \
		  /* This could not be optimized to a single pop, \
		     cause the push and pop bytecodes lie in different \
		     basic blocks! Again, rewrite to an optimizable \
		     sequence.  */ \
		  dest[-2] = POP_STACK_TOP; \
		  dest[-1] = 0; \
		} \
	      else \
		{ \
	          /* Don't optimize jumps that have extension bytes.  But if we \
		     jump to a return, we can safely optimize: returns are \
		     never extended, and the interpreter ignores the extension \
		     byte.  TODO: check if this is still true.  */ \
	          canOptimizeJump = (*IP0 != EXT_BYTE); \
		  kind = IP[-2]; \
	          dest_ip0 = dest = IP0 + ofs; \
	          canOptimizeJump |= IS_RETURN_BYTECODE (*dest); \
		  split = true; \
		} \
	    } \
  goto MATCH_BYTECODES_SWITCH_THREAD_JUMPS; \
  MATCH_BYTECODES_THREAD_JUMPS_POP_JUMP_TRUE: \
  MATCH_BYTECODES_THREAD_JUMPS_POP_JUMP_FALSE: \
  { \
	      if (ofs == 0) \
		{ \
		  dest[-2] = POP_STACK_TOP; \
		  dest[-1] = 0; \
		} \
 \
	      /* Jumps to CONDITIONAL jumps must not be touched, either because \
		 they were unconditional or because they pop the stack top! */ \
	      else if (dest_ip0 == bp) \
		{ \
		  kind = IP[-2]; \
		  dest_ip0 = dest = IP0 + ofs; \
		  split = true; \
		} \
	    } \
  goto MATCH_BYTECODES_SWITCH_THREAD_JUMPS; \
  MATCH_BYTECODES_THREAD_JUMPS_RETURN_METHOD_STACK_TOP: \
  MATCH_BYTECODES_THREAD_JUMPS_RETURN_CONTEXT_STACK_TOP: \
  { \
	      /* Return bytecodes - patch the original jump to return \
	         directly */ \
	      bp[0] = dest[-2]; \
	      bp[1] = 0; \
 \
	      /* This in fact eliminated the jump, don't split in basic  \
	         blocks */ \
	      split = false; \
	    } \
  goto MATCH_BYTECODES_SWITCH_THREAD_JUMPS; \
  MATCH_BYTECODES_THREAD_JUMPS_INVALID: \
  { abort (); } \
  goto MATCH_BYTECODES_SWITCH_THREAD_JUMPS;

#define MATCH_BYTECODES_COMPUTE_STACK_POS \
  MATCH_BYTECODES_COMPUTE_STACK_POS_RETURN_METHOD_STACK_TOP: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_RETURN_CONTEXT_STACK_TOP: \
  { \
	      bc_len = bp - bp_first; \
 \
	      /* We cannot fill the basic block right now because the \
		 stack height might be different.  */ \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, 0, \
				      bp_first + bc_len, curr_sp + balance); \
		  bb_start[bc_len]->suspended = true; \
		  bb_start[bc_len]->next = NULL; \
		  *susp_tail = bb_start[bc_len]; \
		  susp_tail = &(bb_start[bc_len]->next); \
		} \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_POP_STACK_TOP: \
  { balance--; } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_LIT_CONSTANT: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_SELF: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_SPECIAL: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_INTEGER: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_DUP_STACK_TOP: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_PUSH_OUTER_TEMP: \
  { balance++; } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_EXIT_INTERPRETER: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_MAKE_DIRTY_BLOCK: \
  { } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_SEND: \
  { \
	      balance -= super + num_args; \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_SEND_ARITH: \
  { \
	      balance -= _gst_builtin_selectors[n].numArgs; \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_SEND_IMMEDIATE: \
  { \
	      balance -= super + _gst_builtin_selectors[n].numArgs; \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_SEND_SPECIAL: \
  { \
	      balance -= _gst_builtin_selectors[n + 16].numArgs; \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_INVALID: \
  { \
	      abort (); \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_JUMP: \
  { \
	      bc_len = bp - bp_first; \
 \
	      /* We cannot fill the basic block right now because the \
		 stack height might be different.  */ \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, 0, \
				      bp_first + bc_len, 0); \
		  bb_start[bc_len]->suspended = true; \
		  bb_start[bc_len]->next = NULL; \
		  *susp_tail = bb_start[bc_len]; \
		  susp_tail = &(bb_start[bc_len]->next); \
		} \
 \
	      if (!bb_start[ofs]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + ofs, 0, \
				      bp_first + ofs, curr_sp + balance); \
		  bb_start[ofs]->next = worklist; \
		  worklist = bb_start[ofs]; \
		} \
	      else if (bb_start[ofs]->suspended) \
		{ \
		  bb_start[ofs]->suspended = false; \
		  bb_start[ofs]->sp = curr_sp + balance; \
		} \
	      else if (curr_sp + balance != bb_start[ofs]->sp) \
		abort (); \
	    } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS; \
  MATCH_BYTECODES_COMPUTE_STACK_POS_POP_JUMP_TRUE: \
  MATCH_BYTECODES_COMPUTE_STACK_POS_POP_JUMP_FALSE: \
  { \
	      balance--; \
	      bc_len = bp - bp_first; \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, 0, \
				      bp_first + bc_len, curr_sp + balance); \
		  bb_start[bc_len]->next = worklist; \
		  worklist = bb_start[bc_len]; \
		} \
	      else if (bb_start[bc_len]->suspended) \
		{ \
		  bb_start[bc_len]->suspended = false; \
		  bb_start[bc_len]->sp = curr_sp + balance; \
		} \
	      else if (curr_sp + balance != bb_start[bc_len]->sp) \
		abort (); \
 \
	      if (!bb_start[ofs]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + ofs, 0, \
				      bp_first + ofs, curr_sp + balance); \
		  bb_start[ofs]->next = worklist; \
		  worklist = bb_start[ofs]; \
		} \
              else if (bb_start[ofs]->suspended) \
		{ \
		  bb_start[ofs]->suspended = false; \
		  bb_start[ofs]->sp = curr_sp + balance; \
		} \
	      else if (curr_sp + balance != bb_start[ofs]->sp) \
		abort (); \
            } \
  goto MATCH_BYTECODES_SWITCH_COMPUTE_STACK_POS;

#define MATCH_BYTECODES_MAKE_DEST_TABLE \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_LIT_CONSTANT: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_SELF: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_SPECIAL: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_INTEGER: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_RETURN_METHOD_STACK_TOP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_RETURN_CONTEXT_STACK_TOP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_SEND: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_POP_STACK_TOP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_DUP_STACK_TOP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_PUSH_OUTER_TEMP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_EXIT_INTERPRETER: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_SEND_ARITH: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_SEND_SPECIAL: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_SEND_IMMEDIATE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_MAKE_DIRTY_BLOCK: \
  { } \
  goto MATCH_BYTECODES_SWITCH_MAKE_DEST_TABLE; \
  MATCH_BYTECODES_MAKE_DEST_TABLE_INVALID: \
  { abort(); } \
  goto MATCH_BYTECODES_SWITCH_MAKE_DEST_TABLE; \
  MATCH_BYTECODES_MAKE_DEST_TABLE_JUMP: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_POP_JUMP_TRUE: \
  MATCH_BYTECODES_MAKE_DEST_TABLE_POP_JUMP_FALSE: \
  { \
          dest[ofs] = ofs > 0 ? 1 : -1; \
	  count++; \
        } \
  goto MATCH_BYTECODES_SWITCH_MAKE_DEST_TABLE;

#define MATCH_BYTECODES_CREATE_BASIC_BLOCKS \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_RETURN_METHOD_STACK_TOP: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_RETURN_CONTEXT_STACK_TOP: \
  { \
	      bc_len = bp - bp_first; \
 \
	      /* We cannot fill the basic block right now because the \
		 stack height might be different.  */ \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, stack_depth, \
				      bp_first + bc_len, curr_sp + balance); \
		  bb_start[bc_len]->suspended = true; \
		  bb_start[bc_len]->next = NULL; \
		  *susp_tail = bb_start[bc_len]; \
		  susp_tail = &(bb_start[bc_len]->next); \
		} \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_POP_STACK_TOP: \
  { balance--; } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_LIT_CONSTANT: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_SELF: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_SPECIAL: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_INTEGER: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_PUSH_OUTER_TEMP: \
  { balance++; } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_STORE_LIT_VARIABLE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_EXIT_INTERPRETER: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_MAKE_DIRTY_BLOCK: \
  { } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_SEND: \
  { \
	      balance -= super + num_args; \
 \
	      /* Sends touch the new stack top, so they require an extra slot.  */ \
	      if (curr_sp + balance < 1) \
	        return ("stack underflow"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_SEND_ARITH: \
  { \
	      if (!_gst_builtin_selectors[n].symbol) \
		return ("invalid immediate send"); \
 \
	      balance -= _gst_builtin_selectors[n].numArgs; \
 \
	      /* Sends touch the new stack top, so they require an extra slot.  */ \
	      if (curr_sp + balance < 1) \
	        return ("stack underflow"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_SEND_SPECIAL: \
  { \
	      if (!_gst_builtin_selectors[n + 16].symbol) \
		return ("invalid immediate send"); \
 \
	      balance -= _gst_builtin_selectors[n + 16].numArgs; \
 \
	      /* Sends touch the new stack top, so they require an extra slot.  */ \
	      if (curr_sp + balance < 1) \
	        return ("stack underflow"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_SEND_IMMEDIATE: \
  { \
	      if (!_gst_builtin_selectors[n].symbol) \
		return ("invalid immediate send"); \
 \
	      balance -= super + _gst_builtin_selectors[n].numArgs; \
 \
	      /* Sends touch the new stack top, so they require an extra slot.  */ \
	      if (curr_sp + balance < 1) \
	        return ("stack underflow"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_POP_INTO_NEW_STACKTOP: \
  { \
	      balance--; \
 \
	      /* Sends touch the new stack top, so they require an extra slot.  */ \
	      if (curr_sp + balance < 1) \
	        return ("stack underflow"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_DUP_STACK_TOP: \
  { \
	      balance++; \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_INVALID: \
  { \
	      return ("invalid bytecode"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_JUMP: \
  { \
	      if (ofs & 1) \
		return ("jump to odd offset"); \
 \
	      if (ofs + curr_ip < 0 || ofs + curr_ip > size) \
		return ("jump out of range"); \
 \
	      if (ofs + curr_ip > 0 && bp_first[ofs - 2] == EXT_BYTE) \
		return ("jump skips extension bytecode"); \
 \
	      bc_len = bp - bp_first; \
 \
	      /* We cannot fill the basic block right now because the \
		 stack height might be different.  */ \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, stack_depth, \
				      bp_first + bc_len, 0); \
		  bb_start[bc_len]->suspended = true; \
		  bb_start[bc_len]->next = NULL; \
		  *susp_tail = bb_start[bc_len]; \
		  susp_tail = &(bb_start[bc_len]->next); \
		} \
 \
	      if (!bb_start[ofs]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + ofs, stack_depth, \
				      bp_first + ofs, curr_sp + balance); \
 \
		  bb_start[ofs]->next = worklist; \
		  worklist = bb_start[ofs]; \
		  INIT_BASIC_BLOCK (worklist, num_temps); \
		} \
	      else if (bb_start[ofs]->suspended) \
	        { \
		  bb_start[ofs]->suspended = false; \
		  bb_start[ofs]->sp = curr_sp + balance; \
		  INIT_BASIC_BLOCK (bb_start[ofs], num_temps); \
		} \
	      else if (curr_sp + balance != bb_start[ofs]->sp) \
		return ("stack height mismatch"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS; \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_POP_JUMP_TRUE: \
  MATCH_BYTECODES_CREATE_BASIC_BLOCKS_POP_JUMP_FALSE: \
  { \
	      balance--; \
	      if (ofs & 1) \
		return ("jump to odd offset"); \
 \
	      if (ofs + curr_ip < 0 || ofs + curr_ip > size) \
		return ("jump out of range"); \
 \
	      if (ofs + curr_ip > 0 && bp_first[ofs - 2] == EXT_BYTE) \
		return ("jump skips extension bytecode"); \
 \
	      bc_len = bp - bp_first; \
	      if (!bb_start[bc_len]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + bc_len, stack_depth, \
				      bp_first + bc_len, curr_sp + balance); \
 \
		  bb_start[bc_len]->next = worklist; \
		  worklist = bb_start[bc_len]; \
		  INIT_BASIC_BLOCK (worklist, num_temps); \
		} \
              else if (bb_start[bc_len]->suspended) \
                { \
                  bb_start[bc_len]->suspended = false; \
                  bb_start[bc_len]->sp = curr_sp + balance; \
		  INIT_BASIC_BLOCK (bb_start[bc_len], num_temps); \
                } \
	      else if (curr_sp + balance != bb_start[bc_len]->sp) \
		return ("stack height mismatch"); \
 \
	      if (!bb_start[ofs]) \
		{ \
		  ALLOCA_BASIC_BLOCK (bb_start + ofs, stack_depth, \
				      bp_first + ofs, curr_sp + balance); \
 \
		  bb_start[ofs]->next = worklist; \
		  worklist = bb_start[ofs]; \
		  INIT_BASIC_BLOCK (worklist, num_temps); \
		} \
	      else if (bb_start[ofs]->suspended) \
	        { \
		  bb_start[ofs]->suspended = false; \
		  bb_start[ofs]->sp = curr_sp + balance; \
		  INIT_BASIC_BLOCK (bb_start[ofs], num_temps); \
		} \
	      else if (curr_sp + balance != bb_start[ofs]->sp) \
		return ("stack height mismatch"); \
            } \
  goto MATCH_BYTECODES_SWITCH_CREATE_BASIC_BLOCKS;

#define MATCH_BYTECODES_EXEC_BASIC_BLOCK \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_RECEIVER_VARIABLE: \
  { \
	      CHECK_REC_VAR (0, n); \
	      *sp++ = FROM_INT (VARYING); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_TEMPORARY_VARIABLE: \
  { \
	      CHECK_TEMP (n); \
	      *sp++ = stack[n]; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_LIT_CONSTANT: \
  { \
	      CHECK_LITERAL (n); \
	      *sp++ = LITERAL_CLASS (n); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_LIT_VARIABLE: \
  { \
	      CHECK_LIT_VARIABLE (false, n); \
	      *sp++ = LIT_VARIABLE_CLASS (n); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_SELF: \
  { \
	      last_used_literal = NULL; \
	      *sp++ = FROM_INT (SELF); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_SPECIAL: \
  { \
	      switch (n) \
		{ \
		  case NIL_INDEX: last_used_literal = _gst_nil_oop; break; \
	          case TRUE_INDEX: last_used_literal = _gst_true_oop; break; \
	          case FALSE_INDEX: last_used_literal = _gst_false_oop; break; \
		  default: return "invalid special object index"; \
		} \
 \
	      *sp++ = OOP_CLASS (last_used_literal); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_INTEGER: \
  { \
	      last_used_literal = FROM_INT (n); \
	      *sp++ = _gst_small_integer_class; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_RETURN_METHOD_STACK_TOP: \
  { \
              block_header header; \
	      if (OOP_CLASS (methodOOP) != _gst_compiled_block_class) \
		return "invalid return from method"; \
 \
              header = GET_BLOCK_HEADER (methodOOP); \
              if (header.clean != (1 << BLK_CLEAN_BITS) - 1) \
		return "invalid return from clean block"; \
 \
	      break; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_RETURN_CONTEXT_STACK_TOP: \
  { break; } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_LINE_NUMBER_BYTECODE: \
  { } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_STORE_RECEIVER_VARIABLE: \
  { \
	      CHECK_REC_VAR (num_ro_rec_vars, n); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_STORE_TEMPORARY_VARIABLE: \
  { \
	      CHECK_TEMP (n); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_STORE_LIT_VARIABLE: \
  { \
	      CHECK_LIT_VARIABLE (true, n); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_SEND: \
  { \
	      if (super \
		  && (!last_used_literal \
		      || (!IS_A_CLASS (last_used_literal) \
		          && !IS_A_METACLASS (last_used_literal)) \
		      || !is_a_kind_of (methodClass, last_used_literal))) \
		return ("Invalid send to super"); \
 \
	      last_used_literal = NULL; \
	      sp -= super + num_args; \
	      if (super && sp[-1] != FROM_INT (SELF)) \
		return ("Invalid send to super"); \
 \
	      sp[-1] = FROM_INT (VARYING); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_POP_INTO_NEW_STACKTOP: \
  { \
	      if (sp[-2] != _gst_array_class) \
	        return ("Array expected"); \
 \
	      if (!arrays || &sp[-2] - stack != arrays->sp) \
	        return ("Invalid Array constructor"); \
 \
	      if (n >= arrays->size) \
	        return ("Out of bounds Array access"); \
 \
	      /* Discard arrays whose construction has ended.  */ \
	      if (n == arrays->size - 1) \
	        { \
	          partially_constructed_array *next = arrays->next; \
	          arrays->next = arrays_pool; \
	          arrays_pool = arrays; \
	          arrays = next; \
	        } \
 \
	      last_used_literal = NULL; \
	      sp--; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_POP_STACK_TOP: \
  { \
	      last_used_literal = NULL; \
	      sp--; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_DUP_STACK_TOP: \
  { \
	      sp++; \
	      sp[-1] = sp[-2]; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_PUSH_OUTER_TEMP: \
  { \
	      if (scopes == 0 || scopes > depth || n >= num_outer_temps[scopes]) \
	        return ("temporary out of range"); \
 \
	      last_used_literal = NULL; \
	      *sp++ = FROM_INT (VARYING); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_STORE_OUTER_TEMP: \
  { \
	      if (scopes == 0 || scopes > depth || n >= num_outer_temps[scopes]) \
	        return ("temporary out of range"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_EXIT_INTERPRETER: \
  { \
	      if (size != 4 \
		  || IP0 != GET_METHOD_BYTECODES (methodOOP) \
		  || *bp != RETURN_CONTEXT_STACK_TOP) \
		return ("bad termination method"); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_JUMP: \
  { \
	      if (merge_stacks (stack, sp - stack, bb_start[ofs]->stack, \
				bb_start[ofs]->sp)) \
		bb_start[ofs]->next = worklist, worklist = bb_start[ofs]; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_POP_JUMP_TRUE: \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_POP_JUMP_FALSE: \
  { \
	      sp--; \
	      bc_len = bp - bp_first; \
	      if (merge_stacks (stack, sp - stack, bb_start[bc_len]->stack, \
				bb_start[bc_len]->sp)) \
		bb_start[bc_len]->next = worklist, worklist = bb_start[bc_len]; \
 \
	      if (merge_stacks (stack, sp - stack, bb_start[ofs]->stack, \
				bb_start[ofs]->sp)) \
		bb_start[ofs]->next = worklist, worklist = bb_start[ofs]; \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_SEND_ARITH: \
  { \
	      sp -= _gst_builtin_selectors[n].numArgs; \
	      sp[-1] = FROM_INT (VARYING); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_SEND_SPECIAL: \
  { \
	      sp -= _gst_builtin_selectors[n + 16].numArgs; \
	      sp[-1] = FROM_INT (VARYING); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_SEND_IMMEDIATE: \
  { \
	      if (n == NEW_COLON_SPECIAL \
		  && IS_INT (last_used_literal) \
		  && last_used_literal != FROM_INT (0) \
		  && sp[-2] == OOP_CLASS (_gst_array_class)) \
		{ \
		  partially_constructed_array *a; \
		  sp--; \
 \
		  /* If possible, reuse an existing struct, else allocate a new one.  */ \
		  if (arrays_pool) \
		    { \
		      a = arrays_pool; \
		      arrays_pool = arrays_pool->next; \
		    } \
		  else \
		    a = alloca (sizeof (partially_constructed_array)); \
 \
		  a->size = TO_INT (last_used_literal); \
		  a->sp = &sp[-1] - stack; \
		  a->next = arrays; \
		  arrays = a; \
 \
		  sp[-1] = _gst_array_class; \
		} \
	      else \
	        { \
		  if (super \
		      && (!last_used_literal \
			  || (!IS_A_CLASS (last_used_literal) \
			      && !IS_A_METACLASS (last_used_literal)) \
			  || !is_a_kind_of (methodClass, last_used_literal))) \
		    return (_gst_debug (), "Invalid send to super"); \
 \
		  last_used_literal = NULL; \
	          sp -= super + _gst_builtin_selectors[n].numArgs; \
	          if (super && sp[-1] != FROM_INT (SELF)) \
		    return ("Invalid send to super"); \
 \
	          sp[-1] = FROM_INT (VARYING); \
		} \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_MAKE_DIRTY_BLOCK: \
  { \
	      if (sp[-1] != _gst_compiled_block_class \
		  || !last_used_literal) \
		return ("CompiledBlock expected"); \
 \
	      error = _gst_verify_method (last_used_literal, num_outer_temps, depth); \
	      if (error) \
	        return (error); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK; \
  MATCH_BYTECODES_EXEC_BASIC_BLOCK_INVALID: \
  { \
	      abort (); \
	    } \
  goto MATCH_BYTECODES_SWITCH_EXEC_BASIC_BLOCK;

#define MATCH_BYTECODES_XLAT_BUILD_CODE_TREE \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_RECEIVER_VARIABLE: \
  { \
      push_tree_node (IP0, NULL, TREE_PUSH | TREE_REC_VAR, \
                      (PTR) (uintptr_t) n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_TEMPORARY_VARIABLE: \
  { \
      push_tree_node (IP0, NULL, TREE_PUSH | TREE_TEMP, \
                      (PTR) (uintptr_t) n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_LIT_CONSTANT: \
  { \
      push_tree_node_oop (IP0, NULL, TREE_PUSH | TREE_LIT_CONST, \
                          literals[n]); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_LIT_VARIABLE: \
  { \
      if (is_a_kind_of (OOP_INT_CLASS (literals[n]), _gst_association_class)) \
        push_tree_node_oop (IP0, NULL, TREE_PUSH | TREE_LIT_VAR, \
                            literals[n]); \
      else \
	{ \
          push_tree_node_oop (IP0, NULL, TREE_PUSH | TREE_LIT_CONST, \
                              literals[n]); \
          push_send_node (IP0, _gst_builtin_selectors[VALUE_SPECIAL].symbol, \
			  0, false, TREE_SEND, 0); \
	} \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_SELF: \
  { \
      push_tree_node (IP0, NULL, \
		      TREE_PUSH | TREE_SELF | self_class_check, NULL); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_SPECIAL: \
  { \
      push_tree_node_oop (IP0, NULL, TREE_PUSH | TREE_LIT_CONST, \
                          *specialOOPs[n]); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_INTEGER: \
  { \
      push_tree_node_oop (IP0, NULL, TREE_PUSH | TREE_LIT_CONST, \
                          FROM_INT (n)); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_RETURN_METHOD_STACK_TOP: \
  { \
      set_top_node_extra (TREE_EXTRA_METHOD_RET, 0); \
      emit_code (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_RETURN_CONTEXT_STACK_TOP: \
  { \
      set_top_node_extra (TREE_EXTRA_RETURN, 0); \
      emit_code (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_LINE_NUMBER_BYTECODE: \
  { \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_STORE_RECEIVER_VARIABLE: \
  { \
      push_tree_node (IP0, pop_tree_node (NULL), \
                      TREE_STORE | TREE_REC_VAR, \
                      (PTR) (uintptr_t) n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_STORE_TEMPORARY_VARIABLE: \
  { \
      push_tree_node (IP0, pop_tree_node (NULL), \
                      TREE_STORE | TREE_TEMP, \
                      (PTR) (uintptr_t) n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_STORE_LIT_VARIABLE: \
  { \
      if (is_a_kind_of (OOP_INT_CLASS (literals[n]), _gst_association_class)) \
        push_tree_node_oop (IP0, pop_tree_node (NULL), \
			    TREE_STORE | TREE_LIT_VAR, literals[n]); \
      else \
	{ \
	  code_tree *value, *var; \
	  inline_cache *ic; \
 \
          push_tree_node_oop (IP0, NULL, \
			      TREE_ALT_PUSH | TREE_LIT_CONST, literals[n]); \
          ic = set_inline_cache (_gst_builtin_selectors[VALUE_COLON_SPECIAL].symbol, \
			         1, false, TREE_SEND, 0); \
 \
	  var = pop_tree_node (NULL); \
	  value = pop_tree_node (var); \
	  push_tree_node (IP0, value, TREE_SEND | TREE_STORE_LIT_VAR, (PTR) ic); \
	} \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_SEND: \
  { \
      push_send_node (IP0, literals[n], num_args, super, TREE_SEND, 0); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_POP_INTO_NEW_STACKTOP: \
  { \
      push_tree_node (IP0, \
                      pop_tree_node (pop_tree_node (NULL)), \
                      TREE_STORE | TREE_POP_INTO_ARRAY, \
                      (PTR) (uintptr_t) n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_POP_STACK_TOP: \
  { \
      set_top_node_extra (TREE_EXTRA_POP, 0); \
      emit_code (); \
 \
      /* This is very important!  If we do not adjust T_SP here, we \
         miscompile superoperators that include a POP/PUSH sequence.  */ \
      t_sp--; \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_DUP_STACK_TOP: \
  { \
      push_tree_node (IP0, NULL, TREE_PUSH | TREE_DUP, NULL); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_PUSH_OUTER_TEMP: \
  { \
      push_tree_node (IP0, NULL, TREE_PUSH | TREE_OUTER_TEMP, \
		      (PTR) (uintptr_t) ((scopes << 8) | n)); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_STORE_OUTER_TEMP: \
  { \
      push_tree_node (IP0, \
                      pop_tree_node (NULL), \
                      TREE_STORE | TREE_OUTER_TEMP,  \
		      (PTR) (uintptr_t) ((scopes << 8) | n)); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_JUMP: \
  { \
      set_top_node_extra (TREE_EXTRA_JMP_ALWAYS, ofs); \
 \
      emit_code (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_POP_JUMP_TRUE: \
  { \
      set_top_node_extra (TREE_EXTRA_JMP_TRUE, ofs); \
 \
      emit_code (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_POP_JUMP_FALSE: \
  { \
      set_top_node_extra (TREE_EXTRA_JMP_FALSE, ofs); \
 \
      emit_code (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_SEND_ARITH: \
  { \
      int op = special_send_bytecodes[n]; \
      const struct builtin_selector *bs = &_gst_builtin_selectors[n]; \
      push_send_node (IP0, bs->symbol, bs->numArgs, false, op, n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_SEND_SPECIAL: \
  { \
      int op = special_send_bytecodes[n + 16]; \
      const struct builtin_selector *bs = &_gst_builtin_selectors[n + 16]; \
      push_send_node (IP0, bs->symbol, bs->numArgs, false, op, n + 16); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_SEND_IMMEDIATE: \
  { \
      const struct builtin_selector *bs = &_gst_builtin_selectors[n]; \
      push_send_node (IP0, bs->symbol, bs->numArgs, super, \
                      TREE_SEND | TREE_NORMAL, n); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_MAKE_DIRTY_BLOCK: \
  { \
      code_tree *arg; \
      arg = pop_tree_node (NULL); \
      push_tree_node (IP0, arg, TREE_SEND | TREE_DIRTY_BLOCK, NULL); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE; \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_EXIT_INTERPRETER: \
  MATCH_BYTECODES_XLAT_BUILD_CODE_TREE_INVALID: \
  { \
      abort (); \
    } \
  goto MATCH_BYTECODES_SWITCH_XLAT_BUILD_CODE_TREE;

#define MATCH_BYTECODES_XLAT_COUNT_SENDS \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_LIT_CONSTANT: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_SELF: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_SPECIAL: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_INTEGER: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_RETURN_METHOD_STACK_TOP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_RETURN_CONTEXT_STACK_TOP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_LINE_NUMBER_BYTECODE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_STORE_RECEIVER_VARIABLE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_STORE_TEMPORARY_VARIABLE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_POP_INTO_NEW_STACKTOP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_POP_STACK_TOP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_DUP_STACK_TOP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_OUTER_TEMP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_STORE_OUTER_TEMP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_JUMP: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_POP_JUMP_TRUE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_POP_JUMP_FALSE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_MAKE_DIRTY_BLOCK: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_EXIT_INTERPRETER: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_INVALID: \
  { } \
  goto MATCH_BYTECODES_SWITCH_XLAT_COUNT_SENDS; \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_PUSH_LIT_VARIABLE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_STORE_LIT_VARIABLE: \
  { \
	if (!is_a_kind_of (OOP_INT_CLASS (literals[n]), _gst_association_class)) \
	  inlineCacheCount++; \
      } \
  goto MATCH_BYTECODES_SWITCH_XLAT_COUNT_SENDS; \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_SEND_ARITH: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_SEND_SPECIAL: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_SEND_IMMEDIATE: \
  MATCH_BYTECODES_XLAT_COUNT_SENDS_SEND: \
  { \
        inlineCacheCount++; \
      } \
  goto MATCH_BYTECODES_SWITCH_XLAT_COUNT_SENDS;
#endif
