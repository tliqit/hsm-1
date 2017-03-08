#include <stdio.h>
#include <stdbool.h>
#include "hsm.h"

QSTATE s0(QEVENT const *e);
QSTATE s1(QEVENT const *e);
QSTATE s2(QEVENT const *e);
QSTATE s11(QEVENT const *e);
QSTATE s21(QEVENT const *e);
QSTATE s211(QEVENT const *e);
static QSTATE Q_Top(QEVENT const *e);
static void Initial(QEVENT const *e);
static bool bFoo;

enum QSignals {
  A_SIG = Q_USER_SIG,
  B_SIG, C_SIG, D_SIG, E_SIG, F_SIG, G_SIG, H_SIG
};

static const QEVENT testQEvt[] =
{
  {A_SIG, 0, 0}, {B_SIG, 0, 0}, {C_SIG, 0, 0}, {D_SIG, 0, 0},
  {E_SIG, 0, 0}, {F_SIG, 0, 0}, {G_SIG, 0, 0}, {H_SIG, 0, 0}
};

int main()
{
  printf("Hiberarchy state machine testing\n");

  Initial(0);       // trigger initial transition
  for (;;)
  {
    char c;
    printf("\nSignal<-");
    c = getc(stdin);
    getc(stdin);              // discard '\n'
    if (c < 'a' || 'h' < c) {
      return 0;
    }
    Q_Dispatch(&testQEvt[c - 'a']); // dispatch
  }
  return 0;
}

static QSTATE Q_Top(QEVENT const *e)
{
  return 0;
}

void Initial(QEVENT const *e)
{
  bFoo = false;
  Q_Init((QSTATE)s0);
}

QSTATE s0(QEVENT const *e) {
  if (e != NULL)
  {
    switch (e->sig)
    {
    case Q_ENTRY_SIG: printf("s0-ENTRY;"); return 0;
    case Q_EXIT_SIG: printf("s0-EXIT;");  return 0;
    case Q_INIT_SIG: printf("s0-INIT;"); Q_INIT(s1); return 0;
    case E_SIG:      printf("s0-E;"); Q_TRAN(s211);  return 0;
    }
  }
  return (QSTATE)Q_Top;
}

QSTATE s1(QEVENT const *e) {
  switch (e->sig) {
  case Q_ENTRY_SIG: printf("s1-ENTRY;"); return 0;
  case Q_EXIT_SIG: printf("s1-EXIT;");   return 0;
  case Q_INIT_SIG: printf("s1-INIT;"); Q_INIT(s11); return 0;
  case A_SIG:      printf("s1-A;");   Q_TRAN(s1);  return 0;
  case B_SIG:      printf("s1-B;");   Q_TRAN(s11); return 0;
  case C_SIG:      printf("s1-C;");   Q_TRAN(s2);  return 0;
  case D_SIG:      printf("s1-D;");   Q_TRAN(s0);  return 0;
  case F_SIG:      printf("s1-F;");   Q_TRAN(s211); return 0;
  }
  return (QSTATE)s0;
}

QSTATE s11(QEVENT const *e) {
  switch (e->sig) {
  case Q_ENTRY_SIG: printf("s11-ENTRY;"); return 0;
  case Q_EXIT_SIG:  printf("s11-EXIT;");  return 0;
  case G_SIG:  printf("s11-G;"); Q_TRAN(s211); return 0;
  case H_SIG:                 // internal transition with a guard
    if (bFoo)
    { // test the guard condition
      printf("s11-H;");
      bFoo = false;
      return 0;
    }
    break;
  }
  return (QSTATE)s1;
}

QSTATE s2( QEVENT const *e) {
  switch (e->sig) {
  case Q_ENTRY_SIG: printf("s2-ENTRY;"); return 0;
  case Q_EXIT_SIG: printf("s2-EXIT;");   return 0;
  case Q_INIT_SIG: printf("s2-INIT;"); Q_INIT(s21); return 0;
  case C_SIG:      printf("s2-C;");   Q_TRAN(s1);  return 0;
  case F_SIG:      printf("s2-F;");   Q_TRAN(s11); return 0;
  }
  return (QSTATE)s0;
}

QSTATE s21(QEVENT const *e) {
  switch (e->sig) {
  case Q_ENTRY_SIG: printf("s21-ENTRY;"); return 0;
  case Q_EXIT_SIG: printf("s21-EXIT;");   return 0;
  case Q_INIT_SIG: printf("s21-INIT;"); Q_INIT(s211); return 0;
  case B_SIG:     printf("s21-C;");   Q_TRAN(s211); return 0;
  case H_SIG:                     // self transition with a guard
    if (!bFoo)
    { // test the guard condition
      printf("s21-H;");
      bFoo = true;
      Q_TRAN(s21);                   // self transition
      return 0;
    }
    break;                     //break to return the superstate
  }
  return (QSTATE)s2;
}

QSTATE s211(QEVENT const *e) {
  switch (e->sig) {
  case Q_ENTRY_SIG: printf("s211-ENTRY;"); return 0;
  case Q_EXIT_SIG:  printf("s211-EXIT;");  return 0;
  case D_SIG: printf("s211-D;"); Q_TRAN(s21); return 0;
  case G_SIG: printf("s211-G;"); Q_TRAN(s0);  return 0;
  }
  return (QSTATE)s21;
}