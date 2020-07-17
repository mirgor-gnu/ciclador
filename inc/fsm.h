//********************************************************************
//
//   Title                : fsm.h
//
//   Description          : This is the header file a FSM implementation
//
//   Author               : Esteban G. Pupillo
//
//   Created              : <24 Mar 2016>
//
//********************************************************************

#ifndef _FSM_H
#define _FSM_H 1
//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

#define ENTRY_SIG    (0)
#define EXIT_SIG     (1)
#define TICK_SIG     (2)
#define USER_SIG     (3)

/* "inlined" methods of Fsm class */
#define FsmCtor(me_, init_) ((me_)->state__ = (State)(init_))
#define FsmInit(me_, e_)     (*(me_)->state__)((Fsm *)(me_), (const Event *)(e_))
#define FsmDispatch(me_, e_) (*(me_)->state__)((Fsm *)(me_), (const Event *)(e_))
#define FsmTran(me_, targ_) do { FsmDispatch(me_,&exitEvt); \
                                  ((me_)->state__ = (State)(targ_)); \
                                  FsmDispatch(me_,&entryEvt); \
                                } while(0)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef uint32_t Signal;
typedef struct Event Event;
typedef struct Fsm Fsm;
typedef void (*State)(Fsm *, Event const *);

/* Event base class */
struct Event
{
   Signal sig;
};

/* Finite State Machine base class */
struct Fsm
{
   State state__; /* the current state */
};

//********************************************************************
// Global Variable extern Declarations
//********************************************************************
static Event const entryEvt = { ENTRY_SIG };
static Event const exitEvt = { EXIT_SIG };
static Event const tickEvt = { TICK_SIG };
//********************************************************************
// Function Prototypes
//********************************************************************


#endif // _FSM_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************
