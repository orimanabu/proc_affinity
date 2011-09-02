#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <getopt.h>

void usage(void)
{
	printf("usage: proc_affinity [-c|-H|-q] pid\n");
	printf("  -c or --csv\t\tprint in CSV format.\n");
	printf("  -H or --header-only\tprint header.\n");
	printf("  -q or --quote\t\tquote each column. use with -c.\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	cpu_set_t cpuset;
	int cpu;
//	int i;
	int ret;
	int nprocs;
	int option_index = 0;
	int opt;
	int opt_help = 0;
	int opt_csv = 0;
	int opt_header_only = 0;
	int opt_quote = 0;

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"csv", no_argument, NULL, 'c'},
		{"header-only", no_argument, NULL, 'H'},
		{"quote", no_argument, NULL, 'q'},
		{0, 0, 0, 0}
	};

	if (argc == 1) usage();

	while (1) {
		opt = getopt_long(argc, argv, "chHq", long_options, &option_index);
//		printf("*** opt=%d, option_index=%d\n", opt, option_index);
		if (opt == -1)
			break;
		switch (opt) {
		case 'c':
			opt_csv = 1;
			break;
		case 'h':
			opt_help = 1;
			break;
		case 'H':
			opt_header_only = 1;
			break;
		case 'q':
			opt_quote = 1;
			break;
		default:
			opt_help = 1;
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (opt_help) usage();

	nprocs = (int)sysconf(_SC_NPROCESSORS_ONLN);
	if (nprocs < 0) {
		perror("sysconf");
		exit(1);
	}
//	printf("** # of processors: %d\n", nprocs);

	if (opt_header_only) {
		for (cpu = 0; cpu < nprocs; cpu++) {
			if (opt_quote) printf("\"");
			printf("cpu%d", cpu);
			if (opt_quote) printf("\"");
			printf(",");
		}
		exit(0);
	}

	pid = atoi(argv[0]);
	ret = sched_getaffinity(pid, sizeof(cpu_set_t), &cpuset);
	if (ret != 0) {
		perror("sched_getaffinity");
	}

	if (opt_csv) {
//		printf("%d,", pid);
		for (cpu = 0; cpu < nprocs; cpu++) {
			if (opt_quote) printf("\"");
			printf("%d", CPU_ISSET(cpu, &cpuset));
			if (opt_quote) printf("\"");
			printf(",");
		}
	} else {
		printf("pid=%d\n", pid);
		for (cpu = 0; cpu < nprocs; cpu++) {
			if (CPU_ISSET(cpu, &cpuset)) {
				printf("cpu%d: affinity on\n", cpu);
			} else {
				printf("cpu%d: affinity off\n", cpu);
			}
		}
	}

	exit(0);
}
