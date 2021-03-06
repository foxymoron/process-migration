/*
 * ===================================================================================================================
 *
 *       Filename:  gui.c
 *
 *    Description:  GUI for statistics and testing of Process Migration
 *
 *        Version:  1.0
 *        Created:  Thursday 26 May 2011 12:48:32  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *      Author(s):  Jait Dixit(07IT214), Deep Shah(07IT222), Mihir Bhatt(07IT242)
 *        College:  G H Patel College of Engineering & Technology, Bakrol, Vallabh Vidyanagar, Gujarat, India -388120
 *	    Guide:  Prof. Nikhil Gondaliya
 *
 * ==================================================================================================================
 */

#include <signal.h>
#include <time.h>
#include "cdk.h"
#include "info.pb-c.h"
#include "migrate.h"


CDKSCREEN *cdkscreen = (CDKSCREEN *)NULL;
CDKSWINDOW *loadDaemon = (CDKSWINDOW *)NULL;
CDKSWINDOW *commsDaemon = (CDKSWINDOW *)NULL;
CDKSWINDOW *migrateDaemon = (CDKSWINDOW *)NULL;
CDKSWINDOW *extraInfo = (CDKSWINDOW *)NULL;
CDKLABEL *title = (CDKLABEL *)NULL;
CDKLABEL *info = (CDKLABEL *)NULL;
WINDOW *cursesWin = (WINDOW *)NULL;
struct timespec req,rem;
char *shared_memory;

void cp_loadlog() 
{
	char command[] = "cp /var/log/load /tmp/migrate/load.temp";
	
	if(system(command) == -1)
		exit(1);
}

int get_change_loadlog() {
	char command[] = "diff /tmp/migrate/load.temp /var/log/load | head -n 1";
	int i,line;
	FILE *cmd;
	char output[256], *ptr;
	char readline[1024];

	sprintf(output, "\0");

	cmd = popen(command,"r");
	fgets(output, 257, cmd);
	pclose(cmd);
	
	addCDKSwindow(extraInfo, output, BOTTOM);
	if(strcmp(output, "\0") == 0) {
		return -1;
	}

	ptr = strtok(output,"a");

	line = atoi(ptr);
	
	return line;
}
	
void substr(char *res,char *string)
{
	int i;
	char *ptr;
	char *truncated;

	truncated = (char *) malloc(512);

	ptr = rindex(string,':');
	ptr++;
	for(i = 0; ptr[i] != '\0'; i++) {
		truncated[i] = ptr[i];
	}
	truncated[++i] = '\0';
	
	strcpy(res,truncated);
	free(truncated);
}

void cleanup_handler(int signum)
{
	shmdt(shared_memory);
	destroyCDKSwindow(loadDaemon);
	destroyCDKSwindow(commsDaemon);
	destroyCDKSwindow(migrateDaemon);
	destroyCDKSwindow(extraInfo);
	destroyCDKLabel(title);
	destroyCDKLabel(info);
	delwin(cursesWin);
	endCDK();
	exit(0);
}



