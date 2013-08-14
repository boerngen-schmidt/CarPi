/*
 * 
 */
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>


/**
 * @brief Indicator if program loop should continue running
 */
bool doRun = true;

/**
 * @brief Handles the signals
 * 
 * @param signal Numerical value of signal
 * @return void
 */
void signalHandler(int signal) {
	switch(signal) {
		case SIGTERM:
			doRun = false;
			break;
		case SIGHUP:
			// TODO log and do nothing :-)
			break;
	}
	return;
}

int main(int argc, char** argv) {

	pid_t pid, sid;
	
	/** 
	 * Fork off the parent process
	 * http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
	 */
	pid = fork();
	if (pid < 0) {
		// forking failed :(
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {
		// exit the parent process
		exit(EXIT_SUCCESS);
	}
	
	// From here on we are in the daemon process
	/* Change the file mode mask */
	umask(0);
	
	// TODO opening Logfiles for writing
	
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
        /* TODO Log any failure */
		exit(EXIT_FAILURE);
	}
	
	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* TODO Log any failure here */
		exit(EXIT_FAILURE);
	}
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// DONE daemonizing.
	
	
	/**
	 * register the signalHandler for SIGTERM
	 * http://www.cplusplus.com/reference/csignal/signal/
	 */
	signal(SIGTERM, signalHandler); /* catch kill signal */
	signal(SIGHUP, signalHandler); /* catch hangup signal */
	signal(SIGCHLD, SIG_IGN); /* ignore child */
	signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

	while(doRun) {
		
		sleep(2);
	}
	return 0;
}