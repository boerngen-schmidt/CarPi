/*
 * 
 */
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>


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
			syslog(LOG_NOTICE, "Recieved SIGTERM. Exiting main loop."); 
			doRun = false;
			break;
		case SIGHUP:
			syslog(LOG_WARNING, "WARN: Caught SIGHUP. Doing nothing");
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
		syslog(LOG_ERR, "ERROR: Forking returned an error. Exiting.");
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {
		syslog(LOG_DEBUG, "DEBUG: Exiting parent process. Forking was successful.");
		exit(EXIT_SUCCESS);
	}
	
	// From here on we are in the daemon process
	/* Change the file mode mask */
	umask(0);
	
	/* open syslog file */
	setlogmask(LOG_UPTO(LOG_DEBUG));
	openlog("CarPi", LOG_PID | LOG_ODELAY, LOG_USER);
	
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
        syslog(LOG_ERR, "ERROR: New SID for child could not be created. Exiting.");
		exit(EXIT_FAILURE);
	}
	
	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		syslog(LOG_ERR, "ERROR: Could not change directory to '/'. Exiting.");
		exit(EXIT_FAILURE);
	}
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	syslog(LOG_DEBUG, "DEBUG: Daemonizing complete.");
	
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
	syslog(LOG_DEBUG, "DEBUG: Registered signal handling.");

	while(doRun) {
		
		sleep(2);
	}
	
	/* Clean up */
	syslog(LOG_DEBUG, "DEBUG: Exiting application.");
	closelog(); /* close previously opened syslog */
	return 0;
}