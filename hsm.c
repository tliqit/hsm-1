//
//  hsm.c
//  hsm
//
//  Created by rego on 2/23/17.
//  Copyright © 2017 admin. All rights reserved.
//


#include <stdio.h>
#include <assert.h>
#include "hsm.h"
static QState srcState;         // source state
static QState actState;         // active state
static QEVENT const pkgStdEvt[] =
{
  {Q_EMPTY_SIG, 0, 0},
  {Q_INIT_SIG, 0, 0},
  {Q_ENTRY_SIG, 0, 0},
  {Q_EXIT_SIG, 0, 0}
};

void Q_Initial(QEVENT const* pQevt)
{
  printf("Top_Init;");
}

void Q_Dispatch(QEVENT const* pQevt)
{
  for (srcState = actState; srcState;
       srcState = (QState)(*srcState)(pQevt))
  {}
}
void Init_(QState target)
{
  actState = target;
}

void Q_Init(QSTATE target)
{
  register QState s;

  actState = (QState)target;
  srcState = (QState)Q_Initial;

  s = actState;                               // save actState in a temporary
  (*(QPseudoState)srcState)((QEVENT*)0);      // top-most initial tran.
  // initial transition must go one level deep

  s = actState;                                     // update the temporary
  Q_TRIGGER(s, Q_ENTRY_SIG);           // enter the state
  while (0 == Q_TRIGGER(s, Q_INIT_SIG))
  {
    // init handled
    // initial transition must go one level deep
    s = actState;
    Q_TRIGGER(s, Q_ENTRY_SIG);       // enter the substate
  }
}

void Q_TranDyc(QState target)
{
  QState s,trace1[8],trace2[8],p1,p2;
  size_t i=0,d1=0,d2=0;
  for(s=actState;s;s=Q_TRIGGER(p1,Q_EMPTY_SIG))
  {
    p1=s;
    trace1[d1++]=s;
  }
  for(s=target;s;s=Q_TRIGGER(p2,Q_EMPTY_SIG))
  {
    p2=s;
    trace2[d2++]=s;
  }
  if(p1==p2)
  {
    if(d1>d2)
    {
      for(i=d1;i>=d2;i--)
      {
        Q_TRIGGER(trace1[i-1],Q_ENTRY_SIG);
      }
    }
    else
    {
      for(i=d1;i<=d2;i++)
      {
        Q_TRIGGER(trace2[i-1],Q_ENTRY_SIG);
      }
    }
  }
  else
  {
    for(i=0;i<d1;i++)
    {
      Q_TRIGGER(trace1[i],Q_EXIT_SIG);
    }
    for(i=d2;i;i--)
    {
      Q_TRIGGER(trace2[i-1],Q_ENTRY_SIG);
    }
  }
}
