/******************************** -*- C -*- ****************************
 *
 *	Byte code interpreter primitives include file
 *
 *
 ***********************************************************************/

/***********************************************************************
 *
 * Copyright 1988,89,90,91,92,94,95,99,2000,2001,2002,2003,2006,2007,2008,2009
 * Free Software Foundation, Inc.
 * Written by Steve Byrne.
 *
 * This file is part of GNU Smalltalk.
 *
 * GNU Smalltalk is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 *
 * Linking GNU Smalltalk statically or dynamically with other modules is
 * making a combined work based on GNU Smalltalk.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the Free Software Foundation
 * give you permission to combine GNU Smalltalk with free software
 * programs or libraries that are released under the GNU LGPL and with
 * independent programs running under the GNU Smalltalk virtual machine.
 *
 * You may copy and distribute such a system following the terms of the
 * GNU GPL for GNU Smalltalk and the licenses of the other code
 * concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 *
 * Note that people who make modified versions of GNU Smalltalk are not
 * obligated to grant this special exception for their modified
 * versions; it is their choice whether to do so.  The GNU General
 * Public License gives permission to release a modified version without
 * this exception; this exception also makes it possible to release a
 * modified version which carries forward this exception.
 *
 * GNU Smalltalk is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GNU Smalltalk; see the file COPYING.  If not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 ***********************************************************************/

#ifdef OPTIMIZE
#define RECEIVER_IS_INT(x)		(true)
#define RECEIVER_IS_OOP(x)		(true)
#define RECEIVER_IS_CLASS(x, y)		(true)
#define RECEIVER_IS_A_KIND_OF(x, y)	(true)
#define PRIM_CHECKS_RECEIVER		PRIM_SUCCEED
#else
#define RECEIVER_IS_INT(x)		IS_INT((x))
#define RECEIVER_IS_OOP(x)		IS_OOP((x))
#define RECEIVER_IS_CLASS(x, y)		IS_CLASS((x), (y))
#define RECEIVER_IS_A_KIND_OF(x, y)	is_a_kind_of((x), (y))
#define PRIM_CHECKS_RECEIVER		(PRIM_SUCCEED | PRIM_FAIL)
#endif

#ifdef HAVE_GMP
#define PRIM_USES_GMP                   (PRIM_SUCCEED | PRIM_FAIL)
#else
#define PRIM_USES_GMP                   PRIM_FAIL
#endif

#ifdef ENABLE_JIT_TRANSLATION
#define PRIM_FAILED			return ((intptr_t) -1)
#define PRIM_SUCCEEDED			return ((intptr_t) 0)
#define PRIM_SUCCEEDED_RELOAD_IP	return ((intptr_t) native_ip)
#else
#define PRIM_FAILED			return (true)
#define PRIM_SUCCEEDED			return (false)
#define PRIM_SUCCEEDED_RELOAD_IP	return (false)
#endif

#define INT_BIN_OP(op, noOverflow) {            \
    OOP	oop1;					\
    OOP	oop2;					\
    mst_Boolean overflow;                       \
    oop2 = POP_OOP();				\
    oop1 = POP_OOP();				\
    if COMMON (RECEIVER_IS_INT(oop1) && IS_INT(oop2)) {\
      intptr_t iarg1, iarg2;			\
      iarg1 = TO_INT(oop1);			\
      iarg2 = TO_INT(oop2);			\
						\
      oop1 = op;                                \
      if COMMON (noOverflow || !overflow) {	\
        PUSH_OOP(oop1);				\
        PRIM_SUCCEEDED;				\
      }                                         \
    }						\
    UNPOP(2);					\
    PRIM_FAILED;                                \
  }

#define BOOL_BIN_OP(operator) {					\
    OOP	oop1;							\
    OOP	oop2;							\
    oop2 = POP_OOP();						\
    oop1 = POP_OOP();						\
    if COMMON (RECEIVER_IS_INT(oop1) && IS_INT(oop2)) {		\
      PUSH_BOOLEAN( ((intptr_t)oop1) operator ((intptr_t)oop2) );	\
      PRIM_SUCCEEDED;						\
    }								\
    UNPOP(2);							\
    PRIM_FAILED;                                                \
  }


