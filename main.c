#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	int name, references;
} file_t;

typedef struct {
	int name, file1, file2;
} job_t;

typedef struct {
	job_t **jobs;
	file_t **files;
	int num_jobs, num_files, num_alloc_jobs, num_alloc_files;
} state_t;

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
state_t *read_file(char *filename, int cflag);

int main(int argc, char *argv[]) {
	int fflag, eflag, cflag;
	char *filename;

	if (set_flags(argc, argv, &fflag, &filename, &eflag, &cflag)) {
		return 1;
	}

	state_t *state = read_file(filename, cflag);	
	if (!state) {
		return 1;
	}
	
	for (int i = 0; i < state->num_jobs; i++) {
		printf("%d %d %d\n", state->jobs[i]->name, state->jobs[i]->file1, state->jobs[i]->file2);
	}

	for (int i = 0; i < state->num_files; i++) {
		printf("%d %d\n", state->files[i]->name, state->files[i]->references);
	}
	return 0;
}


int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag) {
	/* Get command line options - heavily based on the example given at: "https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html" */
	*fflag = 0, *eflag = 0, *cflag = 0;

	opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "f:e")) != -1) {
		switch (c) { 
			case 'f':
				*fflag = 1;
				*filename = optarg;
				break;
			
			case 'e':
				*eflag = 1;
				break;

			case 'c':
				*cflag = 1;
				break;

			case '?':
				if (optopt == 'f') {
					fprintf(stderr, "Option -%c requires an argument.\n", optopt); 
				} else {
					fprintf(stderr, "Invalid option '-%c'.\n", optopt); // TODO: deal with non-printable characters
				}
				return 1;

			default:
				return 1;
		}	
	}

	if (!*fflag) {
		fprintf(stderr, "Option -f is required.\n");
		return 1;
	}	

	return 0;
}

state_t *read_file(char *filename, int cflag) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "Unable to find file '%s'.\n", filename);
		return NULL;
	}

	/* Setup our system state output */	
	state_t *state = (state_t *)malloc(sizeof(state_t)); 
	state->num_alloc_jobs = 8;
	state->num_alloc_files = 8;
	state->num_jobs = 0;
	state->num_files = 0;
	
	state->jobs = (job_t **)calloc(state->num_alloc_jobs, sizeof(job_t *));
	state->files = (file_t **)calloc(state->num_alloc_files, sizeof(file_t *));

	int name, file1, file2;
	while (fscanf(fp, "%d %d %d\n", &name, &file1, &file2) == 3) {
		/* Add the job */
		job_t *job = (job_t *)malloc(sizeof(job_t));
		job->name = name;
		job->file1 = file1;
		job->file2 = file2;
		state->jobs[state->num_jobs] = job; // TODO: make sure there is enough space
		state->num_jobs++;
		
		int file1_found = 0, file2_found = 0;
		for (int i = 0; i < state->num_files; i++) {
			if (file1_found && file2_found) {
				break;
			}
			if (!file1_found && state->files[i]->name == file1) {
				file1_found = 1; 
				state->files[i]->references++;
			}
			if (!file2_found && state->files[i]->name == file2) {
				file2_found = 1; 
				state->files[i]->references++;
			}
		}
		
		/* Add file1 */
		file_t *file;
		if (!file1_found) {
			file = (file_t *)malloc(sizeof(file_t));
			file->name = file1;
			file->references = 1;
			state->files[state->num_files] = file; // TODO: make sure there is enough space
			state->num_files++;
		}
		
		/* Add file2 */
		if (!file2_found) {
			file = (file_t *)malloc(sizeof(file_t));
			file->name = file2;
			file->references = 1;
			state->files[state->num_files] = file; // TODO: make sure there is enough space
			state->num_files++;
		}
	}

	fclose(fp);
	return state;
}
