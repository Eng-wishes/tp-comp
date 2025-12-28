/* ameziane amani
Aoughellanet Meriem
Sahnouni Aya*/
/* g1 security*/
#include <stdio.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 10
#define MAX_TRANS 10

/* Global Variables */

int n_states, n_symbols;
char symbols[MAX_SYMBOLS];

int start_state;
int n_final;
int final_states[MAX_STATES];

// transitions=state
int trans[MAX_STATES][MAX_SYMBOLS][MAX_TRANS];
int trans_count[MAX_STATES][MAX_SYMBOLS];

// epsilon transitions
int epsilon_trans[MAX_STATES][MAX_STATES];

// epsilon-closure
int epsilon_closure[MAX_STATES][MAX_STATES];
int closure_size[MAX_STATES];

// new transitions
int new_trans[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int new_trans_count[MAX_STATES][MAX_SYMBOLS];
int new_final[MAX_STATES];

int visited[MAX_STATES];

//Task 1

void read_nfa() {
    printf("Enter number of states: ");
    scanf("%d", &n_states);

    printf("Enter number of symbols (excluding epsilon): ");
    scanf("%d", &n_symbols);

    if (n_symbols > 0) {
        printf("Enter symbols: ");
        for (int i = 0; i < n_symbols; i++)
            scanf(" %c", &symbols[i]);
    }

    printf("Enter start state (0 to %d): ", n_states - 1);
    scanf("%d", &start_state);

    printf("Enter number of final states: ");
    scanf("%d", &n_final);

    if (n_final > 0) {
        printf("Enter final states: ");
        for (int i = 0; i < n_final; i++)
            scanf("%d", &final_states[i]);
    }

    //Initialization
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_symbols; j++)
            trans_count[i][j] = 0;

        for (int j = 0; j < n_states; j++)
            epsilon_trans[i][j] = 0;
    }

    printf("\nEnter transitions:\n");

    for (int state = 0; state < n_states; state++) {
        for (int s = 0; s < n_symbols; s++) {
            printf("State %d on '%c' -> count: ", state, symbols[s]);
            scanf("%d", &trans_count[state][s]);

            for (int j = 0; j < trans_count[state][s]; j++)
                scanf("%d", &trans[state][s][j]);
        }

        int count;
        printf("State %d on epsilon -> count: ", state);
        scanf("%d", &count);

        for (int j = 0; j < count; j++) {
            int target;
            scanf("%d", &target);
            epsilon_trans[state][target] = 1;
        }
    }
}

//Task 2:

void dfs_epsilon(int state) {
    visited[state] = 1;

    for (int next = 0; next < n_states; next++) {
        if (epsilon_trans[state][next] && !visited[next]) {
            dfs_epsilon(next);
        }
    }
}

void compute_epsilon_closure() {
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_states; j++)
            visited[j] = 0;

        dfs_epsilon(i);

        int idx = 0;
        for (int j = 0; j < n_states; j++) {
            if (visited[j])
                epsilon_closure[i][idx++] = j;
        }
        closure_size[i] = idx;
    }
}

// Task 3: New Transitions

void compute_new_transitions() {
    for (int state = 0; state < n_states; state++) {
        for (int sym = 0; sym < n_symbols; sym++) {

            int reached[MAX_STATES] = {0};
            int count = 0;

            for (int k = 0; k < closure_size[state]; k++) {
                int from = epsilon_closure[state][k];

                for (int j = 0; j < trans_count[from][sym]; j++) {
                    int target = trans[from][sym][j];

                    for (int m = 0; m < closure_size[target]; m++) {
                        int dest = epsilon_closure[target][m];
                        if (!reached[dest]) {
                            reached[dest] = 1;
                            new_trans[state][sym][count++] = dest;
                        }
                    }
                }
            }
            new_trans_count[state][sym] = count;
        }
    }
}

// Task 4: New Final States

void compute_new_final_states() {
    for (int state = 0; state < n_states; state++) {
        new_final[state] = 0;

        for (int i = 0; i < closure_size[state]; i++) {
            int s = epsilon_closure[state][i];


for (int j = 0; j < n_final; j++) {
                if (s == final_states[j]) {
                    new_final[state] = 1;
                    break;
                }
            }
            if (new_final[state]) break;
        }
    }
}

// Task 5: Display NFA

void display_new_nfa() {

    printf("   New NFA (Epsilon Removed)\n");


    printf("States      : 0 to %d\n", n_states - 1);
    printf("Start state : %d\n", start_state);

    printf("Final states: ");
    int found = 0;
    for (int i = 0; i < n_states; i++) {
        if (new_final[i]) {
            printf("%d ", i);
            found = 1;
        }
    }
    if (!found) printf("None");
    printf("\n\n");

    printf("State  ");
    for (int i = 0; i < n_symbols; i++)
        printf("| %-8c ", symbols[i]);
    printf("\n");

    printf("-------");
    for (int i = 0; i < n_symbols; i++)
        printf("+----------");
    printf("\n");

    for (int state = 0; state < n_states; state++) {
        printf("%-6d ", state);

        for (int sym = 0; sym < n_symbols; sym++) {
            printf("| {");
            for (int j = 0; j < new_trans_count[state][sym]; j++) {
                printf("%d", new_trans[state][sym][j]);
                if (j < new_trans_count[state][sym] - 1)
                    printf(", ");
            }
            printf("} ");
        }
        printf("\n");
    }
}

// main

int main() {
    read_nfa();                  // Task 1
    compute_epsilon_closure();   // Task 2
    compute_new_transitions();   // Task 3
    compute_new_final_states();  // Task 4
    display_new_nfa();           // Task 5
    return 0;
}
