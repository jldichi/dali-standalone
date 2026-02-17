#include <string.h>

#include <ideafix.h>
#include <ideafix/priv/creprep.h>

void SetWIFProperty(char *name, char *value) {
	if (CrackerClient()) {
		if (!name || !value) {
			return;
		} 

		if (strcmp(name, LOCALPRINTER) == 0) {
			char aux[20] = { 0 };
			char *port;

			strcpy(aux, value);
			port = strchr(aux, ':');
			if (port) {
				port[0] = '\0';
				port++;
				setPort(port);
			}
			else setPort(DEFAULT_PORT);

			setCap(aux);
		}
		else {
			char cmd[512] = { 0 };

			sprintf(cmd, "....%s=%s", name, value);

			CrackerReceiveFile(EMPTY_FILE, cmd);
		}
	}
}
