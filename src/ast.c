/* A recursive-descent parser generated by peg 0.1.18 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYRULECOUNT 21
#line 1 "ast.leg"

#include "ast.h"

#define YYSTYPE struct ast_node *
struct ast_node * last;

struct ast_node * mkstmtvar(struct ast_node * name, struct ast_node * expr) {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = STMTASSIGN;
    node->x.stmtassign.name = name->x.atomload.name;
    node->x.stmtassign.expr = expr;
    last = node;
    free(name);
    return node;
}

struct ast_node * mklam(struct ast_node * args, struct ast_node * expr) {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = LAM;
    node->x.lam.name = "<anonymous>";
    node->x.lam.args = args;
    node->x.lam.expr = expr;
    last = node;
    return node;
}

struct ast_node * mkapp(struct ast_node * f, struct ast_node * x) {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = APP;
    node->x.app.f = f;
    node->x.app.x = x;
    last = node;
    return node;
}

struct ast_node * mkatomload(char * x) {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = ATOMLOAD;
    node->x.atomload.name = strdup(x);
    last = node;
    return node;
}

struct ast_node * mkatomint(char * x) {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = ATOMINT;
    node->x.atomint.value = strdup(x);
    last = node;
    return node;
}

struct ast_node * mkstmtfun(struct ast_node * name, struct ast_node * args, struct ast_node * expr) {
    struct ast_node * f = mklam(args, expr);
    f->x.lam.name = name->x.atomload.name;
    return mkstmtvar(name, f);
}

struct ast_node * mkbinop(struct ast_node * op, struct ast_node * lhs, struct ast_node * rhs) {
    struct ast_node * f = op;
    struct ast_node * x = mkapp(f, lhs);
    struct ast_node * y = mkapp(x, rhs);
    return y;
}

struct ast_node * mknamelist() {
    struct ast_node * node = malloc(sizeof(struct ast_node));
    node->type = NAMELIST;
    node->x.namelist.nnames = 0;
    node->x.namelist.names = 0;
    last = node;
    return node;
}

struct ast_node * addnamelist(struct ast_node * l, struct ast_node * x) {
    l->x.namelist.nnames += 1;
    l->x.namelist.names = realloc(l->x.namelist.names, l->x.namelist.nnames * sizeof(char *));
    l->x.namelist.names[l->x.namelist.nnames-1] = x->x.atomload.name;
    return l;
}

struct ast_node * mkstmtbinop(struct ast_node * op, struct ast_node * lhs, struct ast_node * rhs, struct ast_node * e) {
    struct ast_node * args = addnamelist(addnamelist(mknamelist(), lhs), rhs);
    return mkstmtfun(op, args, e);
}

void print_node(struct ast_node * node) {
    switch (node->type) {
        case STMTASSIGN:
            printf("(assign %s ", node->x.stmtassign.name);
            print_node(node->x.stmtassign.expr);
            printf(")");
            break;
        case LAM:
            printf("(lam %s ", node->x.lam.name);
            struct ast_node * namelist = node->x.lam.args;
            for (int i = 0; i < namelist->x.namelist.nnames; i++) {
                printf("%s ", namelist->x.namelist.names[i]);
            }
            print_node(node->x.lam.expr);
            printf(")");
            break;
        case APP:
            printf("(app ");
            print_node(node->x.app.f);
            printf(" ");
            print_node(node->x.app.x);
            printf(")");
            break;
        case ATOMLOAD:
            printf("(load %s)", node->x.atomload.name);
            break;
        case ATOMINT:
            printf("(load %s)", node->x.atomint.value);
            break;
        case NAMELIST:
            for (int i = 0; i < node->x.namelist.nnames; i++) {
                printf("%s ", node->x.namelist.names[i]);
            }
            break;
    }
}

#ifndef YY_MALLOC
#define YY_MALLOC(C, N)		malloc(N)
#endif
#ifndef YY_REALLOC
#define YY_REALLOC(C, P, N)	realloc(P, N)
#endif
#ifndef YY_FREE
#define YY_FREE(C, P)		free(P)
#endif
#ifndef YY_LOCAL
#define YY_LOCAL(T)	static T
#endif
#ifndef YY_ACTION
#define YY_ACTION(T)	static T
#endif
#ifndef YY_RULE
#define YY_RULE(T)	static T
#endif
#ifndef YY_PARSE
#define YY_PARSE(T)	T
#endif
#ifndef YYPARSE
#define YYPARSE		yyparse
#endif
#ifndef YYPARSEFROM
#define YYPARSEFROM	yyparsefrom
#endif
#ifndef YYRELEASE
#define YYRELEASE	yyrelease
#endif
#ifndef YY_BEGIN
#define YY_BEGIN	( yy->__begin= yy->__pos, 1)
#endif
#ifndef YY_END
#define YY_END		( yy->__end= yy->__pos, 1)
#endif
#ifdef YY_DEBUG
# define yyprintf(args)	fprintf args
#else
# define yyprintf(args)
#endif
#ifndef YYSTYPE
#define YYSTYPE	int
#endif
#ifndef YY_STACK_SIZE
#define YY_STACK_SIZE 128
#endif

#ifndef YY_BUFFER_SIZE
#define YY_BUFFER_SIZE 1024
#endif

#ifndef YY_PART

typedef struct _yycontext yycontext;
typedef void (*yyaction)(yycontext *yy, char *yytext, int yyleng);
typedef struct _yythunk { int begin, end;  yyaction  action;  struct _yythunk *next; } yythunk;

struct _yycontext {
  char     *__buf;
  int       __buflen;
  int       __pos;
  int       __limit;
  char     *__text;
  int       __textlen;
  int       __begin;
  int       __end;
  int       __textmax;
  yythunk  *__thunks;
  int       __thunkslen;
  int       __thunkpos;
  YYSTYPE   __;
  YYSTYPE  *__val;
  YYSTYPE  *__vals;
  int       __valslen;
#ifdef YY_CTX_MEMBERS
  YY_CTX_MEMBERS
#endif
};

#ifdef YY_CTX_LOCAL
#define YY_CTX_PARAM_	yycontext *yyctx,
#define YY_CTX_PARAM	yycontext *yyctx
#define YY_CTX_ARG_	yyctx,
#define YY_CTX_ARG	yyctx
#ifndef YY_INPUT
#define YY_INPUT(yy, buf, result, max_size)		\
  {							\
    int yyc= getchar();					\
    result= (EOF == yyc) ? 0 : (*(buf)= yyc, 1);	\
    yyprintf((stderr, "<%c>", yyc));			\
  }
#endif
#else
#define YY_CTX_PARAM_
#define YY_CTX_PARAM
#define YY_CTX_ARG_
#define YY_CTX_ARG
yycontext _yyctx= { 0, 0 };
yycontext *yyctx= &_yyctx;
#ifndef YY_INPUT
#define YY_INPUT(buf, result, max_size)			\
  {							\
    int yyc= getchar();					\
    result= (EOF == yyc) ? 0 : (*(buf)= yyc, 1);	\
    yyprintf((stderr, "<%c>", yyc));			\
  }
#endif
#endif

YY_LOCAL(int) yyrefill(yycontext *yy)
{
  int yyn;
  while (yy->__buflen - yy->__pos < 512)
    {
      yy->__buflen *= 2;
      yy->__buf= (char *)YY_REALLOC(yy, yy->__buf, yy->__buflen);
    }
#ifdef YY_CTX_LOCAL
  YY_INPUT(yy, (yy->__buf + yy->__pos), yyn, (yy->__buflen - yy->__pos));
#else
  YY_INPUT((yy->__buf + yy->__pos), yyn, (yy->__buflen - yy->__pos));
#endif
  if (!yyn) return 0;
  yy->__limit += yyn;
  return 1;
}

YY_LOCAL(int) yymatchDot(yycontext *yy)
{
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  ++yy->__pos;
  return 1;
}

YY_LOCAL(int) yymatchChar(yycontext *yy, int c)
{
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  if ((unsigned char)yy->__buf[yy->__pos] == c)
    {
      ++yy->__pos;
      yyprintf((stderr, "  ok   yymatchChar(yy, %c) @ %s\n", c, yy->__buf+yy->__pos));
      return 1;
    }
  yyprintf((stderr, "  fail yymatchChar(yy, %c) @ %s\n", c, yy->__buf+yy->__pos));
  return 0;
}

YY_LOCAL(int) yymatchString(yycontext *yy, const char *s)
{
  int yysav= yy->__pos;
  while (*s)
    {
      if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
      if (yy->__buf[yy->__pos] != *s)
        {
          yy->__pos= yysav;
          return 0;
        }
      ++s;
      ++yy->__pos;
    }
  return 1;
}

YY_LOCAL(int) yymatchClass(yycontext *yy, unsigned char *bits)
{
  int c;
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  c= (unsigned char)yy->__buf[yy->__pos];
  if (bits[c >> 3] & (1 << (c & 7)))
    {
      ++yy->__pos;
      yyprintf((stderr, "  ok   yymatchClass @ %s\n", yy->__buf+yy->__pos));
      return 1;
    }
  yyprintf((stderr, "  fail yymatchClass @ %s\n", yy->__buf+yy->__pos));
  return 0;
}

YY_LOCAL(void) yyDo(yycontext *yy, yyaction action, int begin, int end)
{
  while (yy->__thunkpos >= yy->__thunkslen)
    {
      yy->__thunkslen *= 2;
      yy->__thunks= (yythunk *)YY_REALLOC(yy, yy->__thunks, sizeof(yythunk) * yy->__thunkslen);
    }
  yy->__thunks[yy->__thunkpos].begin=  begin;
  yy->__thunks[yy->__thunkpos].end=    end;
  yy->__thunks[yy->__thunkpos].action= action;
  ++yy->__thunkpos;
}

YY_LOCAL(int) yyText(yycontext *yy, int begin, int end)
{
  int yyleng= end - begin;
  if (yyleng <= 0)
    yyleng= 0;
  else
    {
      while (yy->__textlen < (yyleng + 1))
	{
	  yy->__textlen *= 2;
	  yy->__text= (char *)YY_REALLOC(yy, yy->__text, yy->__textlen);
	}
      memcpy(yy->__text, yy->__buf + begin, yyleng);
    }
  yy->__text[yyleng]= '\0';
  return yyleng;
}

YY_LOCAL(void) yyDone(yycontext *yy)
{
  int pos;
  for (pos= 0;  pos < yy->__thunkpos;  ++pos)
    {
      yythunk *thunk= &yy->__thunks[pos];
      int yyleng= thunk->end ? yyText(yy, thunk->begin, thunk->end) : thunk->begin;
      yyprintf((stderr, "DO [%d] %p %s\n", pos, thunk->action, yy->__text));
      thunk->action(yy, yy->__text, yyleng);
    }
  yy->__thunkpos= 0;
}

YY_LOCAL(void) yyCommit(yycontext *yy)
{
  if ((yy->__limit -= yy->__pos))
    {
      memmove(yy->__buf, yy->__buf + yy->__pos, yy->__limit);
    }
  yy->__begin -= yy->__pos;
  yy->__end -= yy->__pos;
  yy->__pos= yy->__thunkpos= 0;
}

YY_LOCAL(int) yyAccept(yycontext *yy, int tp0)
{
  if (tp0)
    {
      fprintf(stderr, "accept denied at %d\n", tp0);
      return 0;
    }
  else
    {
      yyDone(yy);
      yyCommit(yy);
    }
  return 1;
}

YY_LOCAL(void) yyPush(yycontext *yy, char *text, int count)
{
  yy->__val += count;
  while (yy->__valslen <= yy->__val - yy->__vals)
    {
      long offset= yy->__val - yy->__vals;
      yy->__valslen *= 2;
      yy->__vals= (YYSTYPE *)YY_REALLOC(yy, yy->__vals, sizeof(YYSTYPE) * yy->__valslen);
      yy->__val= yy->__vals + offset;
    }
}
YY_LOCAL(void) yyPop(yycontext *yy, char *text, int count)   { yy->__val -= count; }
YY_LOCAL(void) yySet(yycontext *yy, char *text, int count)   { yy->__val[count]= yy->__; }

#endif /* YY_PART */

