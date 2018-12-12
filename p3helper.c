/* p3helper.c
 * Chad Krauthamer
 * Program 3
 * CS570
 * Professor Carroll
 * SDSU
 * November 1st 2018
 *
 * This is the only file you may change. (In fact, the other files should
 * be symbolic links to:
 *   ~cs570/Three/p3main.c
 *   ~cs570/Three/p3.h
 *   ~cs570/Three/Makefile
 *   ~cs570/Three/CHK.h    
 *
 */
#include "p3.h"

// Semaphores to control joggers, shooters, and the gym
sem_t shooters;
sem_t joggers;
sem_t gym;

// Count Variables
int numJoggers = 0;
int numShooters = 0;

void initstudentstuff(void) {
    // Initialize the semaphores to be able to enter
    CHK(sem_init(&shooters, 0, 1));
    CHK(sem_init(&joggers, 0, 1));
    CHK(sem_init(&gym, 0, 1));
}

// Similar protocol for joggers and shooters
void prolog(int kind) {
    if(kind == JOGGER){
        CHK(sem_wait(&joggers)); // Get exclusive acces to numJoggers
        numJoggers = numJoggers + 1; // One jogger now
        if (numJoggers == 1) { // First jogger
            CHK(sem_wait(&gym)); // Acces to the gym
        }
        CHK(sem_post(&joggers)); // Release access to numJoggers
    } else{
        CHK(sem_wait(&shooters));
        numShooters = numShooters + 1;
        if (numShooters == 1) {
            CHK(sem_wait(&gym));
        }
        CHK(sem_post(&shooters));
    }
}

// Similar protocol for joggers and shooters
void epilog(int kind) {
    if (kind == JOGGER) {
        CHK(sem_wait(&joggers)); // Exclusive access to numJoggers
        numJoggers = numJoggers - 1; // One less jogger
        if (numJoggers == 0)
            CHK(sem_post(&gym)); // Last jogger
        CHK(sem_post(&joggers)); // Release exclusive access
    } else {
        CHK(sem_wait(&shooters));
        numShooters = numShooters - 1;
        if (numShooters == 0)
            CHK(sem_post(&gym));
        CHK(sem_post(&shooters));
    }
}