/* SmallInteger + arg  */
static intptr_t
VMpr_SmallInteger_plus (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_minus (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_lt (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_gt (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_le (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_ge (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_eq (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_ne (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_times (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_divide (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_modulo (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_intDiv (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_quo (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_bitAnd (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_bitOr (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_bitXor (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_bitShift (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_scramble (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_asFloatD (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_asFloatE (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_asFloatQ (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_eq (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_ne (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_lt (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_le (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_gt (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_ge (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_times (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_intDiv (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_modulo (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_divExact (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_quo (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_rem (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_negated (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_bitAnd (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_bitOr (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_bitXor (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_bitInvert (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_bitShift (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_plus (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_minus (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_gcd (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_asFloatD (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_asFloatE (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_LargeInteger_asFloatQ (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_arith (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_truncated (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_fractionPart (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_exponent (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_timesTwoPower (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_asFloatE (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatD_asFloatQ (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_arith (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_truncated (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_fractionPart (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_exponent (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_timesTwoPower (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_asFloatD (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatE_asFloatQ (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_arith (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_truncated (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_fractionPart (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_exponent (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_timesTwoPower (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_asFloatD (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FloatQ_asFloatE (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_basicAt (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_basicAtPut (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_basicSize (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CharacterArray_valueAt (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CharacterArray_valueAtPut (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CompiledCode_verificationResult (int id ATTRIBUTE_UNUSED,
                                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CompiledBlock_create (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CompiledMethod_create (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_shallowCopy (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_basicNew (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_basicNewColon (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_become (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_instVarAt (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_instVarAtPut (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_hash (int id ATTRIBUTE_UNUSED,
                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_asObject (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SmallInteger_nextValidOop (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_someInstance (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_nextInstance (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_becomeForward (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_allOwners (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ContextPart_thisContext (int id ATTRIBUTE_UNUSED,
                              volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ContextPart_continue (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Continuation_resume (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_BlockClosure_value (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_BlockClosure_cull (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_BlockClosure_valueAndResumeOnUnwind (int id ATTRIBUTE_UNUSED,
                                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_BlockClosure_valueWithArguments (int id ATTRIBUTE_UNUSED,
                                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_perform (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_performWithArguments (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Semaphore_notifyAll (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Semaphore_signalNotify (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Semaphore_lock (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Semaphore_wait (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Semaphore_waitAfterSignalling (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Process_suspend (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Process_resume (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Process_singleStepWaitingOn (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Process_yield (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Processor_pause (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_flushCache (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CompiledCode_discardTranslation (int id ATTRIBUTE_UNUSED,
                                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_changeClassTo (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Time_timezoneBias (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Time_timezone (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Time_secondClock (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Time_nanosecondClock (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Time_millisecondClock (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Processor_signalAt (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Processor_isTimeoutProgrammed (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_String_similarityTo (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_String_hash (int id ATTRIBUTE_UNUSED,
                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ArrayedCollection_equal (int id ATTRIBUTE_UNUSED,
                              volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ArrayedCollection_indexOfStartingAt (int id ATTRIBUTE_UNUSED,
                                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ArrayedCollection_replaceFromToWithStartingAt (int id ATTRIBUTE_UNUSED,
                                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_identity (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_class (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_quit (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_abort (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Dictionary_at (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_bootstrapException (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Character_create (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_UnicodeCharacter_create (int id ATTRIBUTE_UNUSED,
                              volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Character_equal (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Symbol_intern (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Dictionary_new (int id ATTRIBUTE_UNUSED,
                     volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Memory_addressOfOOP (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Memory_addressOf (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_backtrace (int id ATTRIBUTE_UNUSED,
                                 volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_getTraceFlag (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_setTraceFlag (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Memory_at (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Memory_atPut (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_methodsFor (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_methodsForIfTrue (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Processor_disableEnableInterrupts (int id ATTRIBUTE_UNUSED,
                                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Processor_signalOnInterrupt (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_getSpaceGrowRate (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_setSpaceGrowRate (int id ATTRIBUTE_UNUSED,
                                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_getSmoothingFactor (int id ATTRIBUTE_UNUSED,
                                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_setSmoothingFactor (int id ATTRIBUTE_UNUSED,
                                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_getGrowThresholdPercent (int id ATTRIBUTE_UNUSED,
                                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_setGrowThresholdPercent (int id ATTRIBUTE_UNUSED,
                                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_getBigObjectThreshold (int id ATTRIBUTE_UNUSED,
                                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_setBigObjectThreshold (int id ATTRIBUTE_UNUSED,
                                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_growTo (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_update (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_allocType (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_sin (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_cos (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_tan (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_arcSin (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_arcCos (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_arcTan (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_exp (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_ln (int id ATTRIBUTE_UNUSED,
               volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_pow (int id ATTRIBUTE_UNUSED,
                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_free (int id ATTRIBUTE_UNUSED,
                   volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_sqrt (int id ATTRIBUTE_UNUSED,
                 volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Float_ceil_floor (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_basicNewFixed (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_basicNewFixedColon (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_tenure (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_makeFixed (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_at (int id ATTRIBUTE_UNUSED,
                 volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_atPut (int id ATTRIBUTE_UNUSED,
                    volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_address (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CObject_addressColon (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CString_replaceWith (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ByteArray_fromCData_size (int id ATTRIBUTE_UNUSED,
                               volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_String_fromCData_size (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_String_fromCData (int id ATTRIBUTE_UNUSED,
                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_String_ByteArray_asCData (int id ATTRIBUTE_UNUSED,
                               volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_byteCodeCounter (int id ATTRIBUTE_UNUSED,
                                       volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_debug (int id ATTRIBUTE_UNUSED,
                             volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_isUntrusted (int id ATTRIBUTE_UNUSED,
                         volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_makeUntrusted (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_isReadOnly (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_makeReadOnly (int id ATTRIBUTE_UNUSED,
                          volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_primCompile (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Behavior_primCompileIfError (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CCallbackDescriptor_link (int id ATTRIBUTE_UNUSED,
                               volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CFuncDescriptor_addressOf (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_snapshot (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_basicPrint (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_makeWeak (int id ATTRIBUTE_UNUSED,
                      volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Stream_fileInLine (int id ATTRIBUTE_UNUSED,
                        volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FileDescriptor_fileOp (int id ATTRIBUTE_UNUSED,
                            volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_FileDescriptor_socketOp (int id ATTRIBUTE_UNUSED,
                              volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CFuncDescriptor_asyncCall (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_CFuncDescriptor_call (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Object_makeEphemeron (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_Namespace_setCurrent (int id ATTRIBUTE_UNUSED,
                           volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_ObjectMemory_gcPrimitives (int id ATTRIBUTE_UNUSED,
                                volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_SystemDictionary_rawProfile (int id ATTRIBUTE_UNUSED,
                                  volatile int numArgs ATTRIBUTE_UNUSED);

static intptr_t
VMpr_HOLE (int id ATTRIBUTE_UNUSED,
           volatile int numArgs ATTRIBUTE_UNUSED);


intptr_t
VMpr_SmallInteger_plus (int id,
                        volatile int numArgs)
#line 121 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (add_with_check (oop1, oop2, &overflow), false);
}

/* SmallInteger - arg */
intptr_t
VMpr_SmallInteger_minus (int id,
                         volatile int numArgs)
#line 128 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (sub_with_check (oop1, oop2, &overflow), false);
}

/* SmallInteger < arg */
intptr_t
VMpr_SmallInteger_lt (int id,
                      volatile int numArgs)
#line 135 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP (<);
}

/* SmallInteger > arg */
intptr_t
VMpr_SmallInteger_gt (int id,
                      volatile int numArgs)
#line 142 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP (>);
}

/* SmallInteger <= arg */
intptr_t
VMpr_SmallInteger_le (int id,
                      volatile int numArgs)
#line 149 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP (<=);
}

/* SmallInteger >= arg */
intptr_t
VMpr_SmallInteger_ge (int id,
                      volatile int numArgs)
#line 156 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP (>=);
}

/* SmallInteger =, == arg */
intptr_t
VMpr_SmallInteger_eq (int id,
                      volatile int numArgs)
#line 163 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP ( ==);
}

/* SmallInteger ~=, ~~ arg */
intptr_t
VMpr_SmallInteger_ne (int id,
                      volatile int numArgs)
#line 170 "prims.def"
{
  _gst_primitives_executed++;
  BOOL_BIN_OP (!=);
}

/* SmallInteger * arg */
intptr_t
VMpr_SmallInteger_times (int id,
                         volatile int numArgs)
#line 177 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (mul_with_check (oop1, oop2, &overflow), false);
}

/* SmallInteger / arg */
/* quotient as exact as possible */
intptr_t
VMpr_SmallInteger_divide (int id,
                          volatile int numArgs)
#line 185 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (RECEIVER_IS_INT (oop1) && IS_INT (oop2) && oop2 != FROM_INT (0))
    {
      intptr_t iarg1, iarg2, result;
      iarg1 = TO_INT (oop1);
      iarg2 = TO_INT (oop2);
      result = iarg1 / iarg2;
      iarg2 *= result;
      if COMMON (iarg1 == iarg2)
	{
	  PUSH_INT (result);
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* SmallInteger \\ arg */
/* remainder truncated towards -infinity */
intptr_t
VMpr_SmallInteger_modulo (int id,
                          volatile int numArgs)
#line 212 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1) && IS_INT (oop2) && oop2 != FROM_INT (0))
    {
      intptr_t iarg1, iarg2, result;
      iarg1 = TO_INT (oop1);
      iarg2 = TO_INT (oop2);
      result = iarg1 % iarg2;
      PUSH_INT (result
		&& ((result ^ iarg2) < 0) ? result + iarg2 : result);
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* SmallInteger // arg */
/* quotient truncated towards -infinity */
intptr_t
VMpr_SmallInteger_intDiv (int id,
                          volatile int numArgs)
#line 236 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1) && IS_INT (oop2) && oop2 != FROM_INT (0))
    {
      intptr_t iarg1, iarg2;
      iarg1 = TO_INT (oop1);
      iarg2 = TO_INT (oop2);
      if (iarg2 < 0)
	{			/* make the divisor positive */
	  iarg1 = -iarg1;
	  iarg2 = -iarg2;
	}
      if (iarg1 < 0)		/* differing signs => use black magic */
	PUSH_INT (-((iarg2 - 1 - iarg1) / iarg2));
      else
	PUSH_INT (iarg1 / iarg2);

      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* SmallInteger quo: arg */
/* quotient truncated towards 0 */
intptr_t
VMpr_SmallInteger_quo (int id,
                       volatile int numArgs)
#line 267 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1) && IS_INT (oop2))
    {
      intptr_t iarg1, iarg2;
      iarg1 = TO_INT (oop1);
      iarg2 = TO_INT (oop2);
      if COMMON (iarg2 != 0)
	{
	  PUSH_INT (iarg1 / iarg2);
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* SmallInteger bitAnd: arg */
intptr_t
VMpr_SmallInteger_bitAnd (int id,
                          volatile int numArgs)
#line 291 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (tagged_and (oop1, oop2), true);
}

/* SmallInteger bitOr: arg */
intptr_t
VMpr_SmallInteger_bitOr (int id,
                         volatile int numArgs)
#line 298 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (tagged_or (oop1, oop2), true);
}

/* SmallInteger bitXor: arg */
intptr_t
VMpr_SmallInteger_bitXor (int id,
                          volatile int numArgs)
#line 305 "prims.def"
{
  _gst_primitives_executed++;
  INT_BIN_OP (tagged_xor (oop1, oop2), true);
}

/* SmallInteger bitShift: arg */
intptr_t
VMpr_SmallInteger_bitShift (int id,
                            volatile int numArgs)
#line 312 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1) && IS_INT (oop2))
    {
      intptr_t iarg1;
      intptr_t iarg2;
      iarg1 = TO_INT (oop1);
      iarg2 = TO_INT (oop2);
      if (iarg2 < 0)
	{
	  if (iarg2 >= -ST_INT_SIZE)
	    PUSH_INT (iarg1 >> -iarg2);
	  else
	    PUSH_INT (iarg1 >> ST_INT_SIZE);

	  PRIM_SUCCEEDED;
	}
      if COMMON (iarg2 < ST_INT_SIZE)
	{
	  intptr_t result = iarg1 << iarg2;
	  if ((result >> iarg2) == iarg1 && !INT_OVERFLOW(result))
	    {
	      PUSH_INT (result);
	      PRIM_SUCCEEDED;
	    }
	}
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* SmallInteger scramble */

intptr_t
VMpr_SmallInteger_scramble (int id,
                            volatile int numArgs)
#line 351 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1))
    {
      PUSH_INT (scramble (TO_INT (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}


/* SmallInteger asFloatD */

intptr_t
VMpr_SmallInteger_asFloatD (int id,
                            volatile int numArgs)
#line 369 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1))
    {
      PUSH_OOP (floatd_new ((double) TO_INT (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* SmallInteger asFloatE */

intptr_t
VMpr_SmallInteger_asFloatE (int id,
                            volatile int numArgs)
#line 386 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1))
    {
      PUSH_OOP (floate_new ((float) TO_INT (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* SmallInteger asFloatQ */

intptr_t
VMpr_SmallInteger_asFloatQ (int id,
                            volatile int numArgs)
#line 403 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_INT (oop1))
    {
      PUSH_OOP (floatq_new ((long double) TO_INT (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

intptr_t
VMpr_LargeInteger_eq (int id,
                      volatile int numArgs)
#line 418 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result == 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}

intptr_t
VMpr_LargeInteger_ne (int id,
                      volatile int numArgs)
#line 460 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result != 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_lt (int id,
                      volatile int numArgs)
#line 503 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result < 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_le (int id,
                      volatile int numArgs)
#line 546 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result <= 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_gt (int id,
                      volatile int numArgs)
#line 589 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result > 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_ge (int id,
                      volatile int numArgs)
#line 632 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  OOP oop1, oop2;
  int result;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  result = _gst_mpz_cmp (&a, &b);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  PUSH_BOOLEAN (result >= 0);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_times (int id,
                         volatile int numArgs)
#line 675 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_mul (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}



intptr_t
VMpr_LargeInteger_intDiv (int id,
                          volatile int numArgs)
#line 721 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz q = { 0, 0, NULL };
  gst_mpz r = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && oop2 != FROM_INT (0))
    _gst_mpz_fdiv_qr_si (&q, &a, TO_INT(oop2));
  else if (IS_OOP (oop2)
	   && SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class)
    {
      _gst_mpz_from_oop (&b, oop2);
      _gst_mpz_fdiv_qr (&q, &r, &a, &b);
      _gst_mpz_clear (&r);
      _gst_mpz_clear (&b);
    }
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  oop3 = _gst_oop_from_mpz (&q);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&q);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_modulo (int id,
                          volatile int numArgs)
#line 771 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz q = { 0, 0, NULL };
  gst_mpz r = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && oop2 != FROM_INT (0))
    {
      mp_limb_t rem = _gst_mpz_fdiv_qr_si (&q, &a, TO_INT(oop2));
      oop3 = FROM_INT (rem);
    }
  else if (IS_OOP (oop2)
	   && SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class)
    {
      _gst_mpz_from_oop (&b, oop2);
      _gst_mpz_fdiv_qr (&q, &r, &a, &b);
      oop3 = _gst_oop_from_mpz (&r);
      _gst_mpz_clear (&r);
      _gst_mpz_clear (&b);
    }
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_clear (&a);
  _gst_mpz_clear (&q);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_divExact (int id,
                            volatile int numArgs)
#line 824 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz q = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && oop2 != FROM_INT (0))
    _gst_mpz_tdiv_qr_si (&q, &a, TO_INT(oop2));
  else if (IS_OOP (oop2)
	   && SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class)
    {
      _gst_mpz_from_oop (&b, oop2);
      _gst_mpz_divexact (&q, &a, &b);
      _gst_mpz_clear (&b);
    }
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  oop3 = _gst_oop_from_mpz (&q);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&q);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}

intptr_t
VMpr_LargeInteger_quo (int id,
                       volatile int numArgs)
#line 871 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz q = { 0, 0, NULL };
  gst_mpz r = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && oop2 != FROM_INT (0))
    _gst_mpz_tdiv_qr_si (&q, &a, TO_INT(oop2));
  else if (IS_OOP (oop2)
	   && SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class)
    {
      _gst_mpz_from_oop (&b, oop2);
      _gst_mpz_tdiv_qr (&q, &r, &a, &b);
      _gst_mpz_clear (&r);
      _gst_mpz_clear (&b);
    }
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  oop3 = _gst_oop_from_mpz (&q);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&q);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_rem (int id,
                       volatile int numArgs)
#line 921 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz q = { 0, 0, NULL };
  gst_mpz r = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && oop2 != FROM_INT (0))
    {
      mp_limb_t rem = _gst_mpz_tdiv_qr_si (&q, &a, TO_INT(oop2));
      oop3 = FROM_INT (rem);
    }
  else if (IS_OOP (oop2)
	   && SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class)
    {
      _gst_mpz_from_oop (&b, oop2);
      _gst_mpz_tdiv_qr (&q, &r, &a, &b);
      oop3 = _gst_oop_from_mpz (&r);
      _gst_mpz_clear (&r);
      _gst_mpz_clear (&b);
    }
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_clear (&a);
  _gst_mpz_clear (&q);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_negated (int id,
                           volatile int numArgs)
#line 974 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_sub (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_bitAnd (int id,
                          volatile int numArgs)
#line 1006 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_and (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_bitOr (int id,
                         volatile int numArgs)
#line 1051 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_ior (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_bitXor (int id,
                          volatile int numArgs)
#line 1096 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_xor (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_bitInvert (int id,
                             volatile int numArgs)
#line 1141 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_com (&c, &a);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_bitShift (int id,
                            volatile int numArgs)
#line 1172 "prims.def"
{
#ifdef HAVE_GMP
  int n;
  gst_mpz a = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop2))
    n = TO_INT (oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if (n > 0)
    _gst_mpz_mul_2exp (&c, &a, n);
  else
    _gst_mpz_div_2exp (&c, &a, -n);

  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#endif
  PRIM_FAILED;
}


intptr_t
VMpr_LargeInteger_plus (int id,
                        volatile int numArgs)
#line 1216 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_add (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_minus (int id,
                         volatile int numArgs)
#line 1261 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  _gst_mpz_sub (&c, &a, &b);
  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_gcd (int id,
                       volatile int numArgs)
#line 1306 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  gst_mpz b = { 0, 0, NULL };
  gst_mpz c = { 0, 0, NULL };
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2)
      || SUPERCLASS (OOP_CLASS (oop2)) == _gst_large_integer_class
      || OOP_CLASS (oop2) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&b, oop2);
  else
    {
      _gst_mpz_clear (&a);
      UNPOP (2);
      PRIM_FAILED;
    }

  if (oop1 == FROM_INT(0) || oop2 == FROM_INT(0)
      || (IS_OOP(oop1) && OOP_CLASS (oop1) == _gst_large_zero_integer_class)
      || (IS_OOP(oop2) && OOP_CLASS (oop2) == _gst_large_zero_integer_class))
    /* Return the non-zero number between a and b */
    _gst_mpz_add (&c, &a, &b);
  else
    _gst_mpz_gcd (&c, &a, &b);

  oop3 = _gst_oop_from_mpz (&c);
  _gst_mpz_clear (&a);
  _gst_mpz_clear (&b);
  _gst_mpz_clear (&c);
  PUSH_OOP (oop3);
  PRIM_SUCCEEDED;
#else
  PRIM_FAILED;
#endif
}


intptr_t
VMpr_LargeInteger_asFloatD (int id,
                            volatile int numArgs)
#line 1358 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  OOP oop1, oop2;
  double d;

  _gst_primitives_executed++;
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (_gst_mpz_get_d (&a, &d))
    {
      oop2 = floatd_new (d);
      _gst_mpz_clear (&a);
      PUSH_OOP (oop2);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
#endif
  PRIM_FAILED;
}


intptr_t
VMpr_LargeInteger_asFloatE (int id,
                            volatile int numArgs)
#line 1391 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  OOP oop1, oop2;
  double d;

  _gst_primitives_executed++;
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (_gst_mpz_get_d (&a, &d) && (double) (float) d == d)
    {
      oop2 = floate_new (d);
      _gst_mpz_clear (&a);
      PUSH_OOP (oop2);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
#endif
  PRIM_FAILED;
}


intptr_t
VMpr_LargeInteger_asFloatQ (int id,
                            volatile int numArgs)
#line 1424 "prims.def"
{
#ifdef HAVE_GMP
  gst_mpz a = { 0, 0, NULL };
  OOP oop1, oop2;
  long double d;

  _gst_primitives_executed++;
  oop1 = POP_OOP ();
  if COMMON (IS_INT (oop1)
      || SUPERCLASS (OOP_CLASS (oop1)) == _gst_large_integer_class
      || OOP_CLASS (oop1) == _gst_large_zero_integer_class)
    _gst_mpz_from_oop (&a, oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (_gst_mpz_get_ld (&a, &d) && (long double) (float) d == d)
    {
      oop2 = floatq_new (d);
      _gst_mpz_clear (&a);
      PUSH_OOP (oop2);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
#endif
  PRIM_FAILED;
}

intptr_t
VMpr_FloatD_arith (int id,
                   volatile int numArgs)
#line 1466 "prims.def"
{
  double farg2;
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_CLASS (oop2, _gst_floatd_class))
    farg2 = FLOATD_OOP_VALUE (oop2);
  else if (IS_INT (oop2))
    farg2 = (double) TO_INT (oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1;
      farg1 = FLOATD_OOP_VALUE (oop1);
      switch (id)
	{
	case 0:
	  PUSH_OOP (floatd_new (farg1 + farg2));
	  break;
	case -1:
	  PUSH_OOP (floatd_new (farg1 - farg2));
	  break;
	case -2:
	  PUSH_BOOLEAN (farg1 < farg2);
	  break;
	case -3:
	  PUSH_BOOLEAN (farg1 > farg2);
	  break;
	case -4:
	  PUSH_BOOLEAN (farg1 <= farg2);
	  break;
	case -5:
	  PUSH_BOOLEAN (farg1 >= farg2);
	  break;
	case -6:
	  PUSH_BOOLEAN (farg1 == farg2);
	  break;
	case -7:
	  PUSH_BOOLEAN (farg1 != farg2);
	  break;
	case -8:
	  PUSH_OOP (floatd_new (farg1 * farg2));
	  break;
	case -9:
	  PUSH_OOP (floatd_new (farg1 / farg2));
	  break;
	}
      PRIM_SUCCEEDED;
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* FloatD truncated */
intptr_t
VMpr_FloatD_truncated (int id,
                       volatile int numArgs)
#line 1530 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      double oopValue = FLOATD_OOP_VALUE (oop1);
      if COMMON (oopValue >= MIN_ST_INT && oopValue <= MAX_ST_INT)
	{
	  PUSH_INT (lrint (trunc (oopValue)));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD fractionPart */
intptr_t
VMpr_FloatD_fractionPart (int id,
                          volatile int numArgs)
#line 1550 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1;
      farg1 = FLOATD_OOP_VALUE (oop1);
      farg1 -= (farg1 < 0.0) ? ceil (farg1) : floor (farg1);

      PUSH_OOP (floatd_new (farg1));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD exponent */
intptr_t
VMpr_FloatD_exponent (int id,
                      volatile int numArgs)
#line 1570 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1;
      int intArg1;

      farg1 = FLOATD_OOP_VALUE (oop1);
      if (farg1 == 0.0)
	intArg1 = 1;
      else
	{
	  frexp (FLOATD_OOP_VALUE (oop1), &intArg1);
	  intArg1--;
	}
      PUSH_INT (intArg1);
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD timesTwoPower: */
intptr_t
VMpr_FloatD_timesTwoPower (int id,
                           volatile int numArgs)
#line 1597 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (RECEIVER_IS_CLASS (oop1, _gst_floatd_class) && IS_INT (oop2))
    {
      double farg1;
      intptr_t iarg2;
      farg1 = FLOATD_OOP_VALUE (oop1);
      iarg2 = TO_INT (oop2);
      PUSH_OOP (floatd_new (ldexp (farg1, iarg2)));
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* FloatD asFloatE */

intptr_t
VMpr_FloatD_asFloatE (int id,
                      volatile int numArgs)
#line 1620 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      PUSH_OOP (floate_new (FLOATD_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD asFloatQ */

intptr_t
VMpr_FloatD_asFloatQ (int id,
                      volatile int numArgs)
#line 1637 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatd_class))
    {
      PUSH_OOP (floatq_new (FLOATD_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

intptr_t
VMpr_FloatE_arith (int id,
                   volatile int numArgs)
#line 1662 "prims.def"
{
  double farg2;
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_CLASS (oop2, _gst_floate_class))
    farg2 = FLOATE_OOP_VALUE (oop2);
  else if (IS_INT (oop2))
    farg2 = (double) TO_INT (oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1;
      farg1 = FLOATE_OOP_VALUE (oop1);
      switch (id)
	{
	case 0:
	  PUSH_OOP (floate_new (farg1 + farg2));
	  break;
	case -1:
	  PUSH_OOP (floate_new (farg1 - farg2));
	  break;
	case -2:
	  PUSH_BOOLEAN (farg1 < farg2);
	  break;
	case -3:
	  PUSH_BOOLEAN (farg1 > farg2);
	  break;
	case -4:
	  PUSH_BOOLEAN (farg1 <= farg2);
	  break;
	case -5:
	  PUSH_BOOLEAN (farg1 >= farg2);
	  break;
	case -6:
	  PUSH_BOOLEAN (farg1 == farg2);
	  break;
	case -7:
	  PUSH_BOOLEAN (farg1 != farg2);
	  break;
	case -8:
	  PUSH_OOP (floate_new (farg1 * farg2));
	  break;
	case -9:
	  PUSH_OOP (floate_new (farg1 / farg2));
	  break;
	}
      PRIM_SUCCEEDED;
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* FloatD truncated */
intptr_t
VMpr_FloatE_truncated (int id,
                       volatile int numArgs)
#line 1726 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      double oopValue = FLOATE_OOP_VALUE (oop1);
      if COMMON (oopValue >= MIN_ST_INT && oopValue <= MAX_ST_INT)
	{
	  PUSH_INT (lrintf (truncf (oopValue)));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD fractionPart */
intptr_t
VMpr_FloatE_fractionPart (int id,
                          volatile int numArgs)
#line 1746 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1;
      farg1 = FLOATE_OOP_VALUE (oop1);
      farg1 -= (farg1 < 0.0) ? ceil (farg1) : floor (farg1);

      PUSH_OOP (floate_new (farg1));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD exponent */
intptr_t
VMpr_FloatE_exponent (int id,
                      volatile int numArgs)
#line 1766 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1;
      int intArg1;

      farg1 = FLOATE_OOP_VALUE (oop1);
      if (farg1 == 0.0)
	intArg1 = 1;
      else
	{
	  frexp (FLOATE_OOP_VALUE (oop1), &intArg1);
	  intArg1--;
	}
      PUSH_INT (intArg1);
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD timesTwoPower: */
intptr_t
VMpr_FloatE_timesTwoPower (int id,
                           volatile int numArgs)
#line 1793 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (RECEIVER_IS_CLASS (oop1, _gst_floate_class) && IS_INT (oop2))
    {
      double farg1;
      intptr_t iarg2;
      farg1 = FLOATE_OOP_VALUE (oop1);
      iarg2 = TO_INT (oop2);
      PUSH_OOP (floate_new (ldexp (farg1, iarg2)));
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* FloatE asFloatD */

intptr_t
VMpr_FloatE_asFloatD (int id,
                      volatile int numArgs)
#line 1816 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      PUSH_OOP (floatd_new (FLOATE_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD asFloatQ */

intptr_t
VMpr_FloatE_asFloatQ (int id,
                      volatile int numArgs)
#line 1833 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floate_class))
    {
      PUSH_OOP (floatq_new (FLOATE_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

intptr_t
VMpr_FloatQ_arith (int id,
                   volatile int numArgs)
#line 1858 "prims.def"
{
  long double farg2;
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_CLASS (oop2, _gst_floatq_class))
    farg2 = FLOATQ_OOP_VALUE (oop2);
  else if (IS_INT (oop2))
    farg2 = (long double) TO_INT (oop2);
  else
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1;
      farg1 = FLOATQ_OOP_VALUE (oop1);
      switch (id)
	{
	case 0:
	  PUSH_OOP (floatq_new (farg1 + farg2));
	  break;
	case -1:
	  PUSH_OOP (floatq_new (farg1 - farg2));
	  break;
	case -2:
	  PUSH_BOOLEAN (farg1 < farg2);
	  break;
	case -3:
	  PUSH_BOOLEAN (farg1 > farg2);
	  break;
	case -4:
	  PUSH_BOOLEAN (farg1 <= farg2);
	  break;
	case -5:
	  PUSH_BOOLEAN (farg1 >= farg2);
	  break;
	case -6:
	  PUSH_BOOLEAN (farg1 == farg2);
	  break;
	case -7:
	  PUSH_BOOLEAN (farg1 != farg2);
	  break;
	case -8:
	  PUSH_OOP (floatq_new (farg1 * farg2));
	  break;
	case -9:
	  PUSH_OOP (floatq_new (farg1 / farg2));
	  break;
	}
      PRIM_SUCCEEDED;
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* FloatD truncated */
intptr_t
VMpr_FloatQ_truncated (int id,
                       volatile int numArgs)
#line 1922 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      long double oopValue = FLOATQ_OOP_VALUE (oop1);
      if COMMON (oopValue >= MIN_ST_INT && oopValue <= MAX_ST_INT)
	{
	  PUSH_INT (lrintl (truncl (oopValue)));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD fractionPart */
intptr_t
VMpr_FloatQ_fractionPart (int id,
                          volatile int numArgs)
#line 1942 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1;
      farg1 = FLOATQ_OOP_VALUE (oop1);
      farg1 -= (farg1 < 0.0) ? ceill (farg1) : floorl (farg1);

      PUSH_OOP (floatq_new (farg1));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD exponent */
intptr_t
VMpr_FloatQ_exponent (int id,
                      volatile int numArgs)
#line 1962 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1;
      int intArg1;

      farg1 = FLOATQ_OOP_VALUE (oop1);
      if (farg1 == 0.0)
	intArg1 = 1;
      else
	{
	  frexpl (FLOATQ_OOP_VALUE (oop1), &intArg1);
	  intArg1--;
	}
      PUSH_INT (intArg1);
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD timesTwoPower: */
intptr_t
VMpr_FloatQ_timesTwoPower (int id,
                           volatile int numArgs)
#line 1989 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (RECEIVER_IS_CLASS (oop1, _gst_floatq_class) && IS_INT (oop2))
    {
      long double farg1;
      intptr_t iarg2;
      farg1 = FLOATQ_OOP_VALUE (oop1);
      iarg2 = TO_INT (oop2);
      PUSH_OOP (floatq_new (ldexpl (farg1, iarg2)));
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* FloatQ asFloatD */

intptr_t
VMpr_FloatQ_asFloatD (int id,
                      volatile int numArgs)
#line 2012 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      PUSH_OOP (floatd_new (FLOATQ_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* FloatD asFloatQ */

intptr_t
VMpr_FloatQ_asFloatE (int id,
                      volatile int numArgs)
#line 2029 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (RECEIVER_IS_CLASS (oop1, _gst_floatq_class))
    {
      PUSH_OOP (floate_new (FLOATQ_OOP_VALUE (oop1)));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}



/* Object at:, Object basicAt: */

intptr_t
VMpr_Object_basicAt (int id,
                     volatile int numArgs)
#line 2048 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  POP_N_OOPS (numArgs - 1);
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      oop1 = index_oop (oop1, arg2);
      if COMMON (oop1)
	{
	  SET_STACKTOP (oop1);
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (numArgs);
  PRIM_FAILED;
}

/* Object at:put:, Object basicAt:put: */
intptr_t
VMpr_Object_basicAtPut (int id,
                        volatile int numArgs)
#line 2073 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (IS_INT (oop2) && !IS_OOP_READONLY (oop1))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if COMMON (index_oop_put (oop1, arg2, oop3))
	{
	  SET_STACKTOP (oop3);
	  PRIM_SUCCEEDED;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* Object basicSize; Object size; String size */
intptr_t
VMpr_Object_basicSize (int id,
                       volatile int numArgs)
#line 2099 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  PUSH_INT (NUM_INDEXABLE_FIELDS (oop1));
  PRIM_SUCCEEDED;
}

/* CharacterArray valueAt: */

intptr_t
VMpr_CharacterArray_valueAt (int id,
                             volatile int numArgs)
#line 2111 "prims.def"
{
  OOP oop1;
  OOP oop2;
  intptr_t spec;

  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  spec = CLASS_INSTANCE_SPEC (OOP_CLASS (oop1));

  switch (spec & ISP_INDEXEDVARS)
    {
    case GST_ISP_CHARACTER:
      spec ^= GST_ISP_CHARACTER ^ GST_ISP_UCHAR;
      break;
    case GST_ISP_UTF32:
      spec ^= GST_ISP_UTF32 ^ GST_ISP_UINT;
      break;
    default:
      UNPOP (1);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      oop1 = index_oop_spec (oop1, OOP_TO_OBJ (oop1), arg2, spec);
      if COMMON (oop1)
	{
	  SET_STACKTOP (oop1);
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* CharacterArray valueAt:put: */
intptr_t
VMpr_CharacterArray_valueAtPut (int id,
                                volatile int numArgs)
#line 2152 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  intptr_t spec;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  spec = CLASS_INSTANCE_SPEC (OOP_CLASS (oop1));

  switch (spec & ISP_INDEXEDVARS)
    {
    case GST_ISP_CHARACTER:
      spec ^= GST_ISP_CHARACTER ^ GST_ISP_UCHAR;
      break;
    case GST_ISP_UTF32:
      spec ^= GST_ISP_UTF32 ^ GST_ISP_UINT;
      break;
    default:
      UNPOP (2);
      PRIM_FAILED;
    }

  if COMMON (IS_INT (oop2) && !IS_OOP_READONLY (oop1))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if COMMON (index_oop_put_spec (oop1, OOP_TO_OBJ (oop1), arg2, oop3, spec))
	{
	  SET_STACKTOP (oop3);
	  PRIM_SUCCEEDED;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}



/* CompiledCode verificationResult */
intptr_t
VMpr_CompiledCode_verificationResult (int id,
                                      volatile int numArgs)
#line 2196 "prims.def"
{
  OOP oop1 = STACKTOP ();
  const char *result = _gst_verify_method (oop1, NULL, 0);
  OOP resultOOP = result ? _gst_string_new (result) : _gst_nil_oop;

  SET_STACKTOP (resultOOP);
  PRIM_SUCCEEDED;
}

/* CompiledBlock numArgs:numTemps:bytecodes:depth:literals: */
intptr_t
VMpr_CompiledBlock_create (int id,
                           volatile int numArgs)
#line 2207 "prims.def"
{
  OOP *_gst_literals = OOP_TO_OBJ (STACK_AT (0))->data;
  int depth = TO_INT (STACK_AT (1));
  OOP bytecodesOOP = STACK_AT (2);
  int blockTemps = TO_INT (STACK_AT (3));
  int blockArgs = TO_INT (STACK_AT (4));
  bc_vector bytecodes = _gst_extract_bytecodes (bytecodesOOP);

  OOP block =
    _gst_block_new (blockArgs, blockTemps, bytecodes, depth, _gst_literals);

  POP_N_OOPS (5);
  OOP_CLASS(block) = STACKTOP ();

  _gst_primitives_executed++;
  SET_STACKTOP (block);
  PRIM_SUCCEEDED;
}

/* CompiledMethod literals:numArgs:numTemps:attributes:bytecodes:depth: */
intptr_t
VMpr_CompiledMethod_create (int id,
                            volatile int numArgs)
#line 2228 "prims.def"
{
  int depth = TO_INT (STACK_AT (0));
  OOP bytecodesOOP = STACK_AT (1);
  OOP attributesOOP = STACK_AT (2);
  int methodTemps = TO_INT (STACK_AT (3));
  int methodArgs = TO_INT (STACK_AT (4));
  OOP literals = STACK_AT (5);
  bc_vector bytecodes = _gst_extract_bytecodes (bytecodesOOP);
  int primitive = _gst_process_attributes_array (attributesOOP);
  OOP method;
  if (primitive == -1)
    PRIM_FAILED;

  method = _gst_make_new_method (primitive, methodArgs, methodTemps, depth,
			         literals, bytecodes, _gst_nil_oop,
				 _gst_nil_oop, _gst_nil_oop, -1, -1);

  POP_N_OOPS(6);
  OOP_CLASS(method) = STACKTOP ();

  _gst_primitives_executed++;
  SET_STACKTOP (method);
  PRIM_SUCCEEDED;
}

/* Object shallowCopy */
intptr_t
VMpr_Object_shallowCopy (int id,
                         volatile int numArgs)
#line 2255 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  oop1 = _gst_object_copy (oop1);
  SET_STACKTOP (oop1);
  PRIM_SUCCEEDED;
}

/* Behavior basicNew; Behavior new; */
intptr_t
VMpr_Behavior_basicNew (int id,
                        volatile int numArgs)
#line 2267 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if COMMON (RECEIVER_IS_OOP (oop1))
    {
      if COMMON (!CLASS_IS_INDEXABLE (oop1))
	{
	  /* Note: you cannot pass &STACKTOP() because if the stack
	     moves it ain't valid anymore by the time it is set!!! */
	  OOP result;
	  instantiate (oop1, &result);
	  SET_STACKTOP (result);
	  PRIM_SUCCEEDED;
	}
    }
  PRIM_FAILED;
}

/* Behavior new:; Behavior basicNew: */
intptr_t
VMpr_Behavior_basicNewColon (int id,
                             volatile int numArgs)
#line 2289 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (RECEIVER_IS_OOP (oop1) && IS_INT (oop2))
    {
      if COMMON (CLASS_IS_INDEXABLE (oop1))
	{
	  intptr_t arg2;
	  arg2 = TO_INT (oop2);
	  if (arg2 >= 0)
	    {
	      OOP result;
	      instantiate_with (oop1, arg2, &result);
	      SET_STACKTOP (result);
	      PRIM_SUCCEEDED;
	    }
	}
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* Object become: */
intptr_t
VMpr_Object_become (int id,
                    volatile int numArgs)
#line 2318 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (!IS_OOP_READONLY (oop1)
	     && !IS_OOP_READONLY (oop2))
    {
      _gst_swap_objects (oop1, oop2);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* Object instVarAt: */
intptr_t
VMpr_Object_instVarAt (int id,
                       volatile int numArgs)
#line 2338 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if COMMON (CHECK_BOUNDS_OF (oop1, arg2))
	{
	  SET_STACKTOP (inst_var_at (oop1, arg2));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* Object inst_var_at:put: */
intptr_t
VMpr_Object_instVarAtPut (int id,
                          volatile int numArgs)
#line 2361 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if COMMON (CHECK_BOUNDS_OF (oop1, arg2))
	{
	  inst_var_at_put (oop1, arg2, oop3);
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* Object asOop; Object hash; Symbol hash */
intptr_t
VMpr_Object_hash (int id,
                  volatile int numArgs)
#line 2386 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if COMMON (IS_OOP (oop1))
    {
      PUSH_INT (OOP_INDEX (oop1));
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* SmallInteger asObject; SmallInteger asObjectNoFail */
intptr_t
VMpr_SmallInteger_asObject (int id,
                            volatile int numArgs)
#line 2402 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  arg1 = TO_INT (oop1);
  if COMMON (OOP_INDEX_VALID (arg1))
    {
      oop1 = OOP_AT (arg1);
      if (!IS_OOP_VALID (oop1))
	oop1 = _gst_nil_oop;

      SET_STACKTOP (oop1);
      PRIM_SUCCEEDED;
    }

  PRIM_FAILED;
}

/* SmallInteger nextValidOop */
intptr_t
VMpr_SmallInteger_nextValidOop (int id,
                                volatile int numArgs)
#line 2424 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  arg1 = TO_INT (oop1);
  while (OOP_INDEX_VALID (++arg1))
    {
      oop1 = OOP_AT (arg1);
      if (IS_OOP_VALID (oop1))
	{
          SET_STACKTOP_INT (arg1);
          PRIM_SUCCEEDED;
	}
    }

  PRIM_FAILED;
}

/* Behavior someInstance */
intptr_t
VMpr_Behavior_someInstance (int id,
                            volatile int numArgs)
#line 2446 "prims.def"
{
  OOP oop1;
  OOP oop2, lastOOP;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  PREFETCH_START (_gst_mem.ot_base, PREF_READ | PREF_NTA);
  for (oop2 = _gst_mem.ot_base, lastOOP = &_gst_mem.ot[_gst_mem.ot_size];
       oop2 < lastOOP; oop2++)
    {
      PREFETCH_LOOP (oop2, PREF_READ | PREF_NTA);
      if UNCOMMON (IS_OOP_VALID (oop2) && oop1 == OOP_CLASS (oop2))
	{
	  SET_STACKTOP (oop2);
	  PRIM_SUCCEEDED;
	}
    }
  PRIM_FAILED;
}

/* Object nextInstance */
intptr_t
VMpr_Object_nextInstance (int id,
                          volatile int numArgs)
#line 2468 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (OOP_CLASS (oop1) == _gst_char_class)
    {
      /* Characters are one after another - at the end there is
         _gst_nil_oop */
      oop1++;
      if (_gst_char_class == OOP_CLASS (oop1))
	{
	  SET_STACKTOP (oop1);
	  PRIM_SUCCEEDED;
	}
    }
  else if (IS_OOP (oop1) && oop1 >= _gst_mem.ot)
    {
      /* There is just one copy of all other builtin objects, so fail
         for a builtin */
      OOP class_oop = OOP_CLASS (oop1);
      for (++oop1; oop1 <= _gst_mem.last_allocated_oop; oop1++)
	{
          PREFETCH_LOOP (oop1, PREF_READ | PREF_NTA);
	  if (IS_OOP_VALID (oop1) && class_oop == OOP_CLASS (oop1))
	    {
	      SET_STACKTOP (oop1);
	      PRIM_SUCCEEDED;
	    }
	}
    }
  PRIM_FAILED;
}

/* Object becomeForward: */
intptr_t
VMpr_Object_becomeForward (int id,
                           volatile int numArgs)
#line 2504 "prims.def"
{
  OOP oop1, ownerOOP;
  OOP oop2, lastOOP;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();

  if COMMON (IS_INT (oop1) || IS_OOP_READONLY (oop1))
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  /* Search also on LIFO contexts.  */
  empty_context_stack ();
  PREFETCH_START (_gst_mem.ot_base, PREF_READ | PREF_NTA);
  for (ownerOOP = _gst_mem.ot_base, lastOOP = &_gst_mem.ot[_gst_mem.ot_size];
       ownerOOP < lastOOP; ownerOOP++)
    {
      gst_object object;
      OOP *scanPtr;
      int n;

      PREFETCH_LOOP (ownerOOP, PREF_READ | PREF_NTA);
      if COMMON (!IS_OOP_VALID (ownerOOP))
	continue;

      object = OOP_TO_OBJ (ownerOOP);
      n = num_valid_oops (ownerOOP);
      if UNCOMMON (object->objClass == oop1)
        object->objClass = oop2;
      for (scanPtr = object->data; n--; scanPtr++)
	if UNCOMMON (*scanPtr == oop1)
	  *scanPtr = oop2;
    }

  /* The above loop changed the reference to oop1 in the stacktop,
     so we have to set it back manually!  */
  SET_STACKTOP (oop1);
  PRIM_SUCCEEDED;
}

/* Object allOwners */
intptr_t
VMpr_Object_allOwners (int id,
                       volatile int numArgs)
#line 2549 "prims.def"
{
  OOP oop1;
  OOP oop2, lastOOP;
  OOP result;
  gst_object object;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();

  _gst_reset_buffer ();
  PREFETCH_START (_gst_mem.ot_base, PREF_READ | PREF_NTA);
  for (oop2 = _gst_mem.ot_base, lastOOP = &_gst_mem.ot[_gst_mem.ot_size];
       oop2 < lastOOP; oop2++)
    {
      PREFETCH_LOOP (oop2, PREF_READ | PREF_NTA);
      if UNCOMMON (IS_OOP_VALID (oop2) && is_owner(oop2, oop1))
	_gst_add_buf_pointer (oop2);
    }

  object = new_instance_with (_gst_array_class,
			      _gst_buffer_size() / sizeof (PTR),
			      &result);

  _gst_copy_buffer (object->data);
  SET_STACKTOP (result);
  PRIM_SUCCEEDED;
}


intptr_t
VMpr_ContextPart_thisContext (int id,
                              volatile int numArgs)
#line 2579 "prims.def"
{
  _gst_primitives_executed++;
  empty_context_stack ();
  SET_STACKTOP (_gst_this_context_oop);
  PRIM_SUCCEEDED;
}

intptr_t
VMpr_ContextPart_continue (int id,
                           volatile int numArgs)
#line 2587 "prims.def"
{
  OOP oop2;
  OOP oop1;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (RECEIVER_IS_A_KIND_OF (OOP_CLASS (oop1),
				    _gst_context_part_class))
    {
      unwind_to (oop1);
      SET_STACKTOP (oop2);
      PRIM_SUCCEEDED_RELOAD_IP;
    }
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }
}

/* Continuation resume:nextContinuation: */
intptr_t
VMpr_Continuation_resume (int id,
                          volatile int numArgs)
#line 2610 "prims.def"
{
  OOP oop1, oop2, oop3;

  _gst_primitives_executed++;
  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if COMMON (RECEIVER_IS_A_KIND_OF (OOP_CLASS (oop1), _gst_continuation_class))
    {
      gst_continuation cc = (gst_continuation) OOP_TO_OBJ (oop1);
      if (COMMON (!IS_NIL (cc->stack)))
	{
	  resume_suspended_context (cc->stack);
	  cc->stack = oop3;
	  SET_STACKTOP (oop2);
	  PRIM_SUCCEEDED_RELOAD_IP;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* BlockClosure value
   BlockClosure value:
   BlockClosure value:value:
   BlockClosure value:value:value: */
intptr_t
VMpr_BlockClosure_value (int id,
                         volatile int numArgs)
#line 2638 "prims.def"
{
  _gst_primitives_executed++;
  if UNCOMMON (send_block_value (numArgs, 0))
    PRIM_FAILED;
  else
    PRIM_SUCCEEDED_RELOAD_IP;
}

/* BlockClosure cull:
   BlockClosure cull:cull:
   BlockClosure cull:cull:cull: */
intptr_t
VMpr_BlockClosure_cull (int id,
                        volatile int numArgs)
#line 2650 "prims.def"
{
  _gst_primitives_executed++;
  if UNCOMMON (send_block_value (numArgs, numArgs))
    PRIM_FAILED;
  else
    PRIM_SUCCEEDED_RELOAD_IP;
}

/* We cannot cache the IP here, otherwise calls to #valueAndResumeOnUnwind
   don't actually make the context an unwind context.  If we make a provision
   for that in xlat.c, we can set the cache_new_ip attribute.  */
intptr_t
VMpr_BlockClosure_valueAndResumeOnUnwind (int id,
                                          volatile int numArgs)
#line 2662 "prims.def"
{
  gst_method_context context;

  _gst_primitives_executed++;

  context = (gst_method_context) OOP_TO_OBJ (_gst_this_context_oop);
  context->flags |= MCF_IS_UNWIND_CONTEXT;
  if UNCOMMON (send_block_value (numArgs, 0))
    PRIM_FAILED;
  else
    PRIM_SUCCEEDED_RELOAD_IP;

}

/* BlockClosure valueWithArguments: */
intptr_t
VMpr_BlockClosure_valueWithArguments (int id,
                                      volatile int numArgs)
#line 2678 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_CLASS (oop2, _gst_array_class))
    {
      int i;
      numArgs = NUM_INDEXABLE_FIELDS (oop2);
      for (i = 1; i <= numArgs; i++)
	PUSH_OOP (ARRAY_AT (oop2, i));

      if UNCOMMON (send_block_value (numArgs, 0))
	{
	  POP_N_OOPS (numArgs);
	  PUSH_OOP (oop2);
	  PRIM_FAILED;
	}
      else
	PRIM_SUCCEEDED_RELOAD_IP;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* Object perform:
   Object perform:with:
   Object perform:with:with:
   Object perform:with:with:with: */
intptr_t
VMpr_Object_perform (int id,
                     volatile int numArgs)
#line 2710 "prims.def"
{
  OOP oop1;
  OOP *oopVec = alloca (numArgs * sizeof (OOP));
  int i;
  _gst_primitives_executed++;

  /* pop off the arguments (if any) */
  numArgs--;
  for (i = 0; i < numArgs; i++)
    oopVec[i] = POP_OOP ();

  oop1 = POP_OOP ();		/* the selector */
  if COMMON (IS_CLASS (oop1, _gst_symbol_class)
             && check_send_correctness (STACKTOP (), oop1, numArgs))
    {

      /* push the args back onto the stack */
      while (--i >= 0)
	PUSH_OOP (oopVec[i]);

      SEND_MESSAGE (oop1, numArgs);
      PRIM_SUCCEEDED_RELOAD_IP;
    }

  if COMMON (is_a_kind_of (OOP_CLASS (oop1), _gst_compiled_method_class))
    {
      gst_compiled_method method;
      method_header header;

      method = (gst_compiled_method) OOP_TO_OBJ (oop1);
      header = method->header;
      if (header.numArgs == numArgs)
	{
	  /* push the args back onto the stack */
	  while (--i >= 0)
	    PUSH_OOP (oopVec[i]);

	  _gst_send_method (oop1);
	  PRIM_SUCCEEDED_RELOAD_IP;
	}
    }

  UNPOP (numArgs + 1);
  PRIM_FAILED;
}

/* Object perform:withArguments: */
intptr_t
VMpr_Object_performWithArguments (int id,
                                  volatile int numArgs)
#line 2758 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  numArgs = NUM_INDEXABLE_FIELDS (oop2);
  if UNCOMMON (!IS_CLASS (oop2, _gst_array_class))
    /* fall through to UNPOP and PRIM_FAILED */
    ;

  else if COMMON (IS_CLASS (oop1, _gst_symbol_class)
      && check_send_correctness (STACKTOP (), oop1, numArgs))
    {

      int i;
      for (i = 1; i <= numArgs; i++)
	PUSH_OOP (ARRAY_AT (oop2, i));

      SEND_MESSAGE (oop1, numArgs);
      PRIM_SUCCEEDED_RELOAD_IP;
    }

  else if COMMON (is_a_kind_of (OOP_CLASS (oop1), _gst_compiled_method_class))
    {
      gst_compiled_method method;
      method_header header;

      method = (gst_compiled_method) OOP_TO_OBJ (oop1);
      header = method->header;
      if COMMON (header.numArgs == numArgs)
	{
	  int i;
	  for (i = 1; i <= numArgs; i++)
	    PUSH_OOP (ARRAY_AT (oop2, i));

	  _gst_send_method (oop1);
	  PRIM_SUCCEEDED_RELOAD_IP;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* Semaphore notifyAll */

intptr_t
VMpr_Semaphore_notifyAll (int id,
                          volatile int numArgs)
#line 2807 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  while (_gst_sync_signal (oop1, false))
    ;

  PRIM_SUCCEEDED;
}

/* Semaphore signal (id = 0) or Semaphore notify (id = 1) */
intptr_t
VMpr_Semaphore_signalNotify (int id,
                             volatile int numArgs)
#line 2822 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  _gst_sync_signal (oop1, id == 0);
  PRIM_SUCCEEDED;
}

/* Semaphore wait lock */
intptr_t
VMpr_Semaphore_lock (int id,
                     volatile int numArgs)
#line 2833 "prims.def"
{
  OOP oop1;
  gst_semaphore sem;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  sem = (gst_semaphore) OOP_TO_OBJ (oop1);
  SET_STACKTOP_BOOLEAN (TO_INT (sem->signals) > 0);
  sem->signals = FROM_INT (0);
  PRIM_SUCCEEDED;
}

/* Semaphore wait */
intptr_t
VMpr_Semaphore_wait (int id,
                     volatile int numArgs)
#line 2847 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  _gst_sync_wait (oop1);
  PRIM_SUCCEEDED;
}

/* Semaphore waitAfterSignalling: aSemaphore */
intptr_t
VMpr_Semaphore_waitAfterSignalling (int id,
                                    volatile int numArgs)
#line 2858 "prims.def"
{
  OOP oop1, oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  _gst_sync_signal (oop2, true);
  _gst_sync_wait (oop1);
  PRIM_SUCCEEDED;
}

/* Process suspend */
intptr_t
VMpr_Process_suspend (int id,
                      volatile int numArgs)
#line 2871 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  suspend_process (oop1);
  PRIM_SUCCEEDED;
}

/* Process resume */
intptr_t
VMpr_Process_resume (int id,
                     volatile int numArgs)
#line 2882 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (resume_process (oop1, false))
    PRIM_SUCCEEDED;
  else
    PRIM_FAILED;
}

/* Process singleStepWaitingOn: */
intptr_t
VMpr_Process_singleStepWaitingOn (int id,
                                  volatile int numArgs)
#line 2895 "prims.def"
{
  OOP oop1;
  OOP oop2;

  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();

  if (is_process_ready (oop1) || is_process_terminating (oop1))
    {
      UNPOP (2);
      PRIM_FAILED;
    }

  /* Put the current process to sleep, switch execution to the
     new one, and set up the VM to signal the semaphore as soon
     as possible.  */
  _gst_sync_wait (oop2);
  resume_process (oop1, true);
  single_step_semaphore = oop2;
  PRIM_SUCCEEDED;
}

/* Process yield */
intptr_t
VMpr_Process_yield (int id,
                    volatile int numArgs)
#line 2921 "prims.def"
{
  OOP oop1;

  _gst_primitives_executed++;
  oop1 = STACKTOP ();
  if (oop1 == get_active_process ())
    {
      SET_STACKTOP (_gst_nil_oop);	/* this is our return value */
      active_process_yield ();
    }
  PRIM_SUCCEEDED;
}

/* Processor pause: waitForSignal */
intptr_t
VMpr_Processor_pause (int id,
                      volatile int numArgs)
#line 2936 "prims.def"
{
  OOP oop1;

  _gst_primitives_executed++;
  oop1 = POP_OOP ();
  if (would_reschedule_process ())
    {
      if (oop1 == _gst_true_oop)
	_gst_pause ();
      else
	_gst_usleep (20000);
    }

  active_process_yield ();
  PRIM_SUCCEEDED;
}



/* Behavior flushCache */
intptr_t
VMpr_Behavior_flushCache (int id,
                          volatile int numArgs)
#line 2957 "prims.def"
{
  _gst_primitives_executed++;
  _gst_invalidate_method_cache ();
  PRIM_SUCCEEDED;
}

/* CompiledCode discardTranslation */
intptr_t
VMpr_CompiledCode_discardTranslation (int id,
                                      volatile int numArgs)
#line 2965 "prims.def"
{
  _gst_primitives_executed++;
#ifdef ENABLE_JIT_TRANSLATION
  _gst_discard_native_code (STACKTOP ());
#endif
  PRIM_SUCCEEDED;
}

/* Object changeClassTo: */
intptr_t
VMpr_Object_changeClassTo (int id,
                           volatile int numArgs)
#line 2975 "prims.def"
{
  OOP oop1, oop2;
  gst_object obj1, obj2;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  oop2 = STACKTOP ();
  obj1 = OOP_TO_OBJ (oop1);
  obj2 = OOP_TO_OBJ (oop2);
  if (NUM_WORDS (obj1) > 0
      && !IS_INT (obj1->data[0])
      && (IS_NIL (obj1->data[0])
	  || is_a_kind_of (OOP_CLASS (obj1->data[0]), _gst_behavior_class)))
    {
      obj2->objClass = oop1;
      PRIM_SUCCEEDED;
    }
  UNPOP (1);			/* trying to do Bad Things */
  PRIM_FAILED;
}

/* Time class timezoneBias */
intptr_t
VMpr_Time_timezoneBias (int id,
                        volatile int numArgs)
#line 2998 "prims.def"
{
  OOP oop1;
  uint64_t t;

  _gst_primitives_executed++;
  if (numArgs == 1)
    {
      if (!is_c_int_64 (STACKTOP ()))
        PRIM_FAILED;

      oop1 = POP_OOP ();
      /* 25202 = days between 1901 and 1970 */
      t = to_c_int_64 (oop1) - (int64_t)86400 * 25202;
      t = _gst_adjust_time_zone (t) - t;
      SET_STACKTOP_INT (t);
    }
  else
    SET_STACKTOP_INT (_gst_current_time_zone_bias ());

  PRIM_SUCCEEDED;
}

/* Time class timezone */
intptr_t
VMpr_Time_timezone (int id,
                    volatile int numArgs)
#line 3022 "prims.def"
{
  OOP oop1;
  char *result;
  _gst_primitives_executed++;

  result = _gst_current_time_zone_name ();
  oop1 = _gst_string_new (result);
  SET_STACKTOP (oop1);
  xfree (result);
  PRIM_SUCCEEDED;
}

/* Time class secondClock
    -- note: this primitive has different
       semantics from those defined in the
       book.  This primitive returns the
       seconds since/to Jan 1, 2000 00:00:00
       instead of Jan 1,1901.  */
intptr_t
VMpr_Time_secondClock (int id,
                       volatile int numArgs)
#line 3041 "prims.def"
{
  _gst_primitives_executed++;
  /* 10957 = days between 1970 and 2000 */
  SET_STACKTOP_INT (_gst_get_time () - 86400 * 10957);
  PRIM_SUCCEEDED;
}

/* Time class nanosecondClock.  */
intptr_t
VMpr_Time_nanosecondClock (int id,
                           volatile int numArgs)
#line 3050 "prims.def"
{
  OOP nsOOP;
  uint64_t ns;
  _gst_primitives_executed++;
  ns = _gst_get_ns_time ();
  nsOOP = from_c_int_64 (ns);
  SET_STACKTOP (nsOOP);
  PRIM_SUCCEEDED;
}

/* Time class millisecondClock.  */
intptr_t
VMpr_Time_millisecondClock (int id,
                            volatile int numArgs)
#line 3062 "prims.def"
{
  OOP milliOOP;
  uint64_t milli;
  _gst_primitives_executed++;
  milli = _gst_get_milli_time ();
  milliOOP = from_c_int_64 (milli);
  SET_STACKTOP (milliOOP);
  PRIM_SUCCEEDED;
}

/* Processor signal: semaphore atMilliseconds: deltaMilliseconds
   Processor signal: semaphore atNanosecondClockValue: absNanoseconds
 */
intptr_t
VMpr_Processor_signalAt (int id,
                         volatile int numArgs)
#line 3078 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (is_c_int_64 (oop2))
    {
      int64_t arg2 = to_c_int_64 (oop2);
      uint64_t ns = _gst_get_ns_time ();
      if (id == 0)
        arg2 = (arg2 * 1000000) + ns;
      if (arg2 <= ns)
	_gst_sync_signal (oop1, true);
      else
	_gst_async_timed_wait (oop1, arg2);
      PRIM_SUCCEEDED;
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* Processor isTimeoutProgrammed */
intptr_t
VMpr_Processor_isTimeoutProgrammed (int id,
                                    volatile int numArgs)
#line 3104 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP_BOOLEAN (_gst_is_timeout_programmed ());
  PRIM_SUCCEEDED;
}

/* String similarityTo: */

intptr_t
VMpr_String_similarityTo (int id,
                          volatile int numArgs)
#line 3113 "prims.def"
{
  int result, l1, l2;
  gst_uchar *s1, *s2;
  OOP oop1, oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();

  if (IS_INT(oop2)
      || OOP_FIXED_FIELDS (oop2)
      || (OOP_INSTANCE_SPEC (oop2) & ISP_INDEXEDVARS) != GST_ISP_CHARACTER)
    PRIM_FAILED;

#ifndef OPTIMIZE
  if (IS_INT(oop1)
      || OOP_FIXED_FIELDS (oop1)
      || (OOP_INSTANCE_SPEC (oop1) & ISP_INDEXEDVARS) != GST_ISP_CHARACTER)
    PRIM_FAILED;
#endif

  s1 = STRING_OOP_CHARS (oop1);
  s2 = STRING_OOP_CHARS (oop2);
  l1 = NUM_INDEXABLE_FIELDS (oop1);
  l2 = NUM_INDEXABLE_FIELDS (oop2);

  /* Weights are: substitution, case change, insert, delete.  Invert
     the sign so that differences are reported as negative numbers.  */
  result = -strnspell (s1, s2, l1, l2, 7, 3, 4, 4);
  SET_STACKTOP_INT (result);
  PRIM_SUCCEEDED;
}

/* String hash */

intptr_t
VMpr_String_hash (int id,
                  volatile int numArgs)
#line 3149 "prims.def"
{
  uintptr_t hash;
  gst_uchar *base;
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
#ifndef OPTIMIZE
  if (!CLASS_IS_SCALAR (OOP_CLASS (oop1)))
    PRIM_FAILED;
#endif

  base = STRING_OOP_CHARS (oop1);
  hash = _gst_hash_string (base, NUM_INDEXABLE_FIELDS (oop1));
  SET_STACKTOP_INT (hash);
  PRIM_SUCCEEDED;
}

/* LargeInteger =
   ByteArray =
   String =
   Array = */
intptr_t
VMpr_ArrayedCollection_equal (int id,
                              volatile int numArgs)
#line 3172 "prims.def"
{
  OOP srcOOP, dstOOP;
  int dstLen, srcLen;
  gst_uchar *dstBase, *srcBase;
  _gst_primitives_executed++;

  srcOOP = POP_OOP ();
  dstOOP = STACKTOP ();
  if COMMON (OOP_INT_CLASS (srcOOP) == OOP_INT_CLASS (dstOOP))
    {
      intptr_t spec = OOP_INSTANCE_SPEC (srcOOP);
      if (spec & (~0 << ISP_NUMFIXEDFIELDS))
	goto bad;

      /* dstEnd is inclusive: (1 to: 1) has length 1 */
      dstLen = NUM_INDEXABLE_FIELDS (dstOOP);
      srcLen = NUM_INDEXABLE_FIELDS (srcOOP);

      if (dstLen != srcLen)
	SET_STACKTOP_BOOLEAN (false);
      else if UNCOMMON (dstLen == 0)
	SET_STACKTOP_BOOLEAN (true);
      else
	{
	  /* do the comparison */
	  dstBase = (gst_uchar *) OOP_TO_OBJ (dstOOP)->data;
	  srcBase = (gst_uchar *) OOP_TO_OBJ (srcOOP)->data;
	  dstLen <<= _gst_log2_sizes[spec & ISP_SHAPE];
	  SET_STACKTOP_BOOLEAN (!memcmp (dstBase, srcBase, dstLen));
	}
      PRIM_SUCCEEDED;
    }

 bad:
  UNPOP (1);
  PRIM_FAILED;
}

/* ByteArray indexOf:startingAt:
   ByteArray indexOf:startingAt:ifAbsent:
   String indexOf:startingAt:
   String indexOf:startingAt:ifAbsent: */
intptr_t
VMpr_ArrayedCollection_indexOfStartingAt (int id,
                                          volatile int numArgs)
#line 3215 "prims.def"
{
  OOP srcIndexOOP, srcOOP, targetOOP;
  int srcIndex, srcLen, target;
  gst_uchar *srcBase, *srcTarget;
  _gst_primitives_executed++;

  POP_N_OOPS (numArgs - 2);
  srcIndexOOP = POP_OOP ();
  targetOOP = POP_OOP ();
  srcOOP = STACKTOP ();
  if COMMON ((IS_INT (targetOOP) || OOP_CLASS (targetOOP) == _gst_char_class)
	     && IS_INT (srcIndexOOP) && !IS_INT (srcOOP))
    {
      intptr_t srcSpec = OOP_INSTANCE_SPEC (srcOOP);
      if (srcSpec & (~0 << ISP_NUMFIXEDFIELDS))
	goto bad;

      /* Check compatibility.  */
      if (_gst_log2_sizes[srcSpec & ISP_SHAPE] != 0)
	goto bad;

      target = IS_INT (targetOOP) ? TO_INT (targetOOP) : CHAR_OOP_VALUE (targetOOP);
      srcIndex = TO_INT (srcIndexOOP);
      srcLen = NUM_INDEXABLE_FIELDS (srcOOP) - srcIndex + 1;

      if UNCOMMON (srcLen < 0)
	goto bad;

      srcBase = (gst_uchar *) OOP_TO_OBJ (srcOOP)->data;
      srcTarget = memchr (&srcBase[srcIndex - 1], target, srcLen);
      if (!srcTarget)
	goto bad;

      SET_STACKTOP_INT (srcTarget - srcBase + 1);
      PRIM_SUCCEEDED;
    }

 bad:
  UNPOP (numArgs);
  PRIM_FAILED;
}

/* LargeInteger primReplaceFrom:to:with:startingAt
   ByteArray replaceFrom:to:withString:startingAt:
   String replaceFrom:to:withByteArray:startingAt:
   Array replaceFrom:to:with:startingAt:*/
intptr_t
VMpr_ArrayedCollection_replaceFromToWithStartingAt (int id,
                                                    volatile int numArgs)
#line 3262 "prims.def"
{
  OOP srcIndexOOP, srcOOP, dstEndIndexOOP, dstStartIndexOOP, dstOOP;
  int dstEndIndex, dstStartIndex, srcIndex, dstLen, srcLen, dstRangeLen;
  gst_uchar *dstBase, *srcBase;
  _gst_primitives_executed++;

  srcIndexOOP = POP_OOP ();
  srcOOP = POP_OOP ();
  dstEndIndexOOP = POP_OOP ();
  dstStartIndexOOP = POP_OOP ();
  dstOOP = STACKTOP ();
  if COMMON (IS_INT (srcIndexOOP) && IS_INT (dstStartIndexOOP)
             && IS_INT (dstEndIndexOOP) && !IS_INT (srcOOP))
    {
      uintptr_t srcSpec = OOP_INSTANCE_SPEC (srcOOP);
      uintptr_t dstSpec = OOP_INSTANCE_SPEC (dstOOP);
      int srcOffset = srcSpec >> ISP_NUMFIXEDFIELDS;
      int dstOffset = dstSpec >> ISP_NUMFIXEDFIELDS;
      int size;

      /* Assume the receiver knows what it is doing for collections that
         are not simple arrays.  Typically the primitive will not be
         exposed to the user in that case.  Instead, be strict when
         dstOffset == 0.  */
      if (srcOffset && !dstOffset)
        goto bad;

      /* Check compatibility.  */
      size = _gst_log2_sizes[srcSpec & ISP_SHAPE];
      if (size != _gst_log2_sizes[dstSpec & ISP_SHAPE])
	goto bad;
      if (((srcSpec & ISP_INDEXEDVARS) == GST_ISP_POINTER)
          != ((dstSpec & ISP_INDEXEDVARS) == GST_ISP_POINTER))
	goto bad;

      /* dstEnd is inclusive: (1 to: 1) has length 1 */
      dstEndIndex = TO_INT (dstEndIndexOOP);
      dstStartIndex = TO_INT (dstStartIndexOOP);
      srcIndex = TO_INT (srcIndexOOP);
      dstOOP = STACKTOP ();
      dstLen = NUM_INDEXABLE_FIELDS (dstOOP);
      srcLen = NUM_INDEXABLE_FIELDS (srcOOP);
      dstRangeLen = dstEndIndex - dstStartIndex + 1;

      if UNCOMMON (dstRangeLen < 0
		   || dstEndIndex > dstLen || dstStartIndex <= 0
	           || srcIndex + dstRangeLen - 1 > srcLen
		   || (srcIndex <= 0 && dstRangeLen > 0))
	goto bad;

      /* don't do it unless there's something to copy */
      if COMMON (dstRangeLen > 0)
	{
	  /* do the copy */
	  dstBase = (gst_uchar *) &(OOP_TO_OBJ (dstOOP)->data[dstOffset]);
	  srcBase = (gst_uchar *) &(OOP_TO_OBJ (srcOOP)->data[srcOffset]);
	  dstStartIndex = (dstStartIndex - 1) << size;
	  srcIndex = (srcIndex - 1) << size;
	  dstRangeLen <<= size;
	  memmove (&dstBase[dstStartIndex], &srcBase[srcIndex], dstRangeLen);
	}
      PRIM_SUCCEEDED;
    }

 bad:
  UNPOP (4);
  PRIM_FAILED;
}

/* Object == */

intptr_t
VMpr_Object_identity (int id,
                      volatile int numArgs)
#line 3334 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  PUSH_BOOLEAN (oop1 == oop2);
  PRIM_SUCCEEDED;
}

/* Object class */
intptr_t
VMpr_Object_class (int id,
                   volatile int numArgs)
#line 3347 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_INT (oop1))
    PUSH_OOP (_gst_small_integer_class);
  else
    PUSH_OOP (OOP_CLASS (oop1));

  PRIM_SUCCEEDED;
}


/* ------- GNU Smalltalk specific primitives begin here -------------------- */
/* quit: status
   Always fail because if it succeeds we don't return */
intptr_t
VMpr_ObjectMemory_quit (int id,
                        volatile int numArgs)
#line 3365 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_INT (oop1))
    {
      suspend_process (get_scheduled_process ());
      fflush (stdout);
      fflush (stderr);
      _gst_invoke_hook (GST_ABOUT_TO_QUIT);
      arg1 = TO_INT (oop1);
      exit (arg1);
    }
  PRIM_FAILED;
}

/* abort */
intptr_t
VMpr_ObjectMemory_abort (int id,
                         volatile int numArgs)
#line 3385 "prims.def"
{
  _gst_primitives_executed++;
  abort ();
}


/* Dictionary at: */
intptr_t
VMpr_Dictionary_at (int id,
                    volatile int numArgs)
#line 3393 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();

  SET_STACKTOP (dictionary_at (oop1, oop2));
  PRIM_SUCCEEDED;
}

/* This is not defined in terms of #error: in a .st file because some 
   of the required functionality may not be present when it gets
   first invoked, say during the loading of the first kernel files.
   We'll redefine it later.  */

/* Object doesNotUnderstand:
 * Object error: */
intptr_t
VMpr_Object_bootstrapException (int id,
                                volatile int numArgs)
#line 3415 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (id == -1)
    printf ("%O did not understand selector %O\n\n",
      	    oop1, MESSAGE_SELECTOR (oop2));
  else
    printf ("%O error: %#O\n\n", oop1, oop2);

  _gst_show_backtrace (stdout);
  _gst_show_stack_contents ();
  abort ();
}


/* Character class value: */

intptr_t
VMpr_Character_create (int id,
                       volatile int numArgs)
#line 3437 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if (arg2 >= 0 && arg2 <= 255)
	{
	  SET_STACKTOP (CHAR_OOP_AT (arg2));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* UnicodeCharacter class value: */

intptr_t
VMpr_UnicodeCharacter_create (int id,
                              volatile int numArgs)
#line 3461 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if (arg2 >= 0 && arg2 <= 0x10FFFF)
	{
	  SET_STACKTOP (char_new (arg2));
	  PRIM_SUCCEEDED;
	}
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* Character = */
intptr_t
VMpr_Character_equal (int id,
                      volatile int numArgs)
#line 3484 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  PUSH_BOOLEAN (IS_OOP (oop2) && is_a_kind_of (OOP_CLASS (oop2), _gst_char_class)
		&& CHAR_OOP_VALUE (oop2) == CHAR_OOP_VALUE (oop1)
		&& (CHAR_OOP_VALUE (oop1) <= 127
		    || OOP_CLASS (oop2) == OOP_CLASS (oop1)));
  PRIM_SUCCEEDED;
}

/* Symbol class intern: aString */
intptr_t
VMpr_Symbol_intern (int id,
                    volatile int numArgs)
#line 3500 "prims.def"
{
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = STACKTOP ();		/* keeps this guy referenced while
				   being interned */
  if (IS_CLASS (oop2, _gst_string_class))
    {
      OOP internedString;
      internedString = _gst_intern_string_oop (oop2);
      POP_N_OOPS (1);

      SET_STACKTOP (internedString);
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* Dictionary new */
intptr_t
VMpr_Dictionary_new (int id,
                     volatile int numArgs)
#line 3521 "prims.def"
{
  OOP oop1, dictionaryOOP;
  _gst_primitives_executed++;
  oop1 = STACKTOP();
  dictionaryOOP = _gst_dictionary_new (32);
  dictionaryOOP->object->objClass = oop1;
  SET_STACKTOP (dictionaryOOP);
  PRIM_SUCCEEDED;
}


/* Memory addressOfOOP: oop */
intptr_t
VMpr_Memory_addressOfOOP (int id,
                          volatile int numArgs)
#line 3534 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_OOP (oop2))
    {
      PUSH_OOP (FROM_C_ULONG ((uintptr_t) oop2));
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* Memory addressOf: oop */
intptr_t
VMpr_Memory_addressOf (int id,
                       volatile int numArgs)
#line 3552 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_OOP (oop2))
    {
      PUSH_OOP (FROM_C_ULONG ((uintptr_t) OOP_TO_OBJ (oop2)));
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}


/* SystemDictionary backtrace */

intptr_t
VMpr_SystemDictionary_backtrace (int id,
                                 volatile int numArgs)
#line 3572 "prims.def"
{
  _gst_primitives_executed++;
  _gst_show_backtrace (stdout);
  PRIM_SUCCEEDED;
}

/* SystemDictionary getTraceFlag: anIndex */
intptr_t
VMpr_SystemDictionary_getTraceFlag (int id,
                                    volatile int numArgs)
#line 3580 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_INT (oop2))
    {
      intptr_t arg2;
      int value;
      arg2 = TO_INT (oop2);
      value = _gst_get_var (arg2);
      if (value != -1)
	{
	  oop1 = (value > 1 ? FROM_INT (oop2 ) :
		  (value ? _gst_true_oop : _gst_false_oop));
	  PUSH_OOP (oop1);
	  PRIM_SUCCEEDED;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* SystemDictionary setTraceFlag: anIndex to: aBoolean */
intptr_t
VMpr_SystemDictionary_setTraceFlag (int id,
                                    volatile int numArgs)
#line 3608 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_INT (oop1))
    {
      intptr_t arg1 = TO_INT (oop1);
      intptr_t old_value = _gst_set_var (arg1,
					 IS_INT (oop2)
					 ? TO_INT (oop2)
					 : oop2 == _gst_true_oop);
      if (old_value != -1)
	{
	  SET_EXCEPT_FLAG (true);
	  PRIM_SUCCEEDED;
	}
    }

  UNPOP (2);
  PRIM_FAILED;
}


/* Memory type: aType at: anAddress */
intptr_t
VMpr_Memory_at (int id,
                volatile int numArgs)
#line 3636 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_C_LONG (oop3) && IS_INT (oop2))
    {
      intptr_t arg1, arg2;
      arg1 = TO_INT (oop2);
      arg2 = TO_C_LONG (oop3);
      switch (arg1)
	{
	case CDATA_CHAR:		/* char */
	case CDATA_UCHAR:		/* unsigned char */
	  PUSH_OOP (CHAR_OOP_AT (*(unsigned char *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_SHORT:		/* short */
	  PUSH_INT (*(short *) arg2);
	  PRIM_SUCCEEDED;
	case CDATA_USHORT:		/* unsigned short */
	  PUSH_INT (*(unsigned short *) arg2);
	  PRIM_SUCCEEDED;
	case CDATA_LONG:		/* long */
	  PUSH_OOP (FROM_C_LONG (*(long *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_ULONG:		/* unsigned long */
	  PUSH_OOP (FROM_C_ULONG (*(unsigned long *) arg2));
	  PRIM_SUCCEEDED;
       case CDATA_LONGLONG:            /* long long */
         PUSH_OOP (from_c_int_64 (*(long long *) arg2));
         PRIM_SUCCEEDED;
       case CDATA_ULONGLONG:           /* unsigned long long */
         PUSH_OOP (from_c_uint_64 (*(unsigned long long *) arg2));
         PRIM_SUCCEEDED;
	case CDATA_FLOAT:		/* float */
	  PUSH_OOP (floate_new (*(float *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_DOUBLE:		/* double */
	  PUSH_OOP (floatd_new (*(double *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_STRING:		/* string */
	  if (*(char **) arg2)
	    PUSH_OOP (_gst_string_new (*(char **) arg2));
	  else
	    PUSH_OOP (_gst_nil_oop);

	  PRIM_SUCCEEDED;
	case CDATA_OOP:		/* OOP */
	  PUSH_OOP (*(OOP *) arg2);
	  PRIM_SUCCEEDED;
	case CDATA_INT:		/* int */
	  PUSH_OOP (FROM_C_INT (*(int *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_UINT:		/* unsigned int */
	  PUSH_OOP (FROM_C_UINT (*(unsigned int *) arg2));
	  PRIM_SUCCEEDED;
	case CDATA_LONG_DOUBLE:		/* long double */
	  PUSH_OOP (floatq_new (*(long double *) arg2));
	  PRIM_SUCCEEDED;
	}
    }

  UNPOP (3);
  PRIM_FAILED;
}

/* Memory type: aType at: anAddress put: aValue */
intptr_t
VMpr_Memory_atPut (int id,
                   volatile int numArgs)
#line 3708 "prims.def"
{
  OOP oop4;
  OOP oop3;
  OOP oop2;
  _gst_primitives_executed++;

  oop4 = POP_OOP ();
  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  /* don't pop the receiver */
  if (IS_C_LONG (oop3) && IS_INT (oop2))
    {
      intptr_t arg1, arg2;
      arg1 = TO_INT (oop2);
      arg2 = TO_C_LONG (oop3);
      switch (arg1)
	{
	case CDATA_CHAR:		/* char */
	case CDATA_UCHAR:		/* unsigned char */
	  /* may want to use Character instead? */
	  if (IS_CLASS (oop3, _gst_char_class)
	      || (IS_CLASS (oop3, _gst_unicode_character_class)
		  && CHAR_OOP_VALUE (oop3) <= 127))
	    {
	      *(char *) arg2 = CHAR_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_INT (oop4))
	    {
	      *(char *) arg2 = (char) TO_INT (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_SHORT:		/* short */
	case CDATA_USHORT:		/* unsigned short */
	  if (IS_INT (oop4))
	    {
	      *(short *) arg2 = (short) TO_INT (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_LONG:		/* long */
	case CDATA_ULONG:		/* unsigned long */
	  if (IS_C_LONG (oop4) || IS_C_ULONG (oop4))
	    {
	      *(long *) arg2 = TO_C_LONG (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
       case CDATA_LONGLONG:            /* long long */
       case CDATA_ULONGLONG:           /* unsigned long long */
         if (IS_C_LONGLONG (oop4) || IS_C_ULONGLONG (oop4))
           {
             *(long long *) arg2 = to_c_int_64 (oop4);
             PRIM_SUCCEEDED;
           }
         break;
	case CDATA_FLOAT:		/* float */
	  if (IS_CLASS (oop4, _gst_floatd_class))
	    {
	      *(float *) arg2 = (float) FLOATD_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floate_class))
	    {
	      *(float *) arg2 = FLOATE_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floatq_class))
	    {
	      *(float *) arg2 = (float) FLOATQ_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_DOUBLE:		/* double */
	  if (IS_CLASS (oop4, _gst_floatd_class))
	    {
	      *(double *) arg2 = FLOATD_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floate_class))
	    {
	      *(double *) arg2 = (double) FLOATE_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floatq_class))
	    {
	      *(double *) arg2 = (double) FLOATQ_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_STRING:		/* string */
	  if (IS_CLASS (oop4, _gst_string_class)
	      || IS_CLASS (oop4, _gst_symbol_class))
	    {
	      /* Char* cast on the right side needed because
	         _gst_to_cstring returns gst_uchar * */
	      *(char **) arg2 = (char *) _gst_to_cstring (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_OOP:		/* OOP */
	  *(OOP *) arg2 = oop4;
	  PRIM_SUCCEEDED;
	case CDATA_INT:		/* int */
	case CDATA_UINT:		/* unsigned int */
	  if (IS_C_INT (oop4) || is_c_uint_32 (oop4))
	    {
	      *(int *) arg2 = TO_C_INT (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	case CDATA_LONG_DOUBLE:		/* long double */
	  if (IS_CLASS (oop4, _gst_floatd_class))
	    {
	      *(long double *) arg2 = (long double) FLOATD_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floate_class))
	    {
	      *(long double *) arg2 = (long double) FLOATE_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_CLASS (oop4, _gst_floatq_class))
	    {
	      *(long double *) arg2 = FLOATQ_OOP_VALUE (oop4);
	      PRIM_SUCCEEDED;
	    }
	  break;
	}
    }

  UNPOP (3);
  PRIM_FAILED;
}


/* methodsFor: category */
intptr_t
VMpr_Behavior_methodsFor (int id,
                          volatile int numArgs)
#line 3847 "prims.def"
{
  OOP oop2 = POP_OOP ();
  OOP oop1 = STACKTOP ();

  _gst_primitives_executed++;
  if (!_gst_current_parser || _gst_current_parser->state != PARSE_DOIT)
    PRIM_FAILED;

  _gst_set_compilation_class (oop1);
  _gst_set_compilation_category (oop2);
  _gst_display_compilation_trace ("Compiling", true);
  _gst_current_parser->state = PARSE_METHOD_LIST;
  PRIM_SUCCEEDED;
}

/* methodsFor: category ifTrue: condition */
intptr_t
VMpr_Behavior_methodsForIfTrue (int id,
                                volatile int numArgs)
#line 3864 "prims.def"
{
  OOP oop3 = POP_OOP ();
  OOP oop2 = POP_OOP ();
  OOP oop1 = STACKTOP ();

  _gst_primitives_executed++;
  if (!_gst_current_parser || _gst_current_parser->state != PARSE_DOIT)
    PRIM_FAILED;

  _gst_set_compilation_class (oop1);
  _gst_set_compilation_category (oop2);
  if (oop3 == _gst_true_oop)
    _gst_display_compilation_trace ("Conditionally compiling", true);
  else
    {
      _gst_skip_compilation = true;
      _gst_display_compilation_trace ("Conditionally skipping", true);
    }
  _gst_current_parser->state = PARSE_METHOD_LIST;
  PRIM_SUCCEEDED;
}


intptr_t
VMpr_Processor_disableEnableInterrupts (int id,
                                        volatile int numArgs)
#line 3890 "prims.def"
{
  OOP processOOP;
  gst_process process;
  gst_processor_scheduler processor;
  int count;

  _gst_primitives_executed++;
  processor = (gst_processor_scheduler) OOP_TO_OBJ (_gst_processor_oop);
  processOOP = processor->activeProcess;
  process = (gst_process) OOP_TO_OBJ (processOOP);

  count = IS_NIL (process->interrupts) ? 0 : TO_INT (process->interrupts);

  if (id == 0 && count++ == 0)
    async_queue_enabled = false;
  else if (id == -1 && --count == 0)
    {
      async_queue_enabled = true;
      SET_EXCEPT_FLAG (true);
    }

  process->interrupts = FROM_INT (count);

  PRIM_SUCCEEDED;
}

/* ProcessorScheduler signal: aSemaphore onInterrupt: anInteger */
intptr_t
VMpr_Processor_signalOnInterrupt (int id,
                                  volatile int numArgs)
#line 3918 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_INT (oop2))
    {
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      _gst_async_interrupt_wait (oop1, arg2);
      PRIM_SUCCEEDED;
    }

  UNPOP (2);
  PRIM_FAILED;
}

/* ObjectMemory spaceGrowRate */

intptr_t
VMpr_ObjectMemory_getSpaceGrowRate (int id,
                                    volatile int numArgs)
#line 3940 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP (floatd_new ((double) _gst_mem.space_grow_rate));
  PRIM_SUCCEEDED;
}

/* ObjectMemory spaceGrowRate: */
intptr_t
VMpr_ObjectMemory_setSpaceGrowRate (int id,
                                    volatile int numArgs)
#line 3948 "prims.def"
{
  intptr_t arg1;
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    arg1 = (int) FLOATD_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floate_class))
    arg1 = (int) FLOATE_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floatq_class))
    arg1 = (int) FLOATQ_OOP_VALUE (oop1);
  else if (IS_INT (oop1))
    arg1 = TO_INT (oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (arg1 > 0 && arg1 <= 500)
    {
      _gst_init_mem (0, 0, 0, 0, 0, arg1);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* ObjectMemory smoothingFactor */
intptr_t
VMpr_ObjectMemory_getSmoothingFactor (int id,
                                      volatile int numArgs)
#line 3980 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP (floatd_new ((double) _gst_mem.factor));
  PRIM_SUCCEEDED;
}

/* ObjectMemory smoothingFactor: */
intptr_t
VMpr_ObjectMemory_setSmoothingFactor (int id,
                                      volatile int numArgs)
#line 3988 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    arg1 = (int) FLOATD_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floate_class))
    arg1 = (int) FLOATE_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floatq_class))
    arg1 = (int) FLOATQ_OOP_VALUE (oop1);
  else if (IS_INT (oop1))
    arg1 = TO_INT (oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }
  if (arg1 >= 0 && arg1 <= 1)
    {
      _gst_mem.factor = arg1;
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* ObjectMemory growThresholdPercent */
intptr_t
VMpr_ObjectMemory_getGrowThresholdPercent (int id,
                                           volatile int numArgs)
#line 4019 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP (floatd_new ((double) _gst_mem.grow_threshold_percent));
  PRIM_SUCCEEDED;
}

/* ObjectMemory growThresholdPercent: */
intptr_t
VMpr_ObjectMemory_setGrowThresholdPercent (int id,
                                           volatile int numArgs)
#line 4027 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    arg1 = (int) FLOATD_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floate_class))
    arg1 = (int) FLOATE_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floatq_class))
    arg1 = (int) FLOATQ_OOP_VALUE (oop1);
  else if (IS_INT (oop1))
    arg1 = TO_INT (oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }
  if (arg1 > 0 && arg1 < 100)
    {
      _gst_init_mem (0, 0, 0, 0, arg1, 0);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* ObjectMemory bigObjectThreshold */
intptr_t
VMpr_ObjectMemory_getBigObjectThreshold (int id,
                                         volatile int numArgs)
#line 4058 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP_INT (_gst_mem.big_object_threshold);
  PRIM_SUCCEEDED;
}

/* ObjectMemory bigObjectThreshold: */
intptr_t
VMpr_ObjectMemory_setBigObjectThreshold (int id,
                                         volatile int numArgs)
#line 4066 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_NIL (oop1))
    arg1 = INT_MAX < MAX_ST_INT ? INT_MAX : MAX_ST_INT;
  else if (IS_INT (oop1))
    arg1 = TO_INT (oop1);
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (arg1 >= 0)
    {
      _gst_init_mem (0, 0, 0, arg1, 0, 0);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* ObjectMemory growTo: numBytes */
intptr_t
VMpr_ObjectMemory_growTo (int id,
                          volatile int numArgs)
#line 4094 "prims.def"
{
  OOP oop1;
  intptr_t arg1;
  _gst_primitives_executed++;

  oop1 = POP_OOP ();
  if (IS_INT (oop1))
    {
      arg1 = TO_INT (oop1);
      _gst_grow_memory_to (arg1);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* ObjectMemory update */
intptr_t
VMpr_ObjectMemory_update (int id,
                          volatile int numArgs)
#line 4113 "prims.def"
{
  _gst_primitives_executed++;

#ifndef OPTIMIZE
  if (OOP_CLASS (STACKTOP ()) != _gst_object_memory_class)
    PRIM_FAILED;
#endif

  _gst_update_object_memory_oop (STACKTOP ());
  PRIM_SUCCEEDED;
}

/* CObject class alloc: nbytes type: aType */

intptr_t
VMpr_CObject_allocType (int id,
                        volatile int numArgs)
#line 4128 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop1 = STACK_AT (0);
  oop2 = STACK_AT (1);
  oop3 = STACK_AT (2);
  if (IS_INT (oop2)
      && (IS_NIL (oop1) || is_a_kind_of (OOP_CLASS (oop1), _gst_c_type_class))
      && COMMON (RECEIVER_IS_A_KIND_OF (oop3, _gst_c_object_class)))
    {
      intptr_t arg2 = TO_INT (oop2);
      PTR ptr = xmalloc (arg2);
      OOP cObjectOOP = COBJECT_NEW (ptr, oop1, oop3);

      POP_N_OOPS (2);
      SET_STACKTOP (cObjectOOP);
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* sin */
intptr_t
VMpr_Float_sin (int id,
                volatile int numArgs)
#line 4154 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (sin (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (sin (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (sinl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* cos */
intptr_t
VMpr_Float_cos (int id,
                volatile int numArgs)
#line 4182 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (cos (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (cos (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (cosl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* tan */
intptr_t
VMpr_Float_tan (int id,
                volatile int numArgs)
#line 4210 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (tan (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (tan (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (tanl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* arcSin */
intptr_t
VMpr_Float_arcSin (int id,
                   volatile int numArgs)
#line 4238 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (asin (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (asin (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (asinl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* arcCos */
intptr_t
VMpr_Float_arcCos (int id,
                   volatile int numArgs)
#line 4266 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (acos (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (acos (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (acosl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* arcTan */
intptr_t
VMpr_Float_arcTan (int id,
                   volatile int numArgs)
#line 4294 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (atan (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (atan (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (atanl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* exp */
intptr_t
VMpr_Float_exp (int id,
                volatile int numArgs)
#line 4322 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (exp (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (exp (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (expl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* ln */
intptr_t
VMpr_Float_ln (int id,
               volatile int numArgs)
#line 4350 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (log (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = (double) FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (log (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (logl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* raisedTo: aNumber -- receiver ** aNumber */
intptr_t
VMpr_Float_pow (int id,
                volatile int numArgs)
#line 4378 "prims.def"
{
  OOP oop1;
  OOP oop2;
  double farg1, farg2;
  long double lfarg1, lfarg2;
  mst_Boolean long_double = false;

  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    lfarg1 = farg1 = FLOATD_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floate_class))
    lfarg1 = farg1 = FLOATE_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long_double = true;
      lfarg1 = farg1 = FLOATQ_OOP_VALUE (oop1);
    }
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (IS_CLASS (oop2, _gst_floatd_class))
    lfarg2 = farg2 = FLOATD_OOP_VALUE (oop2);
  else if (IS_CLASS (oop2, _gst_floate_class))
    lfarg2 = farg2 = FLOATE_OOP_VALUE (oop2);
  else if (IS_CLASS (oop2, _gst_floatq_class))
    {
      long_double = true;
      lfarg2 = farg2 = FLOATQ_OOP_VALUE (oop2);
    }
  else
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if ((lfarg1 == 0.0 && lfarg2 < 0.0) || lfarg1 < 0.0)
    {
      UNPOP (1);
      PRIM_FAILED;
    }

  if (long_double)
    {
      if (IS_NAN (lfarg1) || IS_NAN (lfarg2))
        /* The C99 standard mandates that pow(1, NaN) = 1.0 and pow
           (NaN, 0.0) = 1.0, which is plain wrong.  We take the
           liberty to make these results be NaN.  */
        SET_STACKTOP (floatq_new (lfarg1 + lfarg2));
      else
        SET_STACKTOP (floatq_new (powl (lfarg1, lfarg2)));
    }

  else
    {
      if (IS_NAN (farg1) || IS_NAN (farg2))
        /* The C99 standard mandates that pow(1, NaN) = 1.0 and pow
           (NaN, 0.0) = 1.0, which is plain wrong.  We take the
           liberty to make these results be NaN.  */
        SET_STACKTOP (floatd_new (farg1 + farg2));
      else
        SET_STACKTOP (floatd_new (pow (farg1, farg2)));
    }

  PRIM_SUCCEEDED;
}

/* CObject free */
intptr_t
VMpr_CObject_free (int id,
                   volatile int numArgs)
#line 4452 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if COMMON (is_a_kind_of (OOP_CLASS (oop1), _gst_c_callback_descriptor_class))
    {
      _gst_free_closure (oop1);
      SET_STACKTOP (_gst_nil_oop);
      PRIM_SUCCEEDED;
    }

  else if COMMON (RECEIVER_IS_A_KIND_OF (OOP_CLASS (oop1), _gst_c_object_class))
    {
      _gst_free_cobject (oop1);	/* free allocated space */
      SET_STACKTOP (_gst_nil_oop);
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* sqrt */
intptr_t
VMpr_Float_sqrt (int id,
                 volatile int numArgs)
#line 4475 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    {
      double farg1 = FLOATD_OOP_VALUE (oop1);
      SET_STACKTOP (floatd_new (sqrt (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floate_class))
    {
      double farg1 = FLOATE_OOP_VALUE (oop1);
      SET_STACKTOP (floate_new (sqrt (farg1)));
      PRIM_SUCCEEDED;
    }
  else if (IS_CLASS (oop1, _gst_floatq_class))
    {
      long double farg1 = FLOATQ_OOP_VALUE (oop1);
      SET_STACKTOP (floatq_new (sqrtl (farg1)));
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}

/* ceiling, floor */
intptr_t
VMpr_Float_ceil_floor (int id,
                       volatile int numArgs)
#line 4505 "prims.def"
{
  OOP oop1;
  double farg1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_CLASS (oop1, _gst_floatd_class))
    farg1 = FLOATD_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floate_class))
    farg1 = (double) FLOATE_OOP_VALUE (oop1);
  else if (IS_CLASS (oop1, _gst_floatq_class))
    farg1 = (double) FLOATQ_OOP_VALUE (oop1);
  else
    PRIM_FAILED;

  if COMMON ((farg1 > MIN_ST_INT) && farg1 < MAX_ST_INT)
    {
      switch (id)
        {
        case 0:
          SET_STACKTOP_INT ((intptr_t) ceil (farg1));
          PRIM_SUCCEEDED;
        case -1:
          SET_STACKTOP_INT ((intptr_t) floor (farg1));
          PRIM_SUCCEEDED;
        }
    }

  PRIM_FAILED;
}

/* Behavior basicNewInFixedSpace */
intptr_t
VMpr_Behavior_basicNewFixed (int id,
                             volatile int numArgs)
#line 4538 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_OOP (oop1))
    {
      if (!CLASS_IS_INDEXABLE (oop1))
	{
	  OOP result;
	  instantiate (oop1, &result);
	  _gst_make_oop_fixed (result);
	  SET_STACKTOP (result);
	  PRIM_SUCCEEDED;
	}
    }
  PRIM_FAILED;
}

/* Behavior basicNewInFixedSpace: */
intptr_t
VMpr_Behavior_basicNewFixedColon (int id,
                                  volatile int numArgs)
#line 4559 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_OOP (oop1) && IS_INT (oop2))
    {
      if (CLASS_IS_INDEXABLE (oop1))
	{
	  intptr_t arg2;
	  arg2 = TO_INT (oop2);
	  if (arg2 >= 0)
	    {
	      OOP result;
	      instantiate_with (oop1, arg2, &result);
	      _gst_make_oop_fixed (result);
	      SET_STACKTOP (result);
	      PRIM_SUCCEEDED;
	    }
	}
    }

  UNPOP (1);
  PRIM_FAILED;
}

intptr_t
VMpr_Object_tenure (int id,
                    volatile int numArgs)
#line 4588 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_OOP (oop1))
    {
      _gst_tenure_oop (oop1);
      PRIM_SUCCEEDED;
    }

  PRIM_FAILED;
}

intptr_t
VMpr_Object_makeFixed (int id,
                       volatile int numArgs)
#line 4603 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  if (IS_OOP (oop1))
    {
      _gst_make_oop_fixed (oop1);
      PRIM_SUCCEEDED;
    }

  PRIM_FAILED;
}


/* CObject at: byteoffset type: aType */

intptr_t
VMpr_CObject_at (int id,
                 volatile int numArgs)
#line 4623 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_INT (oop2)
      && ((IS_INT (oop3) && id == -1)
	  || is_a_kind_of (OOP_CLASS (oop3), _gst_c_type_class)))
    {
      char *addr;
      intptr_t arg2;
      arg2 = TO_INT (oop2);
      if (IS_INT (oop3))
	{			/* int type spec means a scalar type */
	  intptr_t arg3 = TO_INT (oop3);

	  if (!cobject_index_check (oop1, arg2, _gst_c_type_size (arg3)))
	    goto fail;

	  addr = ((char *) cobject_value (oop1)) + arg2;
	  switch (arg3)
	    {
	    case CDATA_CHAR:
	    case CDATA_UCHAR:
	      SET_STACKTOP (CHAR_OOP_AT (*(gst_uchar *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_SHORT:
	      SET_STACKTOP_INT (*(short *) addr);
	      PRIM_SUCCEEDED;

	    case CDATA_USHORT:
	      SET_STACKTOP_INT (*(unsigned short *) addr);
	      PRIM_SUCCEEDED;

           case CDATA_LONGLONG:
             SET_STACKTOP (from_c_int_64 (*(long long *) addr));
             PRIM_SUCCEEDED;

           case CDATA_ULONGLONG:
             SET_STACKTOP (from_c_uint_64 (*(unsigned long long *) addr));
             PRIM_SUCCEEDED;

	    case CDATA_LONG:
	      SET_STACKTOP (FROM_C_LONG (*(long *) addr));
	      PRIM_SUCCEEDED;
	    case CDATA_ULONG:
	      SET_STACKTOP (FROM_C_ULONG (*(unsigned long *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_FLOAT:
	      SET_STACKTOP (floate_new (*(float *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_DOUBLE:
	      SET_STACKTOP (floatd_new (*(double *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_STRING:
	      {
		char **strAddr;
		strAddr = (char **) addr;
		if (*strAddr)
		  {
		    SET_STACKTOP (_gst_string_new (*strAddr));
		    PRIM_SUCCEEDED;
		  }
		else
		  {
		    SET_STACKTOP (_gst_nil_oop);
		    PRIM_SUCCEEDED;
		  }
	      }
	    case CDATA_OOP:
	      SET_STACKTOP (*(OOP *) addr);
	      PRIM_SUCCEEDED;

	    case CDATA_INT:
	      SET_STACKTOP (FROM_C_INT (*(int *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_UINT:
	      SET_STACKTOP (FROM_C_UINT (*(unsigned int *) addr));
	      PRIM_SUCCEEDED;

	    case CDATA_LONG_DOUBLE:
	      SET_STACKTOP (floatq_new (*(long double *) addr));
	      PRIM_SUCCEEDED;
	    }
	}
      else
	{
	  OOP baseOOP;
	  uintptr_t ofs;
	  inc_ptr incPtr;

	  /* Non-integer oop3: use it as the type of the effective address. */

	  if (id == 0)
	    {
	      if (!cobject_index_check (oop1, arg2, sizeof (uintptr_t)))
		goto fail;

	      ofs = *(uintptr_t *) (((char *)cobject_value (oop1)) + arg2);
	      baseOOP = _gst_nil_oop;
	      if (ofs == 0)
		{
		  SET_STACKTOP (_gst_nil_oop);
		  PRIM_SUCCEEDED;
		}
	    }
          else
	    {
	      /* No need to enforce bounds here (if we ever will, remember
		 that a pointer that is one-past the end of the object is
		 valid!).  */
		 
	      gst_cobject cObj = (gst_cobject) OOP_TO_OBJ (oop1);
	      baseOOP = cObj->storage;
	      ofs = COBJECT_OFFSET_OBJ (cObj) + arg2;
	    }

	  /* oop3 could get GC'ed out of existence before it gets used: it is
	     not on the stack, and _gst_c_object_new_base could cause a GC */
	  incPtr = INC_SAVE_POINTER ();
	  INC_ADD_OOP (baseOOP);
	  INC_ADD_OOP (oop3);
	  SET_STACKTOP (_gst_c_object_new_base (baseOOP, ofs, oop3,
					        _gst_c_object_class));
	  INC_RESTORE_POINTER (incPtr);
	  PRIM_SUCCEEDED;
	}
    }

 fail:
  UNPOP (2);
  PRIM_FAILED;
}

/* CObject at: byteOffset put: aValue type: aType */
intptr_t
VMpr_CObject_atPut (int id,
                    volatile int numArgs)
#line 4768 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  OOP oop4;
  _gst_primitives_executed++;

  oop4 = POP_OOP ();
  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();

  if (IS_INT (oop2) && IS_INT (oop4))
    {
      char *addr;
      intptr_t arg2 = TO_INT (oop2);
      intptr_t arg4 = TO_INT (oop4);
      if (!cobject_index_check (oop1, arg2, _gst_c_type_size (arg4)))
	goto fail;

      addr = ((char *) cobject_value (oop1)) + arg2;
      switch (arg4)
	{
	case CDATA_CHAR:		/* char */
	case CDATA_UCHAR:		/* uchar */
	  if (IS_CLASS (oop3, _gst_char_class)
	      || (IS_CLASS (oop3, _gst_unicode_character_class)
		  && CHAR_OOP_VALUE (oop3) <= 127))
	    {
	      *addr = CHAR_OOP_VALUE (oop3);
	      PRIM_SUCCEEDED;
	    }
	  else if (IS_INT (oop3))
	    {
	      *(char *) addr = (char) TO_INT (oop3);
	      PRIM_SUCCEEDED;
	    }
	  break;

	case CDATA_SHORT:		/* short */
	case CDATA_USHORT:		/* ushort */
	  if (IS_INT (oop3))
	    {
	      *(short *) addr = (short) TO_INT (oop3);
	      PRIM_SUCCEEDED;
	    }
	  break;

	case CDATA_LONG:		/* long */
	case CDATA_ULONG:		/* ulong */
          if (IS_C_LONG (oop3) || IS_C_ULONG (oop3))
	    {
	      *(long *) addr = (long) TO_C_LONG (oop3);
	      PRIM_SUCCEEDED;
	    }
	  break;

       case CDATA_LONGLONG:            /* long long */
       case CDATA_ULONGLONG:           /* unsigned long long */
         if (IS_C_LONGLONG (oop3) || IS_C_ULONGLONG (oop3))
           {
             *(long long *) addr = (long long) to_c_int_64 (oop3);
             PRIM_SUCCEEDED;
           }
         break;

	case CDATA_FLOAT:
	  {
	    float *floatAddr;
	    floatAddr = (float *) addr;
	    if (IS_INT (oop3))
	      {
		*floatAddr = (float) TO_INT (oop3);
		PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatd_class))
	      {
		*floatAddr = (float) FLOATD_OOP_VALUE (oop3);
		PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floate_class))
	      {
		    *floatAddr = (float) FLOATE_OOP_VALUE (oop3);
		    PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatq_class))
	      {
	        *floatAddr = (float) FLOATQ_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	  }
	  break;

	case CDATA_DOUBLE:		/* double */
	  {
	    double *doubleAddr;
	    doubleAddr = (double *) addr;
	    if (IS_INT (oop3))
	      {
	        *doubleAddr = TO_INT (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatd_class))
	      {
	        *doubleAddr = FLOATD_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floate_class))
	      {
	        *doubleAddr = FLOATE_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatq_class))
	      {
	        *doubleAddr = FLOATQ_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	  }
	  break;

	case CDATA_STRING:		/* string */
	  {				/* note that this does not allow for
					   replacemnt in place */
	    /* to replace in place, use replaceFrom: */
	    char **strAddr;
	    strAddr = (char **) addr;
	    if (oop3 == _gst_nil_oop)
	      {
	        *strAddr = (char *) 0;
	        PRIM_SUCCEEDED;
	      }
	    else if (is_a_kind_of (OOP_CLASS (oop3), _gst_string_class))
	      {
		*strAddr = (char *) _gst_to_cstring (oop3);
		PRIM_SUCCEEDED;
	      }
	    break;
	  }

	case CDATA_OOP:
	  *(OOP *) addr = oop3;
	  PRIM_SUCCEEDED;

	case CDATA_INT:		/* int */
	case CDATA_UINT:		/* uint */
	  if (IS_C_INT (oop3))
	    {
	      *(int *) addr = (int) TO_C_INT (oop3);
	      PRIM_SUCCEEDED;
	    }
	  break;

	case CDATA_LONG_DOUBLE:	/* long double */
	  {
	    long double *longDoubleAddr;
	    longDoubleAddr = (long double *) addr;
	    if (IS_INT (oop3))
	      {
	        *longDoubleAddr = TO_INT (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatd_class))
	      {
	        *longDoubleAddr = FLOATD_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floate_class))
	      {
	        *longDoubleAddr = FLOATE_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	    else if (IS_CLASS (oop3, _gst_floatq_class))
	      {
	        *longDoubleAddr = FLOATQ_OOP_VALUE (oop3);
	        PRIM_SUCCEEDED;
	      }
	  }
	  break;
	}
    }

 fail:
  UNPOP (3);
  PRIM_FAILED;
}

/* CObject address */
intptr_t
VMpr_CObject_address (int id,
                      volatile int numArgs)
#line 4956 "prims.def"
{
  OOP oop1;
  gst_cobject cObj;
  uintptr_t ptr;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  cObj = (gst_cobject) OOP_TO_OBJ (oop1);
  ptr = (uintptr_t) COBJECT_OFFSET_OBJ (cObj);

  if (IS_NIL (cObj->storage))
    SET_STACKTOP (FROM_C_ULONG (ptr));
  else
    SET_STACKTOP (FROM_C_LONG (ptr));

  PRIM_SUCCEEDED;
}


/* CObject address: */
intptr_t
VMpr_CObject_addressColon (int id,
                           volatile int numArgs)
#line 4977 "prims.def"
{
  OOP oop1, oop2;
  gst_cobject cObj;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  cObj = (gst_cobject) OOP_TO_OBJ (oop1);

  if (IS_NIL (cObj->storage) ? IS_C_ULONG (oop2) : IS_C_LONG (oop2))
    {
      SET_COBJECT_OFFSET_OBJ (cObj, TO_C_LONG (oop2));
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* CString replaceWith: aString */
intptr_t
VMpr_CString_replaceWith (int id,
                          volatile int numArgs)
#line 4998 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();

  /* assumes the receiver is already pointing at an area of memory that 
     is the correct size; does not (re)allocate receiver's string at
     all.  */

  if (IS_CLASS (oop2, _gst_string_class)
      || IS_CLASS (oop2, _gst_byte_array_class))
    {
      size_t srcLen;
      gst_uchar *dstBase, *srcBase;
      srcBase = STRING_OOP_CHARS (oop2);
      srcLen = NUM_INDEXABLE_FIELDS (oop2);

      dstBase = *(gst_uchar **) cobject_value (oop1);
      memcpy (dstBase, srcBase, srcLen);
      dstBase[srcLen] = '\0';	/* since it's a CString type, we NUL
				   term it */
      PRIM_SUCCEEDED;
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* ByteArray class fromCdata: aCObject size: anInteger */
intptr_t
VMpr_ByteArray_fromCData_size (int id,
                               volatile int numArgs)
#line 5030 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_INT (oop3))
    {
      intptr_t arg3 = TO_INT (oop3);
      OOP byteArrayOOP =
	_gst_byte_array_new (cobject_value (oop2), arg3);
      SET_STACKTOP (byteArrayOOP);
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* String class fromCdata: aCObject size: anInteger */
intptr_t
VMpr_String_fromCData_size (int id,
                            volatile int numArgs)
#line 5053 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_INT (oop3))
    {
      intptr_t arg3 = TO_INT (oop3);
      OOP stringOOP =
	_gst_counted_string_new (cobject_value (oop2), arg3);
      SET_STACKTOP (stringOOP);
      PRIM_SUCCEEDED;
    }
  UNPOP (2);
  PRIM_FAILED;
}

/* String class fromCdata: aCObject */
intptr_t
VMpr_String_fromCData (int id,
                       volatile int numArgs)
#line 5076 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP stringOOP;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  stringOOP = _gst_string_new (cobject_value (oop2));
  SET_STACKTOP (stringOOP);
  PRIM_SUCCEEDED;
}

/* String asCdata: aCType
 * ByteArray asCdata: aCType */
intptr_t
VMpr_String_ByteArray_asCData (int id,
                               volatile int numArgs)
#line 5094 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = STACK_AT (0);
  oop1 = STACK_AT (1);
  if (is_a_kind_of (OOP_CLASS (oop2), _gst_c_type_class))
    {
      int size = NUM_INDEXABLE_FIELDS (oop1);
      int alloc_size = (id == 0) ? size + 1 : size;
      char *data = xmalloc (alloc_size);
      OOP cObjectOOP = COBJECT_NEW (data, oop2, _gst_c_object_class);
      memcpy (data, OOP_TO_OBJ (oop1)->data, size);
      if (id == 0)
	data[size] = 0;
      POP_N_OOPS (1);
      SET_STACKTOP (cObjectOOP);
      PRIM_SUCCEEDED;
    }
  PRIM_FAILED;
}


/* SystemDictionary byteCodeCounter */
intptr_t
VMpr_SystemDictionary_byteCodeCounter (int id,
                                       volatile int numArgs)
#line 5120 "prims.def"
{
  _gst_primitives_executed++;
  SET_STACKTOP_INT (_gst_bytecode_counter);
  PRIM_SUCCEEDED;
}

/* SystemDictionary debug */
intptr_t
VMpr_SystemDictionary_debug (int id,
                             volatile int numArgs)
#line 5128 "prims.def"
{
  _gst_primitives_executed++;
  _gst_debug ();		/* used to allow gdb to stop based on
				   Smalltalk execution paths.  */
  PRIM_SUCCEEDED;
}


/* Object isUntrusted */
intptr_t
VMpr_Object_isUntrusted (int id,
                         volatile int numArgs)
#line 5138 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  SET_STACKTOP_BOOLEAN (IS_OOP_UNTRUSTED (oop1));
  PRIM_SUCCEEDED;
}

/* Object makeUntrusted: */
intptr_t
VMpr_Object_makeUntrusted (int id,
                           volatile int numArgs)
#line 5149 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (oop2 == _gst_true_oop)
    {
      MAKE_OOP_UNTRUSTED (oop1, true);
      PRIM_SUCCEEDED;
    }
  else if (oop2 == _gst_false_oop)
    {
      MAKE_OOP_UNTRUSTED (oop1, false);
      PRIM_SUCCEEDED;
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* Object isReadOnly */
intptr_t
VMpr_Object_isReadOnly (int id,
                        volatile int numArgs)
#line 5173 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  SET_STACKTOP_BOOLEAN (IS_OOP_READONLY (oop1));
  PRIM_SUCCEEDED;
}

/* Object makeReadOnly: */
intptr_t
VMpr_Object_makeReadOnly (int id,
                          volatile int numArgs)
#line 5184 "prims.def"
{
  OOP oop1;
  OOP oop2;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = STACKTOP ();
  if (IS_OOP (oop1))
    {
      if (oop2 == _gst_true_oop)
        {
          MAKE_OOP_READONLY (oop1, true);
          PRIM_SUCCEEDED;
        }
      else if (oop2 == _gst_false_oop)
        {
          MAKE_OOP_READONLY (oop1, false);
          PRIM_SUCCEEDED;
        }
    }

  UNPOP (1);
  PRIM_FAILED;
}

/* Behavior primCompile: aString */

intptr_t
VMpr_Behavior_primCompile (int id,
                           volatile int numArgs)
#line 5212 "prims.def"
{
  OOP oop1;
  OOP oop2;
  mst_Boolean interrupted;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_CLASS (oop2, _gst_string_class))
    _gst_push_smalltalk_string (oop2);
  else
    _gst_push_stream_oop (oop2);

  _gst_set_compilation_class (oop1);
  _gst_set_compilation_category (_gst_string_new ("still unclassified"));
  interrupted = parse_stream_with_protection (true);
  _gst_pop_stream (true);
  PUSH_OOP (_gst_latest_compiled_method);

  if (interrupted)
    stop_execution ();

  PRIM_SUCCEEDED;
}

/* Behavior primCompile: aString ifError: aBlock */
intptr_t
VMpr_Behavior_primCompileIfError (int id,
                                  volatile int numArgs)
#line 5239 "prims.def"
{
  OOP oop1;
  OOP oop2;
  OOP oop3;
  _gst_primitives_executed++;

  oop3 = POP_OOP ();
  oop2 = POP_OOP ();
  oop1 = POP_OOP ();
  if (IS_CLASS (oop3, _gst_block_closure_class))
    {
      mst_Boolean oldReportErrors = _gst_report_errors;
      mst_Boolean interrupted;

      if (oldReportErrors)
	{
	  /* only clear out these guys on first transition */
	  _gst_first_error_str = _gst_first_error_file = NULL;
	}
      _gst_report_errors = false;
      if (IS_CLASS (oop2, _gst_string_class))
	_gst_push_smalltalk_string (oop2);
      else
	_gst_push_stream_oop (oop2);

      _gst_set_compilation_class (oop1);
      _gst_set_compilation_category (_gst_string_new ("still unclassified"));
      interrupted = parse_stream_with_protection (true);
      _gst_pop_stream (true);
      _gst_report_errors = oldReportErrors;
      PUSH_OOP (_gst_latest_compiled_method);

      if (interrupted)
        stop_execution ();

      else if (_gst_first_error_str != NULL)
	{
	  SET_STACKTOP (oop3);	/* block context */
	  if (_gst_first_error_file != NULL)
	    {
	      PUSH_OOP (_gst_string_new (_gst_first_error_file));
	      xfree (_gst_first_error_file);
	    }
	  else
	    PUSH_OOP (_gst_nil_oop);

	  PUSH_INT (_gst_first_error_line);
	  PUSH_OOP (_gst_string_new (_gst_first_error_str));
	  xfree (_gst_first_error_str);
	  _gst_first_error_str = _gst_first_error_file = NULL;
	  _gst_report_errors = oldReportErrors;
	  if (send_block_value (3, 3))
	    PRIM_FAILED;
	  else
	    PRIM_SUCCEEDED_RELOAD_IP;
	}

      PRIM_SUCCEEDED;
    }
  UNPOP (3);
  PRIM_FAILED;
}

/* CCallbackDescriptor link */
intptr_t
VMpr_CCallbackDescriptor_link (int id,
                               volatile int numArgs)
#line 5304 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();
  _gst_make_closure (oop1);

  /* Always fail so as to run the Smalltalk code that finishes the setup.  */
  PRIM_FAILED;
}

/* CFunctionDescriptor addressOf: funcNameString */
intptr_t
VMpr_CFuncDescriptor_addressOf (int id,
                                volatile int numArgs)
#line 5317 "prims.def"
{
  OOP oop1;
  _gst_primitives_executed++;

  oop1 = STACKTOP ();

  if (IS_CLASS (oop1, _gst_string_class))
    {
      char *funcName = (char *) _gst_to_cstring (oop1);
      void *funcAddr = _gst_lookup_function (funcName);
      if (funcAddr)
	{
	  POP_N_OOPS (1);
	  SET_STACKTOP (COBJECT_NEW (funcAddr, _gst_nil_oop, _gst_c_object_class));
	  PRIM_SUCCEEDED;
	}
    }

  PRIM_FAILED;
}

/* Object snapshot: aString */
intptr_t
VMpr_ObjectMemory_snapshot (int id,
                            volatile int numArgs)
#line 5340 "prims.def"
{
  char *fileName;
  OOP oop2;
  interp_jmp_buf jb;
  _gst_primitives_executed++;

  oop2 = POP_OOP ();
  if (IS_CLASS (oop2, _gst_string_class))
    {
      mst_Boolean success;
      fileName = _gst_to_cstring (oop2);
      errno = 0;

      /* first overwrite the stack top with true. When we resume from
	 the save, the stack will be in this state. See below. */
      SET_STACKTOP (_gst_true_oop);

      push_jmp_buf (&jb, false, get_active_process ());
      if (setjmp (jb.jmpBuf) == 0)
        success = _gst_save_to_file (fileName);
      else
        {
          success = false;
          errno = EINTR;
        }

      xfree (fileName);
      if (pop_jmp_buf ())
        {
          stop_execution ();
          PRIM_SUCCEEDED;
        }
      else if (success)
	{
	  /* We're returning in the parent, not resuming from
	     save. Overwite the stack top again, with false this time,
	     to let the caller know which side of the fork we're on. */
	  SET_STACKTOP (_gst_false_oop);
	  PRIM_SUCCEEDED;
        }
      else
        _gst_set_errno (errno);
    }
  UNPOP (1);
  PRIM_FAILED;
}

/* Object basicPrint */
intptr_t
VMpr_Object_basicPrint (int id,
                        volatile int numArgs)
#line 5389 "prims.def"
{
  _gst_primitives_executed++;
  printf ("Object: %O", STACKTOP ());
  fflush (stdout);
  PRIM_SUCCEEDED;
}

/* Object makeWeak */
intptr_t
VMpr_Object_makeWeak (int id,
                      volatile int numArgs)
#line 5398 "prims.def"
{
  OOP oop1 = STACKTOP ();
  _gst_primitives_executed++;

  if (IS_INT (oop1))
    PRIM_FAILED;

  if (!IS_OOP_WEAK (oop1))
    _gst_make_oop_weak (oop1);

  PRIM_SUCCEEDED;
}

/* Stream fileInLine: lineNum fileName: aString at: charPosInt */

intptr_t
VMpr_Stream_fileInLine (int id,
                        volatile int numArgs)
#line 5414 "prims.def"
{
  OOP oop4 = POP_OOP ();
  OOP oop3 = POP_OOP ();
  OOP oop2 = (numArgs == 4 ? POP_OOP () : oop3);
  OOP oop1 = POP_OOP ();
  OOP streamOOP = STACKTOP ();
  enum undeclared_strategy old;
  
  if (!RECEIVER_IS_OOP (streamOOP))
    PRIM_FAILED;

  if (IS_INT (oop1)
      && (IS_NIL (oop3)
	  || (IS_CLASS (oop3, _gst_string_class) && IS_INT (oop4))))
    {
      mst_Boolean interrupted;
      intptr_t arg1 = TO_INT (oop1);
      intptr_t arg4 = TO_INT (oop4);

      _gst_push_stream_oop (streamOOP);
      _gst_set_stream_info (arg1, oop2, oop3, arg4);
      old = _gst_set_undeclared (UNDECLARED_GLOBALS);
      interrupted = parse_stream_with_protection (false);
      _gst_set_undeclared (old);
      _gst_pop_stream (false);
      if (interrupted)
	stop_execution ();

      PRIM_SUCCEEDED;
    }

  PRIM_FAILED;
}

/* FileDescriptor>>#fileOp..., variadic */

intptr_t
VMpr_FileDescriptor_fileOp (int id,
                            volatile int numArgs)
#line 5451 "prims.def"
{
  char *fileName, *fileName2;
  gst_file_stream fileStream;
  int fd, rc;
  OOP oop1;
  OOP *oopVec = alloca (numArgs * sizeof (OOP));
  int i;
  intptr_t arg1;
  OOP resultOOP;

  _gst_primitives_executed++;

  for (i = numArgs; --i >= 0;)
    oopVec[i] = POP_OOP ();

  resultOOP = oop1 = STACKTOP ();
  UNPOP (numArgs);

  if (!IS_INT (oopVec[0]))
    goto fail;

  arg1 = TO_INT (oopVec[0]);
  switch (arg1) {
  case PRIM_OPEN_FILE:
  case PRIM_OPEN_PIPE:
    {
      int is_pipe = false;
      char *fileMode = NULL;
      int access = 0;
      struct stat st;

      /* open: fileName[1] mode: mode[2] or popen: command[1] dir:
         direction[2] */
      fileName = _gst_to_cstring (oopVec[1]);
      if (IS_INT (oopVec[2]) && arg1 == PRIM_OPEN_FILE)
        {
          fd = open ((char *) fileName, TO_INT (oopVec[2]));
	  access = TO_INT (oopVec[2]) && (O_RDONLY | O_WRONLY | O_RDWR);
	}

      else if (!is_a_kind_of (OOP_CLASS (oopVec[1]), _gst_string_class))
        fd = -1;

      else if (arg1 == PRIM_OPEN_FILE)
        {
	  fileMode = _gst_to_cstring (oopVec[2]);
          fd = _gst_open_file ((char *) fileName, (char *) fileMode);
	  memset (&st, 0, sizeof (st));
          fstat (fd, &st);
	  is_pipe =
	    S_ISFIFO(st.st_mode) ? true :
	    S_ISREG(st.st_mode) && st.st_size > 0 ? false : -1;
	}

      else
        {
	  fileMode = _gst_to_cstring (oopVec[2]);
	  fd = _gst_open_pipe (fileName, fileMode);
	  is_pipe = true;
	}

      if (fileMode)
	{
          access = strchr (fileMode, '+') ? O_RDWR :
	           (fileMode[0] == 'r') ? O_RDONLY : O_WRONLY;
	  
          xfree (fileMode);
	}

      xfree (fileName);
      if (fd < 0)
	goto fail;

      _gst_set_file_stream_file (oop1, fd, oopVec[1],
			         is_pipe, access, false);

      goto succeed;
    }

  case PRIM_MK_TEMP:
    fileName = _gst_to_cstring (oopVec[1]);
    asprintf (&fileName2, "%sXXXXXX", fileName);
    fd = mkstemp ((char *) fileName2);

    xfree (fileName);
    if (fd < 0)
      {
        xfree (fileName2);
	goto fail;
      }
    
    _gst_set_file_stream_file (oop1, fd, _gst_string_new (fileName2),
			       false, O_RDWR, false);

    xfree (fileName2);
    goto succeed;
  }

  fileStream = (gst_file_stream) OOP_TO_OBJ (oop1);
  if (!IS_INT (fileStream->fd))
    goto fail;

  fd = TO_INT (fileStream->fd);
  switch (arg1)
    {
    case PRIM_CLOSE_FILE:	/* FileDescriptor close */
      _gst_remove_fd_polling_handlers (fd);
      rc = close (fd);
      if (rc == 0)
        fileStream->fd = _gst_nil_oop;
      resultOOP = FROM_INT (rc);
      goto succeed;

    case PRIM_FSEEK_SET:		/* FileDescriptor position: position */
      if (IS_OFF_T (oopVec[1]) &&
	  lseek (fd, TO_OFF_T (oopVec[1]), SEEK_SET) < 0)
	{
	  errno = 0;
	  break;
	}
      else
        goto succeed;

    case PRIM_FTELL:		/* FileDescriptor position */
      {
	off_t off = lseek(fd, 0, SEEK_CUR);
        if (off < 0)
	  {
	    errno = 0;
	    break;
	  }

	resultOOP = FROM_OFF_T (off);
	goto succeed;
      }

    case PRIM_FEOF:
      {				/* FileDescriptor atEnd */
	off_t oldPos;
	oldPos = lseek (fd, 0, SEEK_CUR);
	if (oldPos >= 0 && lseek (fd, 0, SEEK_END) == oldPos)
	  resultOOP = _gst_true_oop;
	else
	  {
	    resultOOP = _gst_false_oop;
	    if (oldPos >= 0)
	      lseek (fd, oldPos, SEEK_SET);
	  }
	errno = 0;
	goto succeed;
      }

    case PRIM_FSIZE:
      {
	struct stat statBuf;
	if (fstat (fd, &statBuf) < 0)
	  {
	    errno = 0;
	    break;
	  }
	resultOOP = FROM_INT (statBuf.st_size);
	goto succeed;
      }

    case PRIM_PUT_CHARS:
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0)
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  ssize_t result;
	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      result = _gst_write (fd, data + from - 1, to - from + 1);
	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_GET_CHARS:		/* only works for strings */
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0)
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  ssize_t result;
	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      /* Parameters to system calls are not guaranteed to
		 generate a SIGSEGV and for this reason we must
		 touch them manually.  */
	      _gst_grey_oop_range (data + from - 1, to - from + 1);
	      result = _gst_read (fd, data + from - 1, to - from + 1);

	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_GET_CHARS_AT:
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0
	  && IS_OFF_T (oopVec[4]))
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  off_t ofs = TO_OFF_T (oopVec[4]);
	  ssize_t result;

	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      /* Parameters to system calls are not guaranteed to
		 generate a SIGSEGV and for this reason we must
		 touch them manually.  */
	      _gst_grey_oop_range (data + from - 1, to - from + 1);
#if HAVE_PREAD
	      result = pread (fd, data + from - 1, to - from + 1, ofs);
#else
	      {
		off_t save = lseek (fd, ofs, SEEK_SET);
		if (save != -1)
		  {
		    result = _gst_read (fd, data + from - 1, to - from + 1);
		    lseek (fd, save, SEEK_SET);
		  }
		else
		  result = -1;
	      }
#endif

	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_PUT_CHARS_AT:
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0
	  && IS_OFF_T (oopVec[4]))
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  off_t ofs = TO_OFF_T (oopVec[4]);
	  ssize_t result;

	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      /* Parameters to system calls are not guaranteed to
		 generate a SIGSEGV and for this reason we must
		 touch them manually.  */
	      _gst_grey_oop_range (data + from - 1, to - from + 1);
#if HAVE_PWRITE
	      result = pwrite (fd, data + from - 1, to - from + 1, ofs);
#else
	      {
		off_t save = lseek (fd, ofs, SEEK_SET);
		if (save != -1)
		  {
		    result = _gst_write (fd, data + from - 1, to - from + 1);
		    lseek (fd, save, SEEK_SET);
		  }
		else
		  result = -1;
	      }
#endif

	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_FTRUNCATE:
      {
	off_t pos;
	pos = lseek (fd, 0, SEEK_CUR);
	if (pos < 0)
	  break;

	ftruncate (fd, pos);
	goto succeed;
      }

    case PRIM_FSEEK_CUR:		/* FileDescriptor skip: */
      if (IS_OFF_T (oopVec[1]) &&
	  lseek (fd, TO_OFF_T (oopVec[1]), SEEK_CUR) < 0)
	break;
      else
	goto succeed;

    case PRIM_SYNC_POLL:
      {
	int result;

	result = _gst_sync_file_polling (fd, TO_INT (oopVec[1]));
	if (result >= 0)
	  {
	    resultOOP = FROM_INT (result);
	    goto succeed;
	  }
      }
      break;

    case PRIM_ASYNC_POLL:
      {
	int result;

	result =
	  _gst_async_file_polling (fd, TO_INT (oopVec[1]), oopVec[2]);
	if (result >= 0)
	  goto succeed;
      }
      break;

    case PRIM_IS_PIPE:
      {
	off_t result;

	result = lseek (fd, 0, SEEK_END);
	if (result != -1)
	  {
	    lseek (fd, result, SEEK_SET);
	    resultOOP = _gst_false_oop;
	    goto succeed;
	  }
	else if (errno == ESPIPE || errno == EINVAL)
	  {
	    resultOOP = _gst_true_oop;
	    errno = 0;
	    goto succeed;
	  }

	goto fail;
      }

    case PRIM_SHUTDOWN_WRITE:
      shutdown (FD_TO_SOCKET (fd), 1);
#ifdef ENOTSOCK
      if (errno == ENOTSOCK && isatty (fd))
	{
	  char buf[1];
	  write (fd, buf, 0);
	  errno = 0;
	}
#endif
      goto succeed;
    }

 fail:
  if (errno)
    _gst_set_errno (errno);

  PRIM_FAILED;

 succeed:
  POP_N_OOPS (numArgs);
  SET_STACKTOP (resultOOP);
  PRIM_SUCCEEDED;
}

/* FileDescriptor>>#socketOp..., socket version, variadic */

intptr_t
VMpr_FileDescriptor_socketOp (int id,
                              volatile int numArgs)
#line 5841 "prims.def"
{
  gst_file_stream fileStream;
  int fd, rc;
  OOP oop1, resultOOP;
  OOP *oopVec = alloca (numArgs * sizeof (OOP));
  int i;
  intptr_t arg1;

  _gst_primitives_executed++;

#ifdef HAVE_SOCKETS
  for (i = numArgs; --i >= 0;)
    oopVec[i] = POP_OOP ();

  resultOOP = oop1 = STACKTOP ();
  UNPOP (numArgs);
  if (!IS_INT (oopVec[0]))
    goto fail;

  arg1 = TO_INT (oopVec[0]);
  fileStream = (gst_file_stream) OOP_TO_OBJ (oop1);
  if (IS_NIL (fileStream->fd))
    goto fail;

  fd = TO_INT (fileStream->fd);
  switch (arg1)
    {

    case PRIM_CLOSE_FILE:	/* FileDescriptor close */
      {
	int result;
        _gst_remove_fd_polling_handlers (fd);
        rc = close (fd);
        if (rc == 0)
          fileStream->fd = _gst_nil_oop;
        resultOOP = FROM_INT (rc);
        goto succeed;
      }

    case PRIM_PUT_CHARS:
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0)
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  ssize_t result;
	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      clear_socket_error ();
	      result = _gst_send (fd, data + from - 1, to - from + 1, 0);
	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_GET_CHARS:		/* only works for strings */
      if (!IS_INT(oopVec[1])
          && (OOP_INSTANCE_SPEC (oopVec[1]) & ISP_INDEXEDVARS) != GST_ISP_FIXED
          && _gst_log2_sizes[OOP_INSTANCE_SPEC (oopVec[1]) & ISP_SHAPE] == 0)
	{
	  char *data = STRING_OOP_CHARS (oopVec[1]);
	  intptr_t from = TO_INT (oopVec[2]);
	  intptr_t to = TO_INT (oopVec[3]);
	  ssize_t result;
	  if (to >= from - 1
	      && from > 0 && to <= NUM_INDEXABLE_FIELDS (oopVec[1]))
	    {
	      /* Parameters to system calls are not guaranteed to
		 generate a SIGSEGV and for this reason we must
		 touch them manually.  */
	      _gst_grey_oop_range (data + from - 1, to - from + 1);
	      clear_socket_error ();
	      result = _gst_recv (fd, data + from - 1, to - from + 1, 0);
	      if (result != -1)
		{
		  resultOOP = FROM_C_ULONG ((size_t) result);
		  goto succeed;
		}
	    }
	}
      break;

    case PRIM_SYNC_POLL:
      {
	int result;

	result = _gst_sync_file_polling (fd, TO_INT (oopVec[1]));
	if (result >= 0)
	  {
	    resultOOP = FROM_INT (result);
	    goto succeed;
	  }
      }
      break;

    case PRIM_ASYNC_POLL:
      {
	int result;

	result =
	  _gst_async_file_polling (fd, TO_INT (oopVec[1]), oopVec[2]);
	if (result >= 0)
	  goto succeed;
      }
      break;

    case PRIM_IS_PIPE:
      resultOOP =_gst_true_oop;
      goto succeed;
      break;
    }

#endif

 fail:
  if (errno)
    _gst_set_errno (errno);
  PRIM_FAILED;

 succeed:
  POP_N_OOPS (numArgs);
  SET_STACKTOP (resultOOP);
  PRIM_SUCCEEDED;
}

/* C callout primitives.  */

intptr_t
VMpr_CFuncDescriptor_asyncCall (int id,
                                volatile int numArgs)
#line 5976 "prims.def"
{
  OOP resultOOP;
  volatile gst_method_context context;
  OOP contextOOP, cFuncOOP, receiverOOP;
  interp_jmp_buf jb;

  _gst_primitives_executed++;

  if (numArgs == 1)
    {
      contextOOP = POP_OOP ();
      context = (gst_method_context) OOP_TO_OBJ (contextOOP);
      receiverOOP = context->receiver;
    }
  else
    {
      contextOOP = _gst_this_context_oop;
      context = (gst_method_context) OOP_TO_OBJ (contextOOP);
      receiverOOP = _gst_self;
    }

  cFuncOOP = STACKTOP ();
  push_jmp_buf (&jb, false, _gst_nil_oop);
  if (setjmp (jb.jmpBuf) == 0)
    resultOOP = _gst_invoke_croutine (cFuncOOP, receiverOOP,
				   context->contextStack);
  else
    resultOOP = NULL;

  if (pop_jmp_buf ())
    {
      stop_execution ();
      PRIM_SUCCEEDED;
    }

  else if (resultOOP)
    {
      SET_EXCEPT_FLAG (true);
      PRIM_SUCCEEDED;
    }

  if (numArgs == 1)
    UNPOP (1);
  PRIM_FAILED;
}

intptr_t
VMpr_CFuncDescriptor_call (int id,
                           volatile int numArgs)
#line 6023 "prims.def"
{
  volatile gst_method_context context;
  gst_object resultHolderObj;
  OOP receiverOOP, contextOOP, cFuncOOP, resultOOP;
  volatile OOP resultHolderOOP;
  interp_jmp_buf jb;

  _gst_primitives_executed++;

  resultHolderOOP = POP_OOP ();
  if (numArgs == 2)
    {
      contextOOP = POP_OOP ();
      context = (gst_method_context) OOP_TO_OBJ (contextOOP);
      receiverOOP = context->receiver;
    }
  else
    {
      contextOOP = _gst_this_context_oop;
      context = (gst_method_context) OOP_TO_OBJ (contextOOP);
      receiverOOP = _gst_self;
    }

  cFuncOOP = POP_OOP ();

  /* Make the result reachable, and also push it before the
     active process can change.  */
  PUSH_OOP (resultHolderOOP);

  push_jmp_buf (&jb, false, get_active_process ());
  if (setjmp (jb.jmpBuf) == 0)
    resultOOP = _gst_invoke_croutine (cFuncOOP, receiverOOP,
				      context->contextStack);
  else
    resultOOP = NULL;

  if (pop_jmp_buf ())
    {
      stop_execution ();
      PRIM_SUCCEEDED;
    }

  else if (resultOOP)
    {
      if (!IS_NIL (resultHolderOOP))
	{
          resultHolderObj = OOP_TO_OBJ (resultHolderOOP);
          resultHolderObj->data[0] = resultOOP;
	}
      SET_EXCEPT_FLAG (true);
      PRIM_SUCCEEDED;
    }
  
  /* Undo changes to the stack made above */
  POP_N_OOPS (1);
  PUSH_OOP (cFuncOOP);
  if (numArgs == 2)
    PUSH_OOP (contextOOP);
  PUSH_OOP (resultHolderOOP);
  PRIM_FAILED;
}


intptr_t
VMpr_Object_makeEphemeron (int id,
                           volatile int numArgs)
#line 6087 "prims.def"
{
  _gst_primitives_executed++;
  if (NUM_OOPS (OOP_TO_OBJ (STACKTOP ())) == 0)
    PRIM_FAILED;

  MAKE_OOP_EPHEMERON (STACKTOP ());
  PRIM_SUCCEEDED;
}

/* Namespace current: aNamespace */
intptr_t
VMpr_Namespace_setCurrent (int id,
                           volatile int numArgs)
#line 6098 "prims.def"
{
  OOP oop1;

  _gst_primitives_executed++;
  oop1 = STACKTOP ();
  if (is_a_kind_of (OOP_CLASS (oop1), _gst_dictionary_class))
    _gst_current_namespace = oop1;
  else if (is_a_kind_of (OOP_CLASS (oop1), _gst_class_class))
    _gst_current_namespace = _gst_class_variable_dictionary (oop1);

  /* Always fail */
  PRIM_FAILED;
}

intptr_t
VMpr_ObjectMemory_gcPrimitives (int id,
                                volatile int numArgs)
#line 6118 "prims.def"
{
  _gst_primitives_executed++;
  switch (id)
    {
      case 0:
	_gst_scavenge ();
	break;

      case -1:
	_gst_global_compact ();
	break;

      case -2:
	_gst_global_gc (0);
	break;

      case -3:
	SET_STACKTOP_BOOLEAN (_gst_incremental_gc_step ());
	break;

      case -4:
	_gst_finish_incremental_gc ();
	break;
    }
  PRIM_SUCCEEDED;
}

/* SystemDictionary profilerOn */

intptr_t
VMpr_SystemDictionary_rawProfile (int id,
                                  volatile int numArgs)
#line 6148 "prims.def"
{
  OOP oop1 = POP_OOP ();
  if (_gst_raw_profile)
    {
      _gst_record_profile (_gst_this_method, NULL, -1);
      SET_STACKTOP (_gst_raw_profile);
      _gst_unregister_oop (_gst_raw_profile);
    }
  else
    SET_STACKTOP (_gst_nil_oop);
  if (IS_NIL (oop1))
    _gst_raw_profile = NULL;
  else
    {
      _gst_raw_profile = oop1;
      _gst_register_oop (_gst_raw_profile);
      _gst_saved_bytecode_counter = _gst_bytecode_counter;
    }
  PRIM_SUCCEEDED;
}


#undef INT_BIN_OP
#undef BOOL_BIN_OP

intptr_t
VMpr_HOLE (int id,
           volatile int numArgs)
{
  _gst_primitives_executed++;
  _gst_errorf ("Unhandled primitive operation %d", id);

  UNPOP (numArgs);
  PRIM_FAILED;
}

unsigned char
_gst_primitives_md5[16] = { 78, 223, 239, 67, 83, 13, 166, 89,
                            53, 6, 254, 212, 55, 193, 107, 135 };

void
_gst_init_primitives()
{
  int i;
  _gst_default_primitive_table[1].name = "VMpr_SmallInteger_plus";
  _gst_default_primitive_table[1].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[1].id = 0;
  _gst_default_primitive_table[1].func = VMpr_SmallInteger_plus;
  _gst_default_primitive_table[2].name = "VMpr_SmallInteger_minus";
  _gst_default_primitive_table[2].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[2].id = 0;
  _gst_default_primitive_table[2].func = VMpr_SmallInteger_minus;
  _gst_default_primitive_table[3].name = "VMpr_SmallInteger_lt";
  _gst_default_primitive_table[3].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[3].id = 0;
  _gst_default_primitive_table[3].func = VMpr_SmallInteger_lt;
  _gst_default_primitive_table[4].name = "VMpr_SmallInteger_gt";
  _gst_default_primitive_table[4].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[4].id = 0;
  _gst_default_primitive_table[4].func = VMpr_SmallInteger_gt;
  _gst_default_primitive_table[5].name = "VMpr_SmallInteger_le";
  _gst_default_primitive_table[5].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[5].id = 0;
  _gst_default_primitive_table[5].func = VMpr_SmallInteger_le;
  _gst_default_primitive_table[6].name = "VMpr_SmallInteger_ge";
  _gst_default_primitive_table[6].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[6].id = 0;
  _gst_default_primitive_table[6].func = VMpr_SmallInteger_ge;
  _gst_default_primitive_table[7].name = "VMpr_SmallInteger_eq";
  _gst_default_primitive_table[7].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[7].id = 0;
  _gst_default_primitive_table[7].func = VMpr_SmallInteger_eq;
  _gst_default_primitive_table[8].name = "VMpr_SmallInteger_ne";
  _gst_default_primitive_table[8].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[8].id = 0;
  _gst_default_primitive_table[8].func = VMpr_SmallInteger_ne;
  _gst_default_primitive_table[9].name = "VMpr_SmallInteger_times";
  _gst_default_primitive_table[9].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[9].id = 0;
  _gst_default_primitive_table[9].func = VMpr_SmallInteger_times;
  _gst_default_primitive_table[10].name = "VMpr_SmallInteger_divide";
  _gst_default_primitive_table[10].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[10].id = 0;
  _gst_default_primitive_table[10].func = VMpr_SmallInteger_divide;
  _gst_default_primitive_table[11].name = "VMpr_SmallInteger_modulo";
  _gst_default_primitive_table[11].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[11].id = 0;
  _gst_default_primitive_table[11].func = VMpr_SmallInteger_modulo;
  _gst_default_primitive_table[12].name = "VMpr_SmallInteger_intDiv";
  _gst_default_primitive_table[12].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[12].id = 0;
  _gst_default_primitive_table[12].func = VMpr_SmallInteger_intDiv;
  _gst_default_primitive_table[13].name = "VMpr_SmallInteger_quo";
  _gst_default_primitive_table[13].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[13].id = 0;
  _gst_default_primitive_table[13].func = VMpr_SmallInteger_quo;
  _gst_default_primitive_table[14].name = "VMpr_SmallInteger_bitAnd";
  _gst_default_primitive_table[14].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[14].id = 0;
  _gst_default_primitive_table[14].func = VMpr_SmallInteger_bitAnd;
  _gst_default_primitive_table[15].name = "VMpr_SmallInteger_bitOr";
  _gst_default_primitive_table[15].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[15].id = 0;
  _gst_default_primitive_table[15].func = VMpr_SmallInteger_bitOr;
  _gst_default_primitive_table[16].name = "VMpr_SmallInteger_bitXor";
  _gst_default_primitive_table[16].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[16].id = 0;
  _gst_default_primitive_table[16].func = VMpr_SmallInteger_bitXor;
  _gst_default_primitive_table[17].name = "VMpr_SmallInteger_bitShift";
  _gst_default_primitive_table[17].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[17].id = 0;
  _gst_default_primitive_table[17].func = VMpr_SmallInteger_bitShift;
  _gst_default_primitive_table[18].name = "VMpr_SmallInteger_scramble";
  _gst_default_primitive_table[18].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[18].id = 0;
  _gst_default_primitive_table[18].func = VMpr_SmallInteger_scramble;
  _gst_default_primitive_table[19].name = "VMpr_SmallInteger_asFloatD";
  _gst_default_primitive_table[19].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[19].id = 0;
  _gst_default_primitive_table[19].func = VMpr_SmallInteger_asFloatD;
  _gst_default_primitive_table[20].name = "VMpr_SmallInteger_asFloatE";
  _gst_default_primitive_table[20].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[20].id = 0;
  _gst_default_primitive_table[20].func = VMpr_SmallInteger_asFloatE;
  _gst_default_primitive_table[21].name = "VMpr_SmallInteger_asFloatQ";
  _gst_default_primitive_table[21].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[21].id = 0;
  _gst_default_primitive_table[21].func = VMpr_SmallInteger_asFloatQ;
  _gst_default_primitive_table[22].name = "VMpr_LargeInteger_eq";
  _gst_default_primitive_table[22].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[22].id = 0;
  _gst_default_primitive_table[22].func = VMpr_LargeInteger_eq;
  _gst_default_primitive_table[23].name = "VMpr_LargeInteger_ne";
  _gst_default_primitive_table[23].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[23].id = 0;
  _gst_default_primitive_table[23].func = VMpr_LargeInteger_ne;
  _gst_default_primitive_table[24].name = "VMpr_LargeInteger_lt";
  _gst_default_primitive_table[24].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[24].id = 0;
  _gst_default_primitive_table[24].func = VMpr_LargeInteger_lt;
  _gst_default_primitive_table[25].name = "VMpr_LargeInteger_le";
  _gst_default_primitive_table[25].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[25].id = 0;
  _gst_default_primitive_table[25].func = VMpr_LargeInteger_le;
  _gst_default_primitive_table[26].name = "VMpr_LargeInteger_gt";
  _gst_default_primitive_table[26].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[26].id = 0;
  _gst_default_primitive_table[26].func = VMpr_LargeInteger_gt;
  _gst_default_primitive_table[27].name = "VMpr_LargeInteger_ge";
  _gst_default_primitive_table[27].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[27].id = 0;
  _gst_default_primitive_table[27].func = VMpr_LargeInteger_ge;
  _gst_default_primitive_table[28].name = "VMpr_LargeInteger_times";
  _gst_default_primitive_table[28].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[28].id = 0;
  _gst_default_primitive_table[28].func = VMpr_LargeInteger_times;
  _gst_default_primitive_table[29].name = "VMpr_LargeInteger_intDiv";
  _gst_default_primitive_table[29].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[29].id = 0;
  _gst_default_primitive_table[29].func = VMpr_LargeInteger_intDiv;
  _gst_default_primitive_table[30].name = "VMpr_LargeInteger_modulo";
  _gst_default_primitive_table[30].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[30].id = 0;
  _gst_default_primitive_table[30].func = VMpr_LargeInteger_modulo;
  _gst_default_primitive_table[31].name = "VMpr_LargeInteger_divExact";
  _gst_default_primitive_table[31].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[31].id = 0;
  _gst_default_primitive_table[31].func = VMpr_LargeInteger_divExact;
  _gst_default_primitive_table[32].name = "VMpr_LargeInteger_quo";
  _gst_default_primitive_table[32].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[32].id = 0;
  _gst_default_primitive_table[32].func = VMpr_LargeInteger_quo;
  _gst_default_primitive_table[33].name = "VMpr_LargeInteger_rem";
  _gst_default_primitive_table[33].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[33].id = 0;
  _gst_default_primitive_table[33].func = VMpr_LargeInteger_rem;
  _gst_default_primitive_table[34].name = "VMpr_LargeInteger_negated";
  _gst_default_primitive_table[34].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[34].id = 0;
  _gst_default_primitive_table[34].func = VMpr_LargeInteger_negated;
  _gst_default_primitive_table[35].name = "VMpr_LargeInteger_bitAnd";
  _gst_default_primitive_table[35].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[35].id = 0;
  _gst_default_primitive_table[35].func = VMpr_LargeInteger_bitAnd;
  _gst_default_primitive_table[36].name = "VMpr_LargeInteger_bitOr";
  _gst_default_primitive_table[36].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[36].id = 0;
  _gst_default_primitive_table[36].func = VMpr_LargeInteger_bitOr;
  _gst_default_primitive_table[37].name = "VMpr_LargeInteger_bitXor";
  _gst_default_primitive_table[37].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[37].id = 0;
  _gst_default_primitive_table[37].func = VMpr_LargeInteger_bitXor;
  _gst_default_primitive_table[38].name = "VMpr_LargeInteger_bitInvert";
  _gst_default_primitive_table[38].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[38].id = 0;
  _gst_default_primitive_table[38].func = VMpr_LargeInteger_bitInvert;
  _gst_default_primitive_table[39].name = "VMpr_LargeInteger_bitShift";
  _gst_default_primitive_table[39].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[39].id = 0;
  _gst_default_primitive_table[39].func = VMpr_LargeInteger_bitShift;
  _gst_default_primitive_table[40].name = "VMpr_LargeInteger_plus";
  _gst_default_primitive_table[40].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[40].id = 0;
  _gst_default_primitive_table[40].func = VMpr_LargeInteger_plus;
  _gst_default_primitive_table[41].name = "VMpr_LargeInteger_minus";
  _gst_default_primitive_table[41].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[41].id = 0;
  _gst_default_primitive_table[41].func = VMpr_LargeInteger_minus;
  _gst_default_primitive_table[42].name = "VMpr_LargeInteger_gcd";
  _gst_default_primitive_table[42].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[42].id = 0;
  _gst_default_primitive_table[42].func = VMpr_LargeInteger_gcd;
  _gst_default_primitive_table[43].name = "VMpr_LargeInteger_asFloatD";
  _gst_default_primitive_table[43].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[43].id = 0;
  _gst_default_primitive_table[43].func = VMpr_LargeInteger_asFloatD;
  _gst_default_primitive_table[44].name = "VMpr_LargeInteger_asFloatE";
  _gst_default_primitive_table[44].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[44].id = 0;
  _gst_default_primitive_table[44].func = VMpr_LargeInteger_asFloatE;
  _gst_default_primitive_table[45].name = "VMpr_LargeInteger_asFloatQ";
  _gst_default_primitive_table[45].attributes = PRIM_USES_GMP;
  _gst_default_primitive_table[45].id = 0;
  _gst_default_primitive_table[45].func = VMpr_LargeInteger_asFloatQ;
  _gst_default_primitive_table[46].name = "VMpr_FloatD_plus";
  _gst_default_primitive_table[46].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[46].id = 0;
  _gst_default_primitive_table[46].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[47].name = "VMpr_FloatD_minus";
  _gst_default_primitive_table[47].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[47].id = -1;
  _gst_default_primitive_table[47].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[48].name = "VMpr_FloatD_lt";
  _gst_default_primitive_table[48].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[48].id = -2;
  _gst_default_primitive_table[48].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[49].name = "VMpr_FloatD_gt";
  _gst_default_primitive_table[49].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[49].id = -3;
  _gst_default_primitive_table[49].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[50].name = "VMpr_FloatD_le";
  _gst_default_primitive_table[50].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[50].id = -4;
  _gst_default_primitive_table[50].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[51].name = "VMpr_FloatD_ge";
  _gst_default_primitive_table[51].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[51].id = -5;
  _gst_default_primitive_table[51].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[52].name = "VMpr_FloatD_eq";
  _gst_default_primitive_table[52].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[52].id = -6;
  _gst_default_primitive_table[52].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[53].name = "VMpr_FloatD_ne";
  _gst_default_primitive_table[53].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[53].id = -7;
  _gst_default_primitive_table[53].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[54].name = "VMpr_FloatD_times";
  _gst_default_primitive_table[54].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[54].id = -8;
  _gst_default_primitive_table[54].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[55].name = "VMpr_FloatD_divide";
  _gst_default_primitive_table[55].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[55].id = -9;
  _gst_default_primitive_table[55].func = VMpr_FloatD_arith;
  _gst_default_primitive_table[56].name = "VMpr_FloatD_truncated";
  _gst_default_primitive_table[56].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[56].id = 0;
  _gst_default_primitive_table[56].func = VMpr_FloatD_truncated;
  _gst_default_primitive_table[57].name = "VMpr_FloatD_fractionPart";
  _gst_default_primitive_table[57].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[57].id = 0;
  _gst_default_primitive_table[57].func = VMpr_FloatD_fractionPart;
  _gst_default_primitive_table[58].name = "VMpr_FloatD_exponent";
  _gst_default_primitive_table[58].attributes = PRIM_CHECKS_RECEIVER | PRIM_RETURN_SMALL_SMALLINTEGER;
  _gst_default_primitive_table[58].id = 0;
  _gst_default_primitive_table[58].func = VMpr_FloatD_exponent;
  _gst_default_primitive_table[59].name = "VMpr_FloatD_timesTwoPower";
  _gst_default_primitive_table[59].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[59].id = 0;
  _gst_default_primitive_table[59].func = VMpr_FloatD_timesTwoPower;
  _gst_default_primitive_table[60].name = "VMpr_FloatD_asFloatE";
  _gst_default_primitive_table[60].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[60].id = 0;
  _gst_default_primitive_table[60].func = VMpr_FloatD_asFloatE;
  _gst_default_primitive_table[61].name = "VMpr_FloatD_asFloatQ";
  _gst_default_primitive_table[61].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[61].id = 0;
  _gst_default_primitive_table[61].func = VMpr_FloatD_asFloatQ;
  _gst_default_primitive_table[62].name = "VMpr_FloatE_plus";
  _gst_default_primitive_table[62].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[62].id = 0;
  _gst_default_primitive_table[62].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[63].name = "VMpr_FloatE_minus";
  _gst_default_primitive_table[63].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[63].id = -1;
  _gst_default_primitive_table[63].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[64].name = "VMpr_FloatE_lt";
  _gst_default_primitive_table[64].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[64].id = -2;
  _gst_default_primitive_table[64].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[65].name = "VMpr_FloatE_gt";
  _gst_default_primitive_table[65].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[65].id = -3;
  _gst_default_primitive_table[65].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[66].name = "VMpr_FloatE_le";
  _gst_default_primitive_table[66].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[66].id = -4;
  _gst_default_primitive_table[66].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[67].name = "VMpr_FloatE_ge";
  _gst_default_primitive_table[67].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[67].id = -5;
  _gst_default_primitive_table[67].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[68].name = "VMpr_FloatE_eq";
  _gst_default_primitive_table[68].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[68].id = -6;
  _gst_default_primitive_table[68].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[69].name = "VMpr_FloatE_ne";
  _gst_default_primitive_table[69].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[69].id = -7;
  _gst_default_primitive_table[69].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[70].name = "VMpr_FloatE_times";
  _gst_default_primitive_table[70].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[70].id = -8;
  _gst_default_primitive_table[70].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[71].name = "VMpr_FloatE_divide";
  _gst_default_primitive_table[71].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[71].id = -9;
  _gst_default_primitive_table[71].func = VMpr_FloatE_arith;
  _gst_default_primitive_table[72].name = "VMpr_FloatE_truncated";
  _gst_default_primitive_table[72].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[72].id = 0;
  _gst_default_primitive_table[72].func = VMpr_FloatE_truncated;
  _gst_default_primitive_table[73].name = "VMpr_FloatE_fractionPart";
  _gst_default_primitive_table[73].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[73].id = 0;
  _gst_default_primitive_table[73].func = VMpr_FloatE_fractionPart;
  _gst_default_primitive_table[74].name = "VMpr_FloatE_exponent";
  _gst_default_primitive_table[74].attributes = PRIM_CHECKS_RECEIVER | PRIM_RETURN_SMALL_SMALLINTEGER;
  _gst_default_primitive_table[74].id = 0;
  _gst_default_primitive_table[74].func = VMpr_FloatE_exponent;
  _gst_default_primitive_table[75].name = "VMpr_FloatE_timesTwoPower";
  _gst_default_primitive_table[75].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[75].id = 0;
  _gst_default_primitive_table[75].func = VMpr_FloatE_timesTwoPower;
  _gst_default_primitive_table[76].name = "VMpr_FloatE_asFloatD";
  _gst_default_primitive_table[76].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[76].id = 0;
  _gst_default_primitive_table[76].func = VMpr_FloatE_asFloatD;
  _gst_default_primitive_table[77].name = "VMpr_FloatE_asFloatQ";
  _gst_default_primitive_table[77].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[77].id = 0;
  _gst_default_primitive_table[77].func = VMpr_FloatE_asFloatQ;
  _gst_default_primitive_table[78].name = "VMpr_FloatQ_plus";
  _gst_default_primitive_table[78].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[78].id = 0;
  _gst_default_primitive_table[78].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[79].name = "VMpr_FloatQ_minus";
  _gst_default_primitive_table[79].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[79].id = -1;
  _gst_default_primitive_table[79].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[80].name = "VMpr_FloatQ_lt";
  _gst_default_primitive_table[80].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[80].id = -2;
  _gst_default_primitive_table[80].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[81].name = "VMpr_FloatQ_gt";
  _gst_default_primitive_table[81].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[81].id = -3;
  _gst_default_primitive_table[81].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[82].name = "VMpr_FloatQ_le";
  _gst_default_primitive_table[82].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[82].id = -4;
  _gst_default_primitive_table[82].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[83].name = "VMpr_FloatQ_ge";
  _gst_default_primitive_table[83].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[83].id = -5;
  _gst_default_primitive_table[83].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[84].name = "VMpr_FloatQ_eq";
  _gst_default_primitive_table[84].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[84].id = -6;
  _gst_default_primitive_table[84].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[85].name = "VMpr_FloatQ_ne";
  _gst_default_primitive_table[85].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[85].id = -7;
  _gst_default_primitive_table[85].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[86].name = "VMpr_FloatQ_times";
  _gst_default_primitive_table[86].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[86].id = -8;
  _gst_default_primitive_table[86].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[87].name = "VMpr_FloatQ_divide";
  _gst_default_primitive_table[87].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[87].id = -9;
  _gst_default_primitive_table[87].func = VMpr_FloatQ_arith;
  _gst_default_primitive_table[88].name = "VMpr_FloatQ_truncated";
  _gst_default_primitive_table[88].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[88].id = 0;
  _gst_default_primitive_table[88].func = VMpr_FloatQ_truncated;
  _gst_default_primitive_table[89].name = "VMpr_FloatQ_fractionPart";
  _gst_default_primitive_table[89].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[89].id = 0;
  _gst_default_primitive_table[89].func = VMpr_FloatQ_fractionPart;
  _gst_default_primitive_table[90].name = "VMpr_FloatQ_exponent";
  _gst_default_primitive_table[90].attributes = PRIM_CHECKS_RECEIVER | PRIM_RETURN_SMALL_SMALLINTEGER;
  _gst_default_primitive_table[90].id = 0;
  _gst_default_primitive_table[90].func = VMpr_FloatQ_exponent;
  _gst_default_primitive_table[91].name = "VMpr_FloatQ_timesTwoPower";
  _gst_default_primitive_table[91].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[91].id = 0;
  _gst_default_primitive_table[91].func = VMpr_FloatQ_timesTwoPower;
  _gst_default_primitive_table[92].name = "VMpr_FloatQ_asFloatD";
  _gst_default_primitive_table[92].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[92].id = 0;
  _gst_default_primitive_table[92].func = VMpr_FloatQ_asFloatD;
  _gst_default_primitive_table[93].name = "VMpr_FloatQ_asFloatE";
  _gst_default_primitive_table[93].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[93].id = 0;
  _gst_default_primitive_table[93].func = VMpr_FloatQ_asFloatE;
  _gst_default_primitive_table[94].name = "VMpr_Object_basicAt";
  _gst_default_primitive_table[94].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[94].id = 60;
  _gst_default_primitive_table[94].func = VMpr_Object_basicAt;
  _gst_default_primitive_table[95].name = "VMpr_Object_basicAtPut";
  _gst_default_primitive_table[95].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[95].id = 61;
  _gst_default_primitive_table[95].func = VMpr_Object_basicAtPut;
  _gst_default_primitive_table[96].name = "VMpr_Object_basicSize";
  _gst_default_primitive_table[96].attributes = PRIM_SUCCEED | PRIM_RETURN_SMALL_SMALLINTEGER | PRIM_INLINED;
  _gst_default_primitive_table[96].id = 62;
  _gst_default_primitive_table[96].func = VMpr_Object_basicSize;
  _gst_default_primitive_table[97].name = "VMpr_CharacterArray_valueAt";
  _gst_default_primitive_table[97].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[97].id = 60;
  _gst_default_primitive_table[97].func = VMpr_CharacterArray_valueAt;
  _gst_default_primitive_table[98].name = "VMpr_CharacterArray_valueAtPut";
  _gst_default_primitive_table[98].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[98].id = 61;
  _gst_default_primitive_table[98].func = VMpr_CharacterArray_valueAtPut;
  _gst_default_primitive_table[99].name = "VMpr_CompiledCode_verificationResult";
  _gst_default_primitive_table[99].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[99].id = 0;
  _gst_default_primitive_table[99].func = VMpr_CompiledCode_verificationResult;
  _gst_default_primitive_table[100].name = "VMpr_CompiledBlock_create";
  _gst_default_primitive_table[100].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[100].id = 0;
  _gst_default_primitive_table[100].func = VMpr_CompiledBlock_create;
  _gst_default_primitive_table[101].name = "VMpr_CompiledMethod_create";
  _gst_default_primitive_table[101].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[101].id = 0;
  _gst_default_primitive_table[101].func = VMpr_CompiledMethod_create;
  _gst_default_primitive_table[102].name = "VMpr_Object_shallowCopy";
  _gst_default_primitive_table[102].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[102].id = 0;
  _gst_default_primitive_table[102].func = VMpr_Object_shallowCopy;
  _gst_default_primitive_table[103].name = "VMpr_Behavior_basicNew";
  _gst_default_primitive_table[103].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[103].id = 70;
  _gst_default_primitive_table[103].func = VMpr_Behavior_basicNew;
  _gst_default_primitive_table[104].name = "VMpr_Behavior_basicNewColon";
  _gst_default_primitive_table[104].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[104].id = 71;
  _gst_default_primitive_table[104].func = VMpr_Behavior_basicNewColon;
  _gst_default_primitive_table[105].name = "VMpr_Object_become";
  _gst_default_primitive_table[105].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[105].id = 0;
  _gst_default_primitive_table[105].func = VMpr_Object_become;
  _gst_default_primitive_table[106].name = "VMpr_Object_instVarAt";
  _gst_default_primitive_table[106].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[106].id = 73;
  _gst_default_primitive_table[106].func = VMpr_Object_instVarAt;
  _gst_default_primitive_table[107].name = "VMpr_Object_instVarAtPut";
  _gst_default_primitive_table[107].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_INLINED;
  _gst_default_primitive_table[107].id = 74;
  _gst_default_primitive_table[107].func = VMpr_Object_instVarAtPut;
  _gst_default_primitive_table[108].name = "VMpr_Object_hash";
  _gst_default_primitive_table[108].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[108].id = 0;
  _gst_default_primitive_table[108].func = VMpr_Object_hash;
  _gst_default_primitive_table[109].name = "VMpr_SmallInteger_asObject";
  _gst_default_primitive_table[109].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[109].id = 0;
  _gst_default_primitive_table[109].func = VMpr_SmallInteger_asObject;
  _gst_default_primitive_table[110].name = "VMpr_SmallInteger_nextValidOop";
  _gst_default_primitive_table[110].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[110].id = 0;
  _gst_default_primitive_table[110].func = VMpr_SmallInteger_nextValidOop;
  _gst_default_primitive_table[111].name = "VMpr_Behavior_someInstance";
  _gst_default_primitive_table[111].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[111].id = 0;
  _gst_default_primitive_table[111].func = VMpr_Behavior_someInstance;
  _gst_default_primitive_table[112].name = "VMpr_Object_nextInstance";
  _gst_default_primitive_table[112].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[112].id = 0;
  _gst_default_primitive_table[112].func = VMpr_Object_nextInstance;
  _gst_default_primitive_table[113].name = "VMpr_Object_becomeForward";
  _gst_default_primitive_table[113].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[113].id = 0;
  _gst_default_primitive_table[113].func = VMpr_Object_becomeForward;
  _gst_default_primitive_table[114].name = "VMpr_Object_allOwners";
  _gst_default_primitive_table[114].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[114].id = 0;
  _gst_default_primitive_table[114].func = VMpr_Object_allOwners;
  _gst_default_primitive_table[115].name = "VMpr_ContextPart_thisContext";
  _gst_default_primitive_table[115].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[115].id = 0;
  _gst_default_primitive_table[115].func = VMpr_ContextPart_thisContext;
  _gst_default_primitive_table[116].name = "VMpr_ContextPart_continue";
  _gst_default_primitive_table[116].attributes = PRIM_CHECKS_RECEIVER | PRIM_RELOAD_IP;
  _gst_default_primitive_table[116].id = 0;
  _gst_default_primitive_table[116].func = VMpr_ContextPart_continue;
  _gst_default_primitive_table[117].name = "VMpr_Continuation_resume";
  _gst_default_primitive_table[117].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[117].id = 0;
  _gst_default_primitive_table[117].func = VMpr_Continuation_resume;
  _gst_default_primitive_table[118].name = "VMpr_BlockClosure_value";
  _gst_default_primitive_table[118].attributes = PRIM_FAIL | PRIM_RELOAD_IP | PRIM_CACHE_NEW_IP;
  _gst_default_primitive_table[118].id = 0;
  _gst_default_primitive_table[118].func = VMpr_BlockClosure_value;
  _gst_default_primitive_table[119].name = "VMpr_BlockClosure_cull";
  _gst_default_primitive_table[119].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[119].id = 0;
  _gst_default_primitive_table[119].func = VMpr_BlockClosure_cull;
  _gst_default_primitive_table[120].name = "VMpr_BlockClosure_valueAndResumeOnUnwind";
  _gst_default_primitive_table[120].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[120].id = 0;
  _gst_default_primitive_table[120].func = VMpr_BlockClosure_valueAndResumeOnUnwind;
  _gst_default_primitive_table[121].name = "VMpr_BlockClosure_valueWithArguments";
  _gst_default_primitive_table[121].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[121].id = 0;
  _gst_default_primitive_table[121].func = VMpr_BlockClosure_valueWithArguments;
  _gst_default_primitive_table[122].name = "VMpr_Object_perform";
  _gst_default_primitive_table[122].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[122].id = 0;
  _gst_default_primitive_table[122].func = VMpr_Object_perform;
  _gst_default_primitive_table[123].name = "VMpr_Object_performWithArguments";
  _gst_default_primitive_table[123].attributes = PRIM_FAIL | PRIM_RELOAD_IP;
  _gst_default_primitive_table[123].id = 0;
  _gst_default_primitive_table[123].func = VMpr_Object_performWithArguments;
  _gst_default_primitive_table[124].name = "VMpr_Semaphore_notifyAll";
  _gst_default_primitive_table[124].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[124].id = 0;
  _gst_default_primitive_table[124].func = VMpr_Semaphore_notifyAll;
  _gst_default_primitive_table[125].name = "VMpr_Semaphore_signal";
  _gst_default_primitive_table[125].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[125].id = 0;
  _gst_default_primitive_table[125].func = VMpr_Semaphore_signalNotify;
  _gst_default_primitive_table[126].name = "VMpr_Semaphore_notify";
  _gst_default_primitive_table[126].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[126].id = -1;
  _gst_default_primitive_table[126].func = VMpr_Semaphore_signalNotify;
  _gst_default_primitive_table[127].name = "VMpr_Semaphore_lock";
  _gst_default_primitive_table[127].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[127].id = 0;
  _gst_default_primitive_table[127].func = VMpr_Semaphore_lock;
  _gst_default_primitive_table[128].name = "VMpr_Semaphore_wait";
  _gst_default_primitive_table[128].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[128].id = 0;
  _gst_default_primitive_table[128].func = VMpr_Semaphore_wait;
  _gst_default_primitive_table[129].name = "VMpr_Semaphore_waitAfterSignalling";
  _gst_default_primitive_table[129].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[129].id = 0;
  _gst_default_primitive_table[129].func = VMpr_Semaphore_waitAfterSignalling;
  _gst_default_primitive_table[130].name = "VMpr_Process_suspend";
  _gst_default_primitive_table[130].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[130].id = 0;
  _gst_default_primitive_table[130].func = VMpr_Process_suspend;
  _gst_default_primitive_table[131].name = "VMpr_Process_resume";
  _gst_default_primitive_table[131].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[131].id = 0;
  _gst_default_primitive_table[131].func = VMpr_Process_resume;
  _gst_default_primitive_table[132].name = "VMpr_Process_singleStepWaitingOn";
  _gst_default_primitive_table[132].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[132].id = 0;
  _gst_default_primitive_table[132].func = VMpr_Process_singleStepWaitingOn;
  _gst_default_primitive_table[133].name = "VMpr_Process_yield";
  _gst_default_primitive_table[133].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[133].id = 0;
  _gst_default_primitive_table[133].func = VMpr_Process_yield;
  _gst_default_primitive_table[134].name = "VMpr_Processor_pause";
  _gst_default_primitive_table[134].attributes = PRIM_SUCCEED | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[134].id = 0;
  _gst_default_primitive_table[134].func = VMpr_Processor_pause;
  _gst_default_primitive_table[135].name = "VMpr_Behavior_flushCache";
  _gst_default_primitive_table[135].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[135].id = 0;
  _gst_default_primitive_table[135].func = VMpr_Behavior_flushCache;
  _gst_default_primitive_table[136].name = "VMpr_CompiledCode_discardTranslation";
  _gst_default_primitive_table[136].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[136].id = 0;
  _gst_default_primitive_table[136].func = VMpr_CompiledCode_discardTranslation;
  _gst_default_primitive_table[137].name = "VMpr_Object_changeClassTo";
  _gst_default_primitive_table[137].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[137].id = 0;
  _gst_default_primitive_table[137].func = VMpr_Object_changeClassTo;
  _gst_default_primitive_table[138].name = "VMpr_Time_timezoneBias";
  _gst_default_primitive_table[138].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[138].id = 0;
  _gst_default_primitive_table[138].func = VMpr_Time_timezoneBias;
  _gst_default_primitive_table[139].name = "VMpr_Time_timezone";
  _gst_default_primitive_table[139].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[139].id = 0;
  _gst_default_primitive_table[139].func = VMpr_Time_timezone;
  _gst_default_primitive_table[140].name = "VMpr_Time_secondClock";
  _gst_default_primitive_table[140].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[140].id = 0;
  _gst_default_primitive_table[140].func = VMpr_Time_secondClock;
  _gst_default_primitive_table[141].name = "VMpr_Time_nanosecondClock";
  _gst_default_primitive_table[141].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[141].id = 0;
  _gst_default_primitive_table[141].func = VMpr_Time_nanosecondClock;
  _gst_default_primitive_table[142].name = "VMpr_Time_millisecondClock";
  _gst_default_primitive_table[142].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[142].id = 0;
  _gst_default_primitive_table[142].func = VMpr_Time_millisecondClock;
  _gst_default_primitive_table[143].name = "VMpr_Processor_signalAtMilliseconds";
  _gst_default_primitive_table[143].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[143].id = 0;
  _gst_default_primitive_table[143].func = VMpr_Processor_signalAt;
  _gst_default_primitive_table[144].name = "VMpr_Processor_signalAtNanosecondClockValue";
  _gst_default_primitive_table[144].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[144].id = -1;
  _gst_default_primitive_table[144].func = VMpr_Processor_signalAt;
  _gst_default_primitive_table[145].name = "VMpr_Processor_isTimeoutProgrammed";
  _gst_default_primitive_table[145].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[145].id = 0;
  _gst_default_primitive_table[145].func = VMpr_Processor_isTimeoutProgrammed;
  _gst_default_primitive_table[146].name = "VMpr_String_similarityTo";
  _gst_default_primitive_table[146].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[146].id = 0;
  _gst_default_primitive_table[146].func = VMpr_String_similarityTo;
  _gst_default_primitive_table[147].name = "VMpr_String_hash";
  _gst_default_primitive_table[147].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[147].id = 0;
  _gst_default_primitive_table[147].func = VMpr_String_hash;
  _gst_default_primitive_table[148].name = "VMpr_ArrayedCollection_equal";
  _gst_default_primitive_table[148].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[148].id = 0;
  _gst_default_primitive_table[148].func = VMpr_ArrayedCollection_equal;
  _gst_default_primitive_table[149].name = "VMpr_ArrayedCollection_indexOfStartingAt";
  _gst_default_primitive_table[149].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[149].id = 0;
  _gst_default_primitive_table[149].func = VMpr_ArrayedCollection_indexOfStartingAt;
  _gst_default_primitive_table[150].name = "VMpr_ArrayedCollection_replaceFromToWithStartingAt";
  _gst_default_primitive_table[150].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[150].id = 0;
  _gst_default_primitive_table[150].func = VMpr_ArrayedCollection_replaceFromToWithStartingAt;
  _gst_default_primitive_table[151].name = "VMpr_Object_identity";
  _gst_default_primitive_table[151].attributes = PRIM_SUCCEED | PRIM_INLINED;
  _gst_default_primitive_table[151].id = 110;
  _gst_default_primitive_table[151].func = VMpr_Object_identity;
  _gst_default_primitive_table[152].name = "VMpr_Object_class";
  _gst_default_primitive_table[152].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[152].id = 111;
  _gst_default_primitive_table[152].func = VMpr_Object_class;
  _gst_default_primitive_table[153].name = "VMpr_ObjectMemory_quit";
  _gst_default_primitive_table[153].attributes = PRIM_FAIL;
  _gst_default_primitive_table[153].id = 0;
  _gst_default_primitive_table[153].func = VMpr_ObjectMemory_quit;
  _gst_default_primitive_table[154].name = "VMpr_ObjectMemory_abort";
  _gst_default_primitive_table[154].attributes = PRIM_FAIL;
  _gst_default_primitive_table[154].id = 0;
  _gst_default_primitive_table[154].func = VMpr_ObjectMemory_abort;
  _gst_default_primitive_table[155].name = "VMpr_Dictionary_at";
  _gst_default_primitive_table[155].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[155].id = 0;
  _gst_default_primitive_table[155].func = VMpr_Dictionary_at;
  _gst_default_primitive_table[156].name = "VMpr_Object_bootstrapError";
  _gst_default_primitive_table[156].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[156].id = 0;
  _gst_default_primitive_table[156].func = VMpr_Object_bootstrapException;
  _gst_default_primitive_table[157].name = "VMpr_Object_bootstrapDNU";
  _gst_default_primitive_table[157].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[157].id = -1;
  _gst_default_primitive_table[157].func = VMpr_Object_bootstrapException;
  _gst_default_primitive_table[158].name = "VMpr_Character_create";
  _gst_default_primitive_table[158].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[158].id = 0;
  _gst_default_primitive_table[158].func = VMpr_Character_create;
  _gst_default_primitive_table[159].name = "VMpr_UnicodeCharacter_create";
  _gst_default_primitive_table[159].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[159].id = 0;
  _gst_default_primitive_table[159].func = VMpr_UnicodeCharacter_create;
  _gst_default_primitive_table[160].name = "VMpr_Character_equal";
  _gst_default_primitive_table[160].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[160].id = 0;
  _gst_default_primitive_table[160].func = VMpr_Character_equal;
  _gst_default_primitive_table[161].name = "VMpr_Symbol_intern";
  _gst_default_primitive_table[161].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[161].id = 0;
  _gst_default_primitive_table[161].func = VMpr_Symbol_intern;
  _gst_default_primitive_table[162].name = "VMpr_Dictionary_new";
  _gst_default_primitive_table[162].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[162].id = 0;
  _gst_default_primitive_table[162].func = VMpr_Dictionary_new;
  _gst_default_primitive_table[163].name = "VMpr_Memory_addressOfOOP";
  _gst_default_primitive_table[163].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[163].id = 0;
  _gst_default_primitive_table[163].func = VMpr_Memory_addressOfOOP;
  _gst_default_primitive_table[164].name = "VMpr_Memory_addressOf";
  _gst_default_primitive_table[164].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[164].id = 0;
  _gst_default_primitive_table[164].func = VMpr_Memory_addressOf;
  _gst_default_primitive_table[165].name = "VMpr_SystemDictionary_backtrace";
  _gst_default_primitive_table[165].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[165].id = 0;
  _gst_default_primitive_table[165].func = VMpr_SystemDictionary_backtrace;
  _gst_default_primitive_table[166].name = "VMpr_SystemDictionary_getTraceFlag";
  _gst_default_primitive_table[166].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[166].id = 0;
  _gst_default_primitive_table[166].func = VMpr_SystemDictionary_getTraceFlag;
  _gst_default_primitive_table[167].name = "VMpr_SystemDictionary_setTraceFlag";
  _gst_default_primitive_table[167].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[167].id = 0;
  _gst_default_primitive_table[167].func = VMpr_SystemDictionary_setTraceFlag;
  _gst_default_primitive_table[168].name = "VMpr_Memory_at";
  _gst_default_primitive_table[168].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[168].id = 0;
  _gst_default_primitive_table[168].func = VMpr_Memory_at;
  _gst_default_primitive_table[169].name = "VMpr_Memory_atPut";
  _gst_default_primitive_table[169].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[169].id = 0;
  _gst_default_primitive_table[169].func = VMpr_Memory_atPut;
  _gst_default_primitive_table[170].name = "VMpr_Behavior_methodsFor";
  _gst_default_primitive_table[170].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[170].id = 0;
  _gst_default_primitive_table[170].func = VMpr_Behavior_methodsFor;
  _gst_default_primitive_table[171].name = "VMpr_Behavior_methodsForIfTrue";
  _gst_default_primitive_table[171].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[171].id = 0;
  _gst_default_primitive_table[171].func = VMpr_Behavior_methodsForIfTrue;
  _gst_default_primitive_table[172].name = "VMpr_Processor_disableInterrupts";
  _gst_default_primitive_table[172].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[172].id = 0;
  _gst_default_primitive_table[172].func = VMpr_Processor_disableEnableInterrupts;
  _gst_default_primitive_table[173].name = "VMpr_Processor_enableInterrupts";
  _gst_default_primitive_table[173].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[173].id = -1;
  _gst_default_primitive_table[173].func = VMpr_Processor_disableEnableInterrupts;
  _gst_default_primitive_table[174].name = "VMpr_Processor_signalOnInterrupt";
  _gst_default_primitive_table[174].attributes = PRIM_SUCCEED | PRIM_FAIL | PRIM_CHECK_INTERRUPT;
  _gst_default_primitive_table[174].id = 0;
  _gst_default_primitive_table[174].func = VMpr_Processor_signalOnInterrupt;
  _gst_default_primitive_table[175].name = "VMpr_ObjectMemory_getSpaceGrowRate";
  _gst_default_primitive_table[175].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[175].id = 0;
  _gst_default_primitive_table[175].func = VMpr_ObjectMemory_getSpaceGrowRate;
  _gst_default_primitive_table[176].name = "VMpr_ObjectMemory_setSpaceGrowRate";
  _gst_default_primitive_table[176].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[176].id = 0;
  _gst_default_primitive_table[176].func = VMpr_ObjectMemory_setSpaceGrowRate;
  _gst_default_primitive_table[177].name = "VMpr_ObjectMemory_getSmoothingFactor";
  _gst_default_primitive_table[177].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[177].id = 0;
  _gst_default_primitive_table[177].func = VMpr_ObjectMemory_getSmoothingFactor;
  _gst_default_primitive_table[178].name = "VMpr_ObjectMemory_setSmoothingFactor";
  _gst_default_primitive_table[178].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[178].id = 0;
  _gst_default_primitive_table[178].func = VMpr_ObjectMemory_setSmoothingFactor;
  _gst_default_primitive_table[179].name = "VMpr_ObjectMemory_getGrowThresholdPercent";
  _gst_default_primitive_table[179].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[179].id = 0;
  _gst_default_primitive_table[179].func = VMpr_ObjectMemory_getGrowThresholdPercent;
  _gst_default_primitive_table[180].name = "VMpr_ObjectMemory_setGrowThresholdPercent";
  _gst_default_primitive_table[180].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[180].id = 0;
  _gst_default_primitive_table[180].func = VMpr_ObjectMemory_setGrowThresholdPercent;
  _gst_default_primitive_table[181].name = "VMpr_ObjectMemory_getBigObjectThreshold";
  _gst_default_primitive_table[181].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[181].id = 0;
  _gst_default_primitive_table[181].func = VMpr_ObjectMemory_getBigObjectThreshold;
  _gst_default_primitive_table[182].name = "VMpr_ObjectMemory_setBigObjectThreshold";
  _gst_default_primitive_table[182].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[182].id = 0;
  _gst_default_primitive_table[182].func = VMpr_ObjectMemory_setBigObjectThreshold;
  _gst_default_primitive_table[183].name = "VMpr_ObjectMemory_growTo";
  _gst_default_primitive_table[183].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[183].id = 0;
  _gst_default_primitive_table[183].func = VMpr_ObjectMemory_growTo;
  _gst_default_primitive_table[184].name = "VMpr_ObjectMemory_update";
  _gst_default_primitive_table[184].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[184].id = 0;
  _gst_default_primitive_table[184].func = VMpr_ObjectMemory_update;
  _gst_default_primitive_table[185].name = "VMpr_CObject_allocType";
  _gst_default_primitive_table[185].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[185].id = 0;
  _gst_default_primitive_table[185].func = VMpr_CObject_allocType;
  _gst_default_primitive_table[186].name = "VMpr_Float_sin";
  _gst_default_primitive_table[186].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[186].id = 0;
  _gst_default_primitive_table[186].func = VMpr_Float_sin;
  _gst_default_primitive_table[187].name = "VMpr_Float_cos";
  _gst_default_primitive_table[187].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[187].id = 0;
  _gst_default_primitive_table[187].func = VMpr_Float_cos;
  _gst_default_primitive_table[188].name = "VMpr_Float_tan";
  _gst_default_primitive_table[188].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[188].id = 0;
  _gst_default_primitive_table[188].func = VMpr_Float_tan;
  _gst_default_primitive_table[189].name = "VMpr_Float_arcSin";
  _gst_default_primitive_table[189].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[189].id = 0;
  _gst_default_primitive_table[189].func = VMpr_Float_arcSin;
  _gst_default_primitive_table[190].name = "VMpr_Float_arcCos";
  _gst_default_primitive_table[190].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[190].id = 0;
  _gst_default_primitive_table[190].func = VMpr_Float_arcCos;
  _gst_default_primitive_table[191].name = "VMpr_Float_arcTan";
  _gst_default_primitive_table[191].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[191].id = 0;
  _gst_default_primitive_table[191].func = VMpr_Float_arcTan;
  _gst_default_primitive_table[192].name = "VMpr_Float_exp";
  _gst_default_primitive_table[192].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[192].id = 0;
  _gst_default_primitive_table[192].func = VMpr_Float_exp;
  _gst_default_primitive_table[193].name = "VMpr_Float_ln";
  _gst_default_primitive_table[193].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[193].id = 0;
  _gst_default_primitive_table[193].func = VMpr_Float_ln;
  _gst_default_primitive_table[194].name = "VMpr_Float_pow";
  _gst_default_primitive_table[194].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[194].id = 0;
  _gst_default_primitive_table[194].func = VMpr_Float_pow;
  _gst_default_primitive_table[195].name = "VMpr_CObject_free";
  _gst_default_primitive_table[195].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[195].id = 0;
  _gst_default_primitive_table[195].func = VMpr_CObject_free;
  _gst_default_primitive_table[196].name = "VMpr_Float_sqrt";
  _gst_default_primitive_table[196].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[196].id = 0;
  _gst_default_primitive_table[196].func = VMpr_Float_sqrt;
  _gst_default_primitive_table[197].name = "VMpr_Float_ceil";
  _gst_default_primitive_table[197].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[197].id = 0;
  _gst_default_primitive_table[197].func = VMpr_Float_ceil_floor;
  _gst_default_primitive_table[198].name = "VMpr_Float_floor";
  _gst_default_primitive_table[198].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[198].id = -1;
  _gst_default_primitive_table[198].func = VMpr_Float_ceil_floor;
  _gst_default_primitive_table[199].name = "VMpr_Behavior_basicNewFixed";
  _gst_default_primitive_table[199].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[199].id = 0;
  _gst_default_primitive_table[199].func = VMpr_Behavior_basicNewFixed;
  _gst_default_primitive_table[200].name = "VMpr_Behavior_basicNewFixedColon";
  _gst_default_primitive_table[200].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[200].id = 0;
  _gst_default_primitive_table[200].func = VMpr_Behavior_basicNewFixedColon;
  _gst_default_primitive_table[201].name = "VMpr_Object_tenure";
  _gst_default_primitive_table[201].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[201].id = 0;
  _gst_default_primitive_table[201].func = VMpr_Object_tenure;
  _gst_default_primitive_table[202].name = "VMpr_Object_makeFixed";
  _gst_default_primitive_table[202].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[202].id = 0;
  _gst_default_primitive_table[202].func = VMpr_Object_makeFixed;
  _gst_default_primitive_table[203].name = "VMpr_CObject_derefAt";
  _gst_default_primitive_table[203].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[203].id = 0;
  _gst_default_primitive_table[203].func = VMpr_CObject_at;
  _gst_default_primitive_table[204].name = "VMpr_CObject_at";
  _gst_default_primitive_table[204].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[204].id = -1;
  _gst_default_primitive_table[204].func = VMpr_CObject_at;
  _gst_default_primitive_table[205].name = "VMpr_CObject_atPut";
  _gst_default_primitive_table[205].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[205].id = 0;
  _gst_default_primitive_table[205].func = VMpr_CObject_atPut;
  _gst_default_primitive_table[206].name = "VMpr_CObject_address";
  _gst_default_primitive_table[206].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[206].id = 0;
  _gst_default_primitive_table[206].func = VMpr_CObject_address;
  _gst_default_primitive_table[207].name = "VMpr_CObject_addressColon";
  _gst_default_primitive_table[207].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[207].id = 0;
  _gst_default_primitive_table[207].func = VMpr_CObject_addressColon;
  _gst_default_primitive_table[208].name = "VMpr_CString_replaceWith";
  _gst_default_primitive_table[208].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[208].id = 0;
  _gst_default_primitive_table[208].func = VMpr_CString_replaceWith;
  _gst_default_primitive_table[209].name = "VMpr_ByteArray_fromCData_size";
  _gst_default_primitive_table[209].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[209].id = 0;
  _gst_default_primitive_table[209].func = VMpr_ByteArray_fromCData_size;
  _gst_default_primitive_table[210].name = "VMpr_String_fromCData_size";
  _gst_default_primitive_table[210].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[210].id = 0;
  _gst_default_primitive_table[210].func = VMpr_String_fromCData_size;
  _gst_default_primitive_table[211].name = "VMpr_String_fromCData";
  _gst_default_primitive_table[211].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[211].id = 0;
  _gst_default_primitive_table[211].func = VMpr_String_fromCData;
  _gst_default_primitive_table[212].name = "VMpr_String_asCData";
  _gst_default_primitive_table[212].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[212].id = 0;
  _gst_default_primitive_table[212].func = VMpr_String_ByteArray_asCData;
  _gst_default_primitive_table[213].name = "VMpr_ByteArray_asCData";
  _gst_default_primitive_table[213].attributes = PRIM_CHECKS_RECEIVER;
  _gst_default_primitive_table[213].id = -1;
  _gst_default_primitive_table[213].func = VMpr_String_ByteArray_asCData;
  _gst_default_primitive_table[214].name = "VMpr_SystemDictionary_byteCodeCounter";
  _gst_default_primitive_table[214].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[214].id = 0;
  _gst_default_primitive_table[214].func = VMpr_SystemDictionary_byteCodeCounter;
  _gst_default_primitive_table[215].name = "VMpr_SystemDictionary_debug";
  _gst_default_primitive_table[215].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[215].id = 0;
  _gst_default_primitive_table[215].func = VMpr_SystemDictionary_debug;
  _gst_default_primitive_table[216].name = "VMpr_Object_isUntrusted";
  _gst_default_primitive_table[216].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[216].id = 0;
  _gst_default_primitive_table[216].func = VMpr_Object_isUntrusted;
  _gst_default_primitive_table[217].name = "VMpr_Object_makeUntrusted";
  _gst_default_primitive_table[217].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[217].id = 0;
  _gst_default_primitive_table[217].func = VMpr_Object_makeUntrusted;
  _gst_default_primitive_table[218].name = "VMpr_Object_isReadOnly";
  _gst_default_primitive_table[218].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[218].id = 0;
  _gst_default_primitive_table[218].func = VMpr_Object_isReadOnly;
  _gst_default_primitive_table[219].name = "VMpr_Object_makeReadOnly";
  _gst_default_primitive_table[219].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[219].id = 0;
  _gst_default_primitive_table[219].func = VMpr_Object_makeReadOnly;
  _gst_default_primitive_table[220].name = "VMpr_Behavior_primCompile";
  _gst_default_primitive_table[220].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[220].id = 0;
  _gst_default_primitive_table[220].func = VMpr_Behavior_primCompile;
  _gst_default_primitive_table[221].name = "VMpr_Behavior_primCompileIfError";
  _gst_default_primitive_table[221].attributes = PRIM_FAIL | PRIM_SUCCEED | PRIM_RELOAD_IP;
  _gst_default_primitive_table[221].id = 0;
  _gst_default_primitive_table[221].func = VMpr_Behavior_primCompileIfError;
  _gst_default_primitive_table[222].name = "VMpr_CCallbackDescriptor_link";
  _gst_default_primitive_table[222].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[222].id = 0;
  _gst_default_primitive_table[222].func = VMpr_CCallbackDescriptor_link;
  _gst_default_primitive_table[223].name = "VMpr_CFuncDescriptor_addressOf";
  _gst_default_primitive_table[223].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[223].id = 0;
  _gst_default_primitive_table[223].func = VMpr_CFuncDescriptor_addressOf;
  _gst_default_primitive_table[224].name = "VMpr_ObjectMemory_snapshot";
  _gst_default_primitive_table[224].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[224].id = 0;
  _gst_default_primitive_table[224].func = VMpr_ObjectMemory_snapshot;
  _gst_default_primitive_table[225].name = "VMpr_Object_basicPrint";
  _gst_default_primitive_table[225].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[225].id = 0;
  _gst_default_primitive_table[225].func = VMpr_Object_basicPrint;
  _gst_default_primitive_table[226].name = "VMpr_Object_makeWeak";
  _gst_default_primitive_table[226].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[226].id = 0;
  _gst_default_primitive_table[226].func = VMpr_Object_makeWeak;
  _gst_default_primitive_table[227].name = "VMpr_Stream_fileInLine";
  _gst_default_primitive_table[227].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[227].id = 0;
  _gst_default_primitive_table[227].func = VMpr_Stream_fileInLine;
  _gst_default_primitive_table[228].name = "VMpr_FileDescriptor_fileOp";
  _gst_default_primitive_table[228].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[228].id = 0;
  _gst_default_primitive_table[228].func = VMpr_FileDescriptor_fileOp;
  _gst_default_primitive_table[229].name = "VMpr_FileDescriptor_socketOp";
  _gst_default_primitive_table[229].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[229].id = 0;
  _gst_default_primitive_table[229].func = VMpr_FileDescriptor_socketOp;
  _gst_default_primitive_table[230].name = "VMpr_CFuncDescriptor_asyncCall";
  _gst_default_primitive_table[230].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[230].id = 0;
  _gst_default_primitive_table[230].func = VMpr_CFuncDescriptor_asyncCall;
  _gst_default_primitive_table[231].name = "VMpr_CFuncDescriptor_call";
  _gst_default_primitive_table[231].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[231].id = 0;
  _gst_default_primitive_table[231].func = VMpr_CFuncDescriptor_call;
  _gst_default_primitive_table[232].name = "VMpr_Object_makeEphemeron";
  _gst_default_primitive_table[232].attributes = PRIM_SUCCEED | PRIM_FAIL;
  _gst_default_primitive_table[232].id = 0;
  _gst_default_primitive_table[232].func = VMpr_Object_makeEphemeron;
  _gst_default_primitive_table[233].name = "VMpr_Namespace_setCurrent";
  _gst_default_primitive_table[233].attributes = PRIM_FAIL;
  _gst_default_primitive_table[233].id = 0;
  _gst_default_primitive_table[233].func = VMpr_Namespace_setCurrent;
  _gst_default_primitive_table[234].name = "VMpr_ObjectMemory_scavenge";
  _gst_default_primitive_table[234].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[234].id = 0;
  _gst_default_primitive_table[234].func = VMpr_ObjectMemory_gcPrimitives;
  _gst_default_primitive_table[235].name = "VMpr_ObjectMemory_compact";
  _gst_default_primitive_table[235].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[235].id = -1;
  _gst_default_primitive_table[235].func = VMpr_ObjectMemory_gcPrimitives;
  _gst_default_primitive_table[236].name = "VMpr_ObjectMemory_globalGarbageCollect";
  _gst_default_primitive_table[236].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[236].id = -2;
  _gst_default_primitive_table[236].func = VMpr_ObjectMemory_gcPrimitives;
  _gst_default_primitive_table[237].name = "VMpr_ObjectMemory_incrementalGCStep";
  _gst_default_primitive_table[237].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[237].id = -3;
  _gst_default_primitive_table[237].func = VMpr_ObjectMemory_gcPrimitives;
  _gst_default_primitive_table[238].name = "VMpr_ObjectMemory_finishIncrementalGC";
  _gst_default_primitive_table[238].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[238].id = -4;
  _gst_default_primitive_table[238].func = VMpr_ObjectMemory_gcPrimitives;
  _gst_default_primitive_table[239].name = "VMpr_SystemDictionary_rawProfile";
  _gst_default_primitive_table[239].attributes = PRIM_SUCCEED;
  _gst_default_primitive_table[239].id = 0;
  _gst_default_primitive_table[239].func = VMpr_SystemDictionary_rawProfile;

  for (i = 240; i < NUM_PRIMITIVES; i++)
    {
      _gst_default_primitive_table[i].name = NULL;
      _gst_default_primitive_table[i].attributes = PRIM_FAIL;
      _gst_default_primitive_table[i].id = i;
      _gst_default_primitive_table[i].func = VMpr_HOLE;
    }
}