#define	YYACCEPT	yyAccept(yy, yythunkpos0)

YY_RULE(int) yy_SPACE(yycontext *yy); /* 21 */
YY_RULE(int) yy_NUMBER(yycontext *yy); /* 20 */
YY_RULE(int) yy_CLOSE(yycontext *yy); /* 19 */
YY_RULE(int) yy_OPEN(yycontext *yy); /* 18 */
YY_RULE(int) yy_AtomLoad(yycontext *yy); /* 17 */
YY_RULE(int) yy_AtomInt(yycontext *yy); /* 16 */
YY_RULE(int) yy_TO(yycontext *yy); /* 15 */
YY_RULE(int) yy_Term(yycontext *yy); /* 14 */
YY_RULE(int) yy_BinOp(yycontext *yy); /* 13 */
YY_RULE(int) yy_Lam(yycontext *yy); /* 12 */
YY_RULE(int) yy_App(yycontext *yy); /* 11 */
YY_RULE(int) yy_OP(yycontext *yy); /* 10 */
YY_RULE(int) yy_Arg(yycontext *yy); /* 9 */
YY_RULE(int) yy_Expr(yycontext *yy); /* 8 */
YY_RULE(int) yy_ASSIGN(yycontext *yy); /* 7 */
YY_RULE(int) yy_NAME(yycontext *yy); /* 6 */
YY_RULE(int) yy_EOL(yycontext *yy); /* 5 */
YY_RULE(int) yy_StmtFun(yycontext *yy); /* 4 */
YY_RULE(int) yy_StmtVar(yycontext *yy); /* 3 */
YY_RULE(int) yy_StmtBinOp(yycontext *yy); /* 2 */
YY_RULE(int) yy_Stmt(yycontext *yy); /* 1 */

