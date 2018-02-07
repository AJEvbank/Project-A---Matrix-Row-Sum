#ifndef COMM_LINE

#define COMM_LINE

#include "main.h"

void CommLineArgs(int argc,
                  char ** argv,
                  int *rows,
                  int *cols,
                  int *seed,
									int *gran,
                  int *print
                 )
{
  *rows = ROWS; *cols = COLS; *seed = SEED; *gran = GRAN, *print = PRINT;
  static char * SHORT = "";
	static struct option OPTS[] =
																{
																	{"rows",required_argument,0,'r'},
                                  {"cols",required_argument,0,'c'},
                                  {"seed",required_argument,0,'s'},
                                  {"gran",required_argument,0,'g'},
                                  {"print",no_argument,0,'p'},
																	{0,0,0,0}
																};
	int ch;
	int opt_index = 0;
	while(1)
	{
		ch = getopt_long_only(argc,argv,SHORT,OPTS,&opt_index);

    if (ch == -1)
		{
			break;
		}

    switch(ch)
    {
      case 'r':
                if(isNumber(optarg))
                {
                  *rows = atoi(optarg);
                }
                else
                {
                  *rows = ROWS;
                }
                break;
      case 'c':
                if(isNumber(optarg))
                {
                  *cols = atoi(optarg);
                }
                else
                {
                  *cols = COLS;
                }
                break;
      case 's':
                if(isNumber(optarg))
                {
                  *seed = atoi(optarg);
                }
                else
                {
                  *seed = SEED;
                }
                break;
      case 'g':
                if(isNumber(optarg))
                {
                  *gran = atoi(optarg);
                }
                else
                {
                  *gran = GRAN;
                }
                break;
      case 'p':
                *print = 1;
                break;
      default:
                printf("default case\n");
                break;
    }
  }

}

int isNumber(const char * str)
{
	int i;
	int len = strlen(str);
	char ch;
	enum isNumStates state = INITIAL;
	if (len > 0)
	{
		for (i = 0; i < len; i++)
		{
			ch = str[i];
			switch (state)
			{
			case INITIAL:
			{
				if (ch == '-' || ch == '+')
				{
					state = PLUSORMINUS;
				}
				else if (ch == '0')
				{
					state = ZERO;
				}
				else if (isdigit(ch))
				{
					state = NUMBER;
				}
				else
				{
					state = ERROR;
				}
				break;
			}

			case PLUSORMINUS:
			{
				if (ch == '0')
				{
					state = ZERO;
				}
				else if (isdigit(ch))
				{
					state = NUMBER;
				}
				else
				{
					state = ERROR;
				}
				break;
			}

			case ZERO:
			{
				if (ch == '.')
				{
					state = DECIMAL;
				}
				else
				{
					state = ERROR;
				}
				break;
			}

			case NUMBER:
			{
				if (isdigit(ch))
				{
					state = NUMBER;
				}
				else if (ch == '.')
				{
					state = DECIMAL;
				}
				else
				{
					state = ERROR;
				}
				break;
			}

			case DECIMAL:
			{
				if (isdigit(ch))
				{
					state = DECIMAL;
				}
				else
				{
					state = ERROR;
				}
				break;
			}

			case ERROR:
			{
				return 0;
				break;
			}

			default:
			{
				printf("default in isNumber: %d \n", (int)state);
				return 0;
				break;
			}

			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}



#endif