int main()
{
	char *mesg[4],*infobuf[2];
	char *shared_memory;
	char string[1024],temp[1024];
	FILE *file;
	Nodeinfo *mynode;
	int segment_id, sem_id, i, line;
	void *buf;
	
	signal(SIGTERM, cleanup_handler);
	signal(SIGINT, cleanup_handler);

	req.tv_nsec = 5000000;
	cursesWin = initscr();
	cdkscreen = initCDKScreen(cursesWin);

	initCDKColor();
	sprintf(temp,"<C></B/3>Process Migration - GUI for Statistics and Testing");
	mesg[0] = copyChar(temp);
	title = newCDKLabel(cdkscreen, CENTER, TOP, mesg,1,1,0);
      	setCDKLabel (title, mesg, 1, 1);
      	drawCDKLabel (title, 1);
	freeChar (mesg[0]);

	loadDaemon = newCDKSwindow (cdkscreen, (COLS - 50) * 0.10, (LINES - 12) * 0.40, 12, 50, "<C></B/5>Load Daemon", 1000, 1, 0);
	commsDaemon = newCDKSwindow (cdkscreen, (COLS - 50) * 0.90, (LINES - 12) * 0.40, 12, 50, "<C></B/5>Commucation Daemon", 1000, 1, 0);
	migrateDaemon = newCDKSwindow (cdkscreen, (COLS - 50) * 0.10, (LINES - 12) * 0.90, 12, 50, "<C></B/5>Migration Control Daemon", 1000, 1, 0);
	extraInfo = newCDKSwindow (cdkscreen, (COLS - 50) * 0.90, (LINES - 12) * 0.90, 12, 50, "<C></B/5>Information", 1000, 1, 0);


	//setCDKSwindowBackgroundColor(loadDaemon, "</5>");
	//setCDKSwindowBackgroundColor(commsDaemon, "</5>");
	//setCDKSwindowBackgroundColor(migrateDaemon, "</5>");
	//setCDKSwindowBackgroundColor(extraInfo, "</5>");

	file = fopen("/var/log/load","r");
	while(fgets(string, 1025, file) != NULL) {
		substr(&temp[0],&string[0]);
		addCDKSwindow (loadDaemon, temp, BOTTOM);
	}
	fclose(file);
	
	buf = malloc(33);

	file = fopen(INFO_SHM_ID_PATH,"r");
	fgets(string,33,file);
	segment_id = atoi(string);
	fclose(file);

	file = fopen(INFO_SEM_ID_PATH,"r");
	fgets(string,33,file);
	sem_id = atoi(string);
	fclose(file);
	
	info = newCDKLabel(cdkscreen, 0, (LINES - 5) * 0.15, infobuf,3,0,0);
	shared_memory = (void *) shmat(segment_id, 0, 0);
	//setCDKLabelBackgroundColor(info, "</4>");
	refreshCDKScreen(cdkscreen);
	binary_semaphore_post(sem_id);
	cp_loadlog();
	for(;;) {
		line = get_change_loadlog();
		if(line != -1) {
			sprintf(temp,"Yaay, got info it is line %d",line);
			addCDKSwindow(extraInfo, temp, BOTTOM);
			file = fopen("/var/log/load","r");
			i = 0;
			while(fgets(string, 1025, file) != NULL) {
				if(i < line) {
					i++;
					continue;
				}
				else {
					substr(&temp[0],&string[0]);
					addCDKSwindow (loadDaemon, temp, BOTTOM);
					i++;
				}
			}
		} else {
			sprintf(temp,"Shit no info",line);
			addCDKSwindow(extraInfo, temp, BOTTOM);
		}
		binary_semaphore_wait(sem_id);
		memcpy(buf, shared_memory, 33);
		binary_semaphore_post(sem_id);
		mynode = nodeinfo__unpack(NULL, 33, buf);
		if(mynode == NULL) {
			fprintf(stderr,"error unpacking message\n");
			exit(1);
		}
		sprintf(temp,"\t\t\t\t\t\t\t</B/U/7>STATISTICS");
		infobuf[0] = copyChar(temp);
	sprintf(temp,"\t</B>Load:<!B> %0.2f\t</B>No. of peers connected:<!B> %d\t</B>No. of processes migrated:<!B> %d\t</B>No. of processes accepted:<!B> %d\t",mynode->load,mynode->np,mynode->npm,mynode->npa);
		infobuf[1] = copyChar(temp);
		sprintf(temp,"\t\t\t\t</B>Upper Threshold:<!B> %.2f\t</B>Lower Threshold:<!B> %.2f",mynode->uthresh,mynode->lthresh);
		infobuf[2] = copyChar(temp);
		setCDKLabel (info, infobuf, 3, 0);
		drawCDKLabel (info, 0);
		freeChar (infobuf[0]);
		freeChar (infobuf[1]);
		freeChar (infobuf[2]);
		sleep(1);
	}
	exit(0);
}
