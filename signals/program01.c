#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void signal_handler(int signal_number)
{
	if (signal_number == SIGINT) {
		printf("\nInterrupt SIGINT received.\nDo you really want to quit (y|n)?\n");
	}
	if (signal_number == SIGTERM)
	{
		printf("\nInterrupt SIGTERM received.\nDo you really want to quit (y|n)?\n");
	}
	char in;
	scanf("%c", &in);
	if (in == 'y' || in == 'Y')
		exit(0);
}

int main() {
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	printf("%d\n", getpid());
	while(1);
}