YY_ACTION(void) yy_1_NAME(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_NAME\n"));
  {
#line 150
   __ = mkatomload(yytext); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_1_OP(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_OP\n"));
  {
#line 149
   __ = mkatomload(yytext); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_1_NUMBER(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_NUMBER\n"));
  {
#line 148
   __ = mkatomint(yytext); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_1_AtomInt(yycontext *yy, char *yytext, int yyleng)
{
#define n yy->__val[-1]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_AtomInt\n"));
  {
#line 144
   __ = n; ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef n
}
YY_ACTION(void) yy_1_AtomLoad(yycontext *yy, char *yytext, int yyleng)
{
#define n yy->__val[-1]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_AtomLoad\n"));
  {
#line 143
   __ = n; ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef n
}
YY_ACTION(void) yy_1_App(yycontext *yy, char *yytext, int yyleng)
{
#define x yy->__val[-1]
#define f yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_App\n"));
  {
#line 139
   __ = mkapp(f, x); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef x
#undef f
}
YY_ACTION(void) yy_1_BinOp(yycontext *yy, char *yytext, int yyleng)
{
#define rhs yy->__val[-1]
#define op yy->__val[-2]
#define lhs yy->__val[-3]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_BinOp\n"));
  {
#line 138
   __ = mkbinop(op, lhs, rhs); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef rhs
#undef op
#undef lhs
}
YY_ACTION(void) yy_3_Arg(yycontext *yy, char *yytext, int yyleng)
{
#define next yy->__val[-1]
#define args yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_3_Arg\n"));
  {
#line 137
   __ = args; ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef next
#undef args
}
YY_ACTION(void) yy_2_Arg(yycontext *yy, char *yytext, int yyleng)
{
#define next yy->__val[-1]
#define args yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_2_Arg\n"));
  {
#line 136
   __ = addnamelist(args, next); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef next
#undef args
}
YY_ACTION(void) yy_1_Arg(yycontext *yy, char *yytext, int yyleng)
{
#define next yy->__val[-1]
#define args yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Arg\n"));
  {
#line 135
   args = addnamelist(mknamelist(), args); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef next
#undef args
}
YY_ACTION(void) yy_1_Lam(yycontext *yy, char *yytext, int yyleng)
{
#define e yy->__val[-1]
#define a yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Lam\n"));
  {
#line 134
   __ = mklam(a, e); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef e
#undef a
}
YY_ACTION(void) yy_1_StmtBinOp(yycontext *yy, char *yytext, int yyleng)
{
#define e yy->__val[-1]
#define rhs yy->__val[-2]
#define op yy->__val[-3]
#define lhs yy->__val[-4]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_StmtBinOp\n"));
  {
#line 129
   __ = mkstmtbinop(op, lhs, rhs, e); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef e
#undef rhs
#undef op
#undef lhs
}
YY_ACTION(void) yy_1_StmtFun(yycontext *yy, char *yytext, int yyleng)
{
#define e yy->__val[-1]
#define a yy->__val[-2]
#define n yy->__val[-3]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_StmtFun\n"));
  {
#line 127
   __ = mkstmtfun(n, a, e); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef e
#undef a
#undef n
}
YY_ACTION(void) yy_1_StmtVar(yycontext *yy, char *yytext, int yyleng)
{
#define e yy->__val[-1]
#define n yy->__val[-2]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_StmtVar\n"));
  {
#line 126
   __ = mkstmtvar(n, e); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef e
#undef n
}

YY_RULE(int) yy_SPACE(yycontext *yy)
{
  yyprintf((stderr, "%s\n", "SPACE"));
  l2:;	
  {  int yypos3= yy->__pos, yythunkpos3= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\002\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l3;  goto l2;
  l3:;	  yy->__pos= yypos3; yy->__thunkpos= yythunkpos3;
  }
  yyprintf((stderr, "  ok   %s @ %s\n", "SPACE", yy->__buf+yy->__pos));
  return 1;
}
YY_RULE(int) yy_NUMBER(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "NUMBER"));  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l4;
#undef yytext
#undef yyleng
  }  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\000\377\003\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l4;
  l5:;	
  {  int yypos6= yy->__pos, yythunkpos6= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\000\377\003\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l6;  goto l5;
  l6:;	  yy->__pos= yypos6; yy->__thunkpos= yythunkpos6;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l4;
#undef yytext
#undef yyleng
  }  if (!yy_SPACE(yy)) goto l4;  yyDo(yy, yy_1_NUMBER, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "NUMBER", yy->__buf+yy->__pos));
  return 1;
  l4:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "NUMBER", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_CLOSE(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "CLOSE"));  if (!yymatchChar(yy, ')')) goto l7;  if (!yy_SPACE(yy)) goto l7;
  yyprintf((stderr, "  ok   %s @ %s\n", "CLOSE", yy->__buf+yy->__pos));
  return 1;
  l7:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "CLOSE", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_OPEN(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "OPEN"));  if (!yymatchChar(yy, '(')) goto l8;  if (!yy_SPACE(yy)) goto l8;
  yyprintf((stderr, "  ok   %s @ %s\n", "OPEN", yy->__buf+yy->__pos));
  return 1;
  l8:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "OPEN", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_AtomLoad(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 1, 0);
  yyprintf((stderr, "%s\n", "AtomLoad"));
  {  int yypos10= yy->__pos, yythunkpos10= yy->__thunkpos;  if (!yy_Arg(yy)) goto l10;  if (!yy_TO(yy)) goto l10;  goto l9;
  l10:;	  yy->__pos= yypos10; yy->__thunkpos= yythunkpos10;
  }  if (!yy_NAME(yy)) goto l9;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_AtomLoad, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "AtomLoad", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 1, 0);
  return 1;
  l9:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "AtomLoad", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_AtomInt(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 1, 0);
  yyprintf((stderr, "%s\n", "AtomInt"));  if (!yy_NUMBER(yy)) goto l11;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_AtomInt, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "AtomInt", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 1, 0);
  return 1;
  l11:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "AtomInt", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_TO(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "TO"));  if (!yymatchString(yy, "->")) goto l12;  if (!yy_SPACE(yy)) goto l12;
  yyprintf((stderr, "  ok   %s @ %s\n", "TO", yy->__buf+yy->__pos));
  return 1;
  l12:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "TO", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Term(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Term"));
  {  int yypos14= yy->__pos, yythunkpos14= yy->__thunkpos;  if (!yy_AtomInt(yy)) goto l15;  goto l14;
  l15:;	  yy->__pos= yypos14; yy->__thunkpos= yythunkpos14;  if (!yy_AtomLoad(yy)) goto l16;  goto l14;
  l16:;	  yy->__pos= yypos14; yy->__thunkpos= yythunkpos14;  if (!yy_OPEN(yy)) goto l13;  if (!yy_Expr(yy)) goto l13;  if (!yy_CLOSE(yy)) goto l13;
  }
  l14:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Term", yy->__buf+yy->__pos));
  return 1;
  l13:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Term", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_BinOp(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 3, 0);
  yyprintf((stderr, "%s\n", "BinOp"));  if (!yy_Term(yy)) goto l17;  yyDo(yy, yySet, -3, 0);  if (!yy_OP(yy)) goto l17;  yyDo(yy, yySet, -2, 0);  if (!yy_Expr(yy)) goto l17;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_BinOp, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "BinOp", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 3, 0);
  return 1;
  l17:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "BinOp", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Lam(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 2, 0);
  yyprintf((stderr, "%s\n", "Lam"));  if (!yy_Arg(yy)) goto l18;  yyDo(yy, yySet, -2, 0);  if (!yy_TO(yy)) goto l18;  if (!yy_Expr(yy)) goto l18;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_Lam, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "Lam", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 2, 0);
  return 1;
  l18:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Lam", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_App(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 2, 0);
  yyprintf((stderr, "%s\n", "App"));  if (!yy_Term(yy)) goto l19;  yyDo(yy, yySet, -2, 0);  if (!yy_Expr(yy)) goto l19;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_App, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "App", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 2, 0);
  return 1;
  l19:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "App", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_OP(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "OP"));
  {  int yypos21= yy->__pos, yythunkpos21= yy->__thunkpos;  if (!yy_TO(yy)) goto l21;  goto l20;
  l21:;	  yy->__pos= yypos21; yy->__thunkpos= yythunkpos21;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l20;
#undef yytext
#undef yyleng
  }  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\140\254\000\120\000\000\000\100\000\000\000\020\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l20;
  l22:;	
  {  int yypos23= yy->__pos, yythunkpos23= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\140\254\000\120\000\000\000\100\000\000\000\020\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l23;  goto l22;
  l23:;	  yy->__pos= yypos23; yy->__thunkpos= yythunkpos23;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l20;
#undef yytext
#undef yyleng
  }  if (!yy_SPACE(yy)) goto l20;  yyDo(yy, yy_1_OP, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "OP", yy->__buf+yy->__pos));
  return 1;
  l20:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "OP", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Arg(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 2, 0);
  yyprintf((stderr, "%s\n", "Arg"));  if (!yy_NAME(yy)) goto l24;  yyDo(yy, yySet, -2, 0);  yyDo(yy, yy_1_Arg, yy->__begin, yy->__end);
  l25:;	
  {  int yypos26= yy->__pos, yythunkpos26= yy->__thunkpos;  if (!yy_NAME(yy)) goto l26;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_2_Arg, yy->__begin, yy->__end);  goto l25;
  l26:;	  yy->__pos= yypos26; yy->__thunkpos= yythunkpos26;
  }  yyDo(yy, yy_3_Arg, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "Arg", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 2, 0);
  return 1;
  l24:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Arg", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Expr(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Expr"));
  {  int yypos28= yy->__pos, yythunkpos28= yy->__thunkpos;  if (!yy_App(yy)) goto l29;  goto l28;
  l29:;	  yy->__pos= yypos28; yy->__thunkpos= yythunkpos28;  if (!yy_Lam(yy)) goto l30;  goto l28;
  l30:;	  yy->__pos= yypos28; yy->__thunkpos= yythunkpos28;  if (!yy_BinOp(yy)) goto l31;  goto l28;
  l31:;	  yy->__pos= yypos28; yy->__thunkpos= yythunkpos28;  if (!yy_Term(yy)) goto l27;
  }
  l28:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Expr", yy->__buf+yy->__pos));
  return 1;
  l27:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Expr", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_ASSIGN(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "ASSIGN"));  if (!yymatchChar(yy, '=')) goto l32;  if (!yy_SPACE(yy)) goto l32;
  yyprintf((stderr, "  ok   %s @ %s\n", "ASSIGN", yy->__buf+yy->__pos));
  return 1;
  l32:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "ASSIGN", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_NAME(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "NAME"));  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l33;
#undef yytext
#undef yyleng
  }  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\000\000\000\376\377\377\207\376\377\377\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l33;
  l34:;	
  {  int yypos35= yy->__pos, yythunkpos35= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\200\377\003\376\377\377\207\376\377\377\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l35;  goto l34;
  l35:;	  yy->__pos= yypos35; yy->__thunkpos= yythunkpos35;
  }
  l36:;	
  {  int yypos37= yy->__pos, yythunkpos37= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\200\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l37;  goto l36;
  l37:;	  yy->__pos= yypos37; yy->__thunkpos= yythunkpos37;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l33;
#undef yytext
#undef yyleng
  }  if (!yy_SPACE(yy)) goto l33;  yyDo(yy, yy_1_NAME, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "NAME", yy->__buf+yy->__pos));
  return 1;
  l33:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "NAME", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_EOL(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "EOL"));
  {  int yypos39= yy->__pos, yythunkpos39= yy->__thunkpos;  if (!yymatchChar(yy, '\n')) goto l40;  goto l39;
  l40:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchString(yy, "\r\n")) goto l41;  goto l39;
  l41:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, '\r')) goto l38;
  }
  l39:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "EOL", yy->__buf+yy->__pos));
  return 1;
  l38:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "EOL", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_StmtFun(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 3, 0);
  yyprintf((stderr, "%s\n", "StmtFun"));  if (!yy_NAME(yy)) goto l42;  yyDo(yy, yySet, -3, 0);  if (!yy_Arg(yy)) goto l42;  yyDo(yy, yySet, -2, 0);  if (!yy_ASSIGN(yy)) goto l42;  if (!yy_Expr(yy)) goto l42;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_StmtFun, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "StmtFun", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 3, 0);
  return 1;
  l42:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "StmtFun", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_StmtVar(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 2, 0);
  yyprintf((stderr, "%s\n", "StmtVar"));  if (!yy_NAME(yy)) goto l43;  yyDo(yy, yySet, -2, 0);  if (!yy_ASSIGN(yy)) goto l43;  if (!yy_Expr(yy)) goto l43;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_StmtVar, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "StmtVar", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 2, 0);
  return 1;
  l43:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "StmtVar", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_StmtBinOp(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 4, 0);
  yyprintf((stderr, "%s\n", "StmtBinOp"));  if (!yy_NAME(yy)) goto l44;  yyDo(yy, yySet, -4, 0);  if (!yy_OP(yy)) goto l44;  yyDo(yy, yySet, -3, 0);  if (!yy_NAME(yy)) goto l44;  yyDo(yy, yySet, -2, 0);  if (!yy_ASSIGN(yy)) goto l44;  if (!yy_Expr(yy)) goto l44;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_1_StmtBinOp, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "StmtBinOp", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 4, 0);
  return 1;
  l44:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "StmtBinOp", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Stmt(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Stmt"));
  {  int yypos46= yy->__pos, yythunkpos46= yy->__thunkpos;  if (!yy_StmtBinOp(yy)) goto l47;  goto l46;
  l47:;	  yy->__pos= yypos46; yy->__thunkpos= yythunkpos46;  if (!yy_StmtVar(yy)) goto l48;  goto l46;
  l48:;	  yy->__pos= yypos46; yy->__thunkpos= yythunkpos46;  if (!yy_StmtFun(yy)) goto l45;
  }
  l46:;	  if (!yy_EOL(yy)) goto l45;
  yyprintf((stderr, "  ok   %s @ %s\n", "Stmt", yy->__buf+yy->__pos));
  return 1;
  l45:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Stmt", yy->__buf+yy->__pos));
  return 0;
}

