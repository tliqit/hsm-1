#ifndef STATE_INHERIT_H
#define STATE_INHERIT_H
typedef unsigned short QSIG;
// Define the signal of state machine
enum
{
  Q_EMPTY_SIG = 0,
  Q_INIT_SIG = 1,
  Q_ENTRY_SIG,
  Q_EXIT_SIG,
  Q_USER_SIG
};

// Define the signal of state machine
typedef struct tagQEVENT
{
  QSIG sig;
  unsigned char *pEvent1;
  unsigned char *pEvent2;
  // TODO: add fields to the event
} QEVENT;

// define state data type
typedef void (* QPseudoState)(QEVENT const *pEvent);
typedef QPseudoState (* QState)(QEVENT const *pEvent);
typedef QPseudoState QSTATE;

#define Q_TRIGGER(state, sig) \
   (QState)(*(state))((QEVENT*)&pkgStdEvt[sig])

// define a transation that don't change the state,
// just treat the pEvent with the target state.
// this is used by concurrent state
#define Q_INIT(target_) Init_((QState)(target_));
#define Q_TRAN(target_) Q_TranDyc((QState)(target_));

void Init_(QState target);
void Q_Init(QSTATE target);
void Q_Initial(QEVENT const* pQevt);
void Q_Dispatch(QEVENT const* pQevt);
void Q_TranDyc(QState target);


#endif //STATE_INHERIT_H