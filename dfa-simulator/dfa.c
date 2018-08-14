# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>



# define MAXNUM 100
# define MAXSTATES 100
# define MAXINPUT 1000 
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
int getdesignatedstates(FILE *file, int *init, int accept[], int numstates);
int run(int init, int accept[], int (*transitions)[][MAXSTATES], int input[]);
void printtable(int transitions[][MAXSTATES], int numsymbols, int numstates) ;
void printdesignated(int start, int *accept, int numstates);
void printinput(int input[], int len); 
// need to handle init and accept states


// print the table, the accept states and the input

int main(int argc, char** argv) {  
  FILE *file;
  file = fopen(argv[1], "r"); // get trans table
  if (!file) {
    printf("couldn't open transition table file\n");
    return -1;
  }

  int numsymbols = getintf(file);
  int numstates = getintf(file);


    // get transitions
  printf("building transition table..\n");
  int transitions[numsymbols][MAXSTATES];
  if(buildtable(file, &transitions, numsymbols, numstates) < 0) {
    printf("failed to open transition table file.\n");
    return -1;
  }

  fclose(file);

  printtable(transitions, numsymbols, numstates);


  printf("getting init and accept states...\n");
  file = fopen(argv[2], "r");
  if(!file) {
    printf("failed to open state designation file.\n");
    return -1;
  }
  int init;
  int accept[numstates];
  if(getdesignatedstates(file, &init, accept, numstates) < 0) 
    printf("failed designating the init and accept states");
  fclose(file);
  printdesignated(init, accept, numstates);


  printf("getting input string...\n");
  file = fopen(argv[3], "r");
  if(!file) {
    printf("failed to open input file.\n");
    return -1;
  }
  int len = getintf(file);
  int input[len];
  int isym;
  int *i = input;
  while((*i++ = getintf(file)) >= 0)
    ;
  fclose(file);
  printinput(input, len);

  // run the dfa
  printf("running the dfa...\n");
  return run(init, accept, &transitions, input);
}


int getintf(FILE *fp) {
  char numeral[MAXNUM];
  char *n = numeral;
  while(isspace(*n = getc(fp)) && *n != EOF)
    ;
  if (*n < 0)
    return -1;
  if(*n < '0' || *n > '9') {
    printf("getintf: read nonnumeric character %d\n", *n);
    return -1;
  }
  else {
    while((*++n = getc(fp)) >= '0' && *n <= '9')
      ;
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


void printtable(int transitions[][MAXSTATES], int numsymbols, int numstates) {
  printf("\t");
  for(int symbol = 0; symbol < numsymbols; symbol++) {
    printf("\t%d", symbol);
  }
  printf("\n");
  for(int state = 0; state < numstates; state++) {
    printf("%d\t", state);
    for(int symbol = 0; symbol < numsymbols; symbol++) {
      printf("\t%d", transitions[symbol][state]);
    }
    printf("\n");
  }
}



int inbounds(int s, int numstates) {
  return (s >= 0 && s < numstates);
}


// get init and accept states
int getdesignatedstates(FILE *file, int *init, int *accept, int numstates) {
  if(!inbounds((*init = getintf(file)), numstates)) {
    printf("read out of bounds init state.\n");
    return -1;
  }
  for(int i = 0; i < numstates; accept[i++] = 0)
    ;
  int a;
  while((a = getintf(file)) >= 0) {
    /* printf("setting %d to accept\n", a); */
    if(!inbounds(a, numstates)) {
      printf("read out of bounds accept state %d.\n", a);
      return -1;
    }
    accept[a] = 1;
  }
  return 0;
}

void printdesignated(int init, int accept[], int numstates) {
  printf("init:\t%d\n", init);
  printf("accept:\t");
  for(int i = 0; i < numstates; i++)
    /* printf("%d", i); */
    printf(accept[i]? "%d\t": "", i);
  printf("\n");
}


void printinput(int input[], int len) {
  printf("input string of len %d: ", len);
  for(int i = 0; i < len; i++)
    /* printf("%d", i); */
    printf("%d\t", input[i]);
  printf("\n");
}


int run(int init, int accept[], int (*transitions)[][MAXSTATES], int input[]) {
  int symbol; 
  int state = init;
  int newstate;
  while((symbol = *input++) >= 0) {
    newstate = (*transitions)[symbol][state];
    printf("%d, %d -> %d\n", state, symbol, newstate);
    state = newstate;
  }
  int result = accept[state];
  printf("ended in state %d, which is %san accept state.\n",
         state, result? "": "not ");
  return accept[state];
}