#ifndef YY_PART

typedef int (*yyrule)(yycontext *yy);

YY_PARSE(int) YYPARSEFROM(YY_CTX_PARAM_ yyrule yystart)
{
  int yyok;
  if (!yyctx->__buflen)
    {
      yyctx->__buflen= YY_BUFFER_SIZE;
      yyctx->__buf= (char *)YY_MALLOC(yyctx, yyctx->__buflen);
      yyctx->__textlen= YY_BUFFER_SIZE;
      yyctx->__text= (char *)YY_MALLOC(yyctx, yyctx->__textlen);
      yyctx->__thunkslen= YY_STACK_SIZE;
      yyctx->__thunks= (yythunk *)YY_MALLOC(yyctx, sizeof(yythunk) * yyctx->__thunkslen);
      yyctx->__valslen= YY_STACK_SIZE;
      yyctx->__vals= (YYSTYPE *)YY_MALLOC(yyctx, sizeof(YYSTYPE) * yyctx->__valslen);
      yyctx->__begin= yyctx->__end= yyctx->__pos= yyctx->__limit= yyctx->__thunkpos= 0;
    }
  yyctx->__begin= yyctx->__end= yyctx->__pos;
  yyctx->__thunkpos= 0;
  yyctx->__val= yyctx->__vals;
  yyok= yystart(yyctx);
  if (yyok) yyDone(yyctx);
  yyCommit(yyctx);
  return yyok;
}

YY_PARSE(int) YYPARSE(YY_CTX_PARAM)
{
  return YYPARSEFROM(YY_CTX_ARG_ yy_Stmt);
}

YY_PARSE(yycontext *) YYRELEASE(yycontext *yyctx)
{
  if (yyctx->__buflen)
    {
      yyctx->__buflen= 0;
      YY_FREE(yyctx, yyctx->__buf);
      YY_FREE(yyctx, yyctx->__text);
      YY_FREE(yyctx, yyctx->__thunks);
      YY_FREE(yyctx, yyctx->__vals);
    }
  return yyctx;
}

#endif