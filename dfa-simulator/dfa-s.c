# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>



# define MAXNUM 100
# define MAXSTATES 100

/* 
representation:
the symbol set is [0..n-1] for some n;
the state set is [0..m-1] for some m;
input is an array of nonnegative int, followed by -1;
transition table delta maps state plus symbol to state

api:
read the transition table as a whitespace separated string of mxn+2 integers, where the first two integers are themselves m and n
read the input as a whitespace separated string of integers
the program takes the two filenames as arguments.  allow flags to specify verbosity and bound on the steps
*/

// first line represents number of states
// assume for now that file is syntactically correct



int getintf(FILE *fp);
int inbounds(int s, int numstates);
int buildtable(FILE *file, int (*table)[][MAXSTATES], int numsymbols, int numstates);
int getlabeledstates(FILE *file, int *init, int accept[], int numstates);
int run(int init, int accept[], int (*transitions)[][MAXSTATES], int input[]);


// need to handle init and accept states

int main(int argc, char** argv) {  
  FILE *file;
  file = fopen(argv[1], "r"); // get trans table
  if (!file) {
    printf("couldn't open transition table file\n");
    return -1;
  }
    // get transitions
  int numstates = getintf(file);
  int numsymbols = getintf(file);

  // build table
  int transitions[numsymbols][MAXSTATES];
  if(buildtable(file, &transitions, numsymbols, numstates) < 0) {
    printf("failed to open transition table file.\n");
    return -1;
  }

  fclose(file);


  // get init and accept states
  file = fopen(argv[2], "r");
  if(!file) {
    printf("failed to open state designation file.\n");
    return -1;
  }
  int init = getintf(file);
  int accept[numstates];
  if(getlabeledstates(file, &init, accept, numstates) < 0) 
    printf("failed labeling the init and accept states");
  fclose(file);


  // get the input
  file = fopen(argv[2], "r");
  if(!file) {
    printf("failed to open input file.\n");
    return -1;
  }
  int len = getintf(file);
  int input[len];
  int isym;
  int *i = input;
  while(*i++ = getintf(file) > 0) {
    *i++ = isym;
  }
  fclose(file);

  // run the dfa
  return run(init, accept, &transitions, input);
}



int getintf(FILE *fp) {
  char numeral[MAXNUM];
  char *n = numeral;
  while(isspace(*n = getc(fp) && *n != EOF));
  if(*n < '0' || *n > '9') {
    return -1;
  }
  else {
    while((*++n = getc(fp)) >= '0' && *n <= '9');
    *n = '\0';
    return atoi(numeral);
  }
}


int buildtable(FILE *file, int (*table)[][MAXSTATES], int numsymbols, int numstates) {
  int state, symbol, newstate;
  for(state = 0; state < numstates; state++) {
    for(symbol = 0; symbol < numsymbols; symbol++) {
      if((newstate = getintf(file)) >= 0) {
        if(((*table)[symbol][state] = newstate) >= numstates) 
          printf("at state %d, symbol %d, transition to state %d is out of bounds!\n",
                 state, symbol, newstate);
      }
      else {
        printf("no transition was specified at state %d for symbol %d", state, symbol);
        return -1;
      }
    }
  }
  return 0;
}

int inbounds(int s, int numstates) {
  return (s >= 0 && s < numstates);
}


// get init and accept states
int getlabeledstates(FILE *file, int *init, int accept[], int numstates) {
  if(!inbounds((*init = getintf(file)), numstates)) {
    printf("read out of bounds init state.\n");
    return -1;
  }
  for(int i = 0; i < numstates; accept[i++] = 0)
    ;
  int a;
  while(a = getintf(file))
    if(!inbounds(a, numstates)) {
       printf("read out of bounds accept state.\n");
      return -1;
    }
  accept[a] = 1;
  return 0;
}


int run(int init, int accept[], int (*transitions)[][MAXSTATES], int input[]) {
  int symbol; 
  int state = init;
  while((symbol = *input++) > 0) {
    state = (*transitions)[symbol][state];
  }
  return accept[state];
}



