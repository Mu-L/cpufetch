#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "args.h"
#include "global.h"

#define ARG_STR_STYLE    "style"
#define ARG_STR_COLOR    "color"
#define ARG_STR_HELP     "help"
#define ARG_STR_LEVELS   "levels"
#define ARG_STR_VERBOSE  "verbose"
#define ARG_STR_VERSION  "version"

#define ARG_CHAR_STYLE   's'
#define ARG_CHAR_COLOR   'c'
#define ARG_CHAR_HELP    'h'
#define ARG_CHAR_LEVELS  'l'
#define ARG_CHAR_VERBOSE 'v'
#define ARG_CHAR_VERSION 'v'

#define STYLE_STR_1      "fancy"
#define STYLE_STR_2      "retro"
#define STYLE_STR_3      "legacy"

struct args_struct {
  bool levels_flag;
  bool help_flag;
  bool verbose_flag;
  bool version_flag;
  STYLE style;
  struct colors* colors;
};

static const char* SYTLES_STR_LIST[STYLES_COUNT] = { STYLE_STR_1, STYLE_STR_2, STYLE_STR_3 };
static struct args_struct args;

STYLE get_style() {
  return args.style;
}

struct colors* get_colors() {
  return args.colors;
}

bool show_help() {
  return args.help_flag;
}

bool show_version() {
  return args.version_flag;
}

bool show_levels() {
  return args.levels_flag;
}

bool verbose_enabled() {
  return args.verbose_flag;
}

STYLE parse_style(char* style) {
  int i = 0;
  while(i != STYLES_COUNT && strcmp(SYTLES_STR_LIST[i],style) != 0)
    i++;

  if(i == STYLES_COUNT)
    return STYLE_INVALID;
  return i;
}

void free_colors_struct(struct colors* cs) {
  free(cs->c1);
  free(cs->c2);
  free(cs->c3);
  free(cs->c4);
  free(cs);
}

bool parse_color(char* optarg, struct colors** cs) {
  *cs = malloc(sizeof(struct colors));        
  (*cs)->c1 = malloc(sizeof(struct color));
  (*cs)->c2 = malloc(sizeof(struct color));
  (*cs)->c3 = malloc(sizeof(struct color));
  (*cs)->c4 = malloc(sizeof(struct color));
  struct color** c1 = &((*cs)->c1);
  struct color** c2 = &((*cs)->c2);
  struct color** c3 = &((*cs)->c3);
  struct color** c4 = &((*cs)->c4);
  int32_t ret;
  
  ret = sscanf(optarg, "%d,%d,%d:%d,%d,%d:%d,%d,%d:%d,%d,%d", 
               &(*c1)->R, &(*c1)->G, &(*c1)->B,
               &(*c2)->R, &(*c2)->G, &(*c2)->B,
               &(*c3)->R, &(*c3)->G, &(*c3)->B,
               &(*c4)->R, &(*c4)->G, &(*c4)->B);
  
  if(ret != 12) {
    printErr("Expected to read 12 values for color but read %d", ret);
    return false;    
  }
  
  //TODO: Refactor c1->R c2->R ... to c[i]->R
  if((*c1)->R < 0 || (*c1)->R > 255) {
    printErr("Red in color 1 is invalid. Must be in range (0, 255)");
    return false;
  }
  if((*c1)->G < 0 || (*c1)->G > 255) {
    printErr("Green in color 1 is invalid. Must be in range (0, 255)");
    return false;
  }
  if((*c1)->B < 0 || (*c1)->B > 255) {
    printErr("Blue in color 1 is invalid. Must be in range (0, 255)");
    return false;
  }
  if((*c2)->R < 0 || (*c2)->R > 255) {
    printErr("Red in color 2 is invalid. Must be in range (0, 255)");
    return false;
  }
  if((*c2)->G < 0 || (*c2)->G > 255) {
    printErr("Green in color 2 is invalid. Must be in range (0, 255)");
    return false;
  }
  if((*c2)->B < 0 || (*c2)->B > 255) {
    printErr("Blue in color 2 is invalid. Must be in range (0, 255)");
    return false;
  }
  
  return true;      
}

bool parse_args(int argc, char* argv[]) {
  int c;
  int option_index = 0;  
  opterr = 0;

  bool color_flag = false;
  args.levels_flag = false;
  args.verbose_flag = false;
  args.help_flag = false;
  args.style = STYLE_EMPTY;
  args.colors = NULL;

  static struct option long_options[] = {
      {ARG_STR_STYLE,    required_argument, 0, ARG_CHAR_STYLE   },
      {ARG_STR_COLOR,    required_argument, 0, ARG_CHAR_COLOR   },
      {ARG_STR_HELP,     no_argument,       0, ARG_CHAR_HELP    },
      {ARG_STR_LEVELS,   no_argument,       0, ARG_CHAR_LEVELS  },
      {ARG_STR_VERBOSE,  no_argument,       0, ARG_CHAR_VERBOSE },
      {ARG_STR_VERSION,  no_argument,       0, ARG_CHAR_VERSION },
      {0, 0, 0, 0}
  };

  c = getopt_long(argc, argv, "", long_options, &option_index);

  while (c != -1) {
     if(c == ARG_CHAR_COLOR) {
       if(color_flag) {
         printErr("Color option specified more than once");
         return false;
       }
       color_flag  = true;       
       if(!parse_color(optarg, &args.colors)) {
         printErr("Color parsing failed");
         return false;
       }
     }
     else if(c == ARG_CHAR_STYLE) {
       if(args.style != STYLE_EMPTY) {
         printErr("Style option specified more than once");
         return false;
       }
       args.style = parse_style(optarg);
       if(args.style == STYLE_INVALID) {
         printErr("Invalid style '%s'",optarg);
         return false;
       }
     }
     else if(c == ARG_CHAR_HELP) {
       if(args.help_flag) {
         printErr("Help option specified more than once");
         return false;
       }
       args.help_flag  = true;
     }
     else if(c == ARG_CHAR_VERBOSE) {
       if(args.verbose_flag) {
         printErr("Verbose option specified more than once");
         return false;
       }
       args.verbose_flag  = true;
     }
     else if(c == ARG_CHAR_LEVELS) {
       if(args.levels_flag) {
         printErr("Levels option specified more than once");
         return false;
       }
       args.levels_flag  = true;
     }
     else if (c == ARG_CHAR_VERSION) {
       if(args.version_flag) {
         printErr("Version option specified more than once");
         return false;
       }
       args.version_flag = true;
     }
     else if(c == '?') {
       printWarn("Invalid options");
       args.help_flag  = true;
       break;
     }
     else
      printBug("Bug at line number %d in file %s", __LINE__, __FILE__);

    option_index = 0;
    c = getopt_long(argc, argv,"",long_options, &option_index);
  }

  if (optind < argc) {
    printWarn("Invalid options");
    args.help_flag  = true;
  }

  if((args.help_flag + args.version_flag + color_flag) > 1) {
    printWarn("You should specify just one option");
    args.help_flag  = true;
  }

  return true;
}