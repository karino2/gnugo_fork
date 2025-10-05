#include <jni.h>

#include "config.h"

#define BUILDING_GNUGO_ENGINE

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "gnugo.h"
#include "interface.h"
#include "random.h"

void clearstats(void);

/*
  based on gtp.c
*/
static int g_current_id;


/*
  other
*/
static char g_output_line[1024];

static void set_boardsize(int size) { board_size = size; }
static int isBlackToColor(int isBlack) {
  if(isBlack) {
    return BLACK;
  } else {
    return WHITE;
  }
}

void
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_initNative (
	JNIEnv*	env,
	jclass clasz
	)
{
  // based on main.c
  init_gnugo((float)DEFAULT_MEMORY, time(0));
  // 10 is too slow for my device.
  set_level(4);

  // based o play_gtp
  set_boardsize(9);
  init_timers();
  reset_engine();
  clearstats();
}

void
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_setKomi (
	JNIEnv*	env,
	jclass clasz,
	jfloat komi_
	)
{
  komi = komi_;
}


void
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_clearBoard (
	JNIEnv*	env,
	jclass clasz
	)
{
  clear_board();
  init_timers();
}

/*
  x, y is start from upperleft, 0 origin.

  A3 means
  x = 0
  y = BOARD_SIZE-3

  In GnuGo3:
  i = y
  j = x
*/
static jboolean play(int x, int y, jboolean isBlack) {
  int cur_color = isBlackToColor(isBlack);

  if (!is_allowed_move(POS(y, x), cur_color))
    return JNI_FALSE;
  
  gnugo_play_move(POS(y, x), cur_color);
  return JNI_TRUE;
}

jboolean
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_doMove (
	JNIEnv*	env,
	jclass clasz,
  jint x, 
  jint y,
  jboolean isBlack
	)
{
  return play(x, y, isBlack)
;}

void
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_doPass (
	JNIEnv*	env,
	jclass clasz,
  jboolean isBlack
	)
{
  int color = isBlackToColor(isBlack);
  gnugo_play_move(PASS_MOVE, color);
}

/*
  x | (y<<16),
  pass is -1.
*/
jint
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_genMoveInternal (
	JNIEnv*	env,
	jclass clasz,
  jboolean isBlack
	)
{
  int move;
  int resign;
  int color = isBlackToColor(isBlack);
  adjust_level_offset(color);

  move = genmove(color, NULL, &resign);

  if (resign) {
    // treat as pass for a while.
    return play(-1, -1, isBlack);
  }

  gnugo_play_move(move, color);  

  if (move != PASS_MOVE) {
    return J(move) | (I(move)<<16);
  }
  else {
    // PASS
    return -1;
  }
}


void
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_setBoardSize (
	JNIEnv*	env,
	jclass clasz,
	jint boardSize
	)
{
  set_boardsize(boardSize);
  clear_board();
  reset_engine();
}

static int
oprintf(const char *format_string, ...)
{
  va_list arguments;
  int result;
  char buf[256];

  va_start(arguments, format_string);
  result = vsprintf(buf, format_string, arguments);
  va_end(arguments);

  strcat(g_output_line, buf);

  return result;
}


jstring
Java_io_github_karino2_paoogo_goengine_gnugo3_GnuGo3Native_debugInfo (
	JNIEnv*	env,
	jclass clasz
	)
{
  g_output_line[0] = '\0';
  /*
  oprintf("komi=%f\n", get_komi());
  oprintf("boardsize=%d\n", get_boardsize());
  oprintf("movenumber=%d\n", get_movenumber());
  oprintf("depth=%d\n", depth);
  */
  return (*env)->NewStringUTF (env, g_output_line);
}
