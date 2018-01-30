#ifndef COMM_LINE

#define COMM_LINE

#include "main.h"

void CommLineArgs(int argc,
                  char ** argv,
                  unsigned long *rows,
                  unsigned long *cols,
                  unsigned long *seed,
                  unsigned long *gran
                 )
{
  *rows = ROWS; *cols = COLS; *seed = SEED; *gran = GRAN;
  static char * SHORT = "";
	static struct option OPTS[] =
																{
																	{"rows",required_argument,0,'r'},
                                  {"cols",required_argument,0,'c'},
                                  {"seed",required_argument,0,'s'},
                                  {"gran",required_argument,0,'g'},
																	{0,0,0,0}
																};
	int ch;
	int opt_index = 0;
  char * end;
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
                end = &optarg[strlen(optarg)-1];
                if(isNumber(optarg))
                {
                  *rows = strtoul(optarg,&end,10);
                }
                else
                {
                  *rows = ROWS;
                }
                break;
      case 'c':
                end = &optarg[strlen(optarg)-1];
                if(isNumber(optarg))
                {
                  *cols = strtoul(optarg,&end,10);
                }
                else
                {
                  *cols = COLS;
                }
                break;
      case 's':
                end = &optarg[strlen(optarg)-1];
                if(isNumber(optarg))
                {
                  *seed = strtoul(optarg,&end,10);
                }
                else
                {
                  *seed = SEED;
                }
                break;
      case 'g':
                end = &optarg[strlen(optarg)-1];
                if(isNumber(optarg))
                {
                  *gran = strtoul(optarg,&end,10);
                }
                else
                {
                  *gran = GRAN;
                }
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
