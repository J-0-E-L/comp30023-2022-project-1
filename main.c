#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <search.h>

typedef struct file file_t;
typedef struct job job_t;

typedef struct file {
	int name, num_references;
	job_t *curr_lock, *next_lock;
} file_t;

typedef struct job {
	int name;
	file_t *file1, *file2;
} job_t;

typedef struct {
	/* Binary trees which store jobs and files */
	void *jobs, *files;
	int num_jobs, num_files;
} state_t;

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
state_t *read_file(char *filename, int cflag);

int file_cmp(const void *file1, const void *file2);
int job_cmp(const void *job1, const void *job2);

void printf_job(const void *job, VISIT value, int level);
void printf_file(const void *file, VISIT value, int level);

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
	
	/* Task 1*/
	printf("Processes %d\n", state->num_jobs);
	printf("Files %d\n", state->num_files);

	twalk(state->jobs, printf_job);
	twalk(state->files, printf_file);

	return 0;
}


int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag) {
	/* Get command line options - heavily based on the example given at: "https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html" */
	*fflag = 0, *eflag = 0, *cflag = 0;

	opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "f:e")) != -1) { switch (c) { 
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
	state->num_jobs = 0;
	state->num_files = 0;
	state->jobs = NULL;
	state->files = NULL;

	int jobs_alloced = 8;	
	job_t **jobs = (job_t **)calloc(jobs_alloced, sizeof(job_t *));

	int name, file1_name, file2_name;
	while (fscanf(fp, "%d %d %d\n", &name, &file1_name, &file2_name) == 3) {
		job_t *new_job;
		file_t *new_file, *file1, *file2;
		/* Add file1 */
		new_file = (file_t *)malloc(sizeof(file_t));
		new_file->name = file1_name;
		new_file->num_references = 1;
		new_file->curr_lock = NULL;
		new_file->next_lock = NULL;
		file1 = *(file_t **)tsearch((void *)new_file, &(state->files), file_cmp);
		if (file1 != new_file) {
			state->num_files++;
			file1->num_references++;
			free(new_file);
		} 

		/* Add file2 */
		new_file = (file_t *)malloc(sizeof(file_t));
		new_file->name = file2_name;
		new_file->num_references = 1;
		new_file->curr_lock = NULL;
		new_file->next_lock = NULL;
		file2 = *(file_t **)tsearch((void *)new_file, &(state->files), file_cmp);
		if (file2 != new_file) {
			state->num_files++;
			file2->num_references++;
			free(new_file);
		} 

		/* Add the job */
		new_job = (job_t *)malloc(sizeof(job_t));
		new_job->name = name;
		new_job->file1 = file1;
		new_job->file2 = file2;
	
		/* Increase the size of our job array if we have to */
		if (state->num_jobs >= jobs_alloced) {
			jobs_alloced *= 2;
			jobs = realloc(jobs, jobs_alloced * sizeof(job_t *));
		}
		jobs[state->num_jobs] = new_job;
		state->num_jobs++;

	}

	/* Add all jobs into output binary tree */
	for (int i = 0; i < state->num_jobs; i++) {
		tsearch(jobs[i], &(state->jobs), job_cmp);
	}
	
	free(jobs);
	fclose(fp);
	return state;
}

int file_cmp(const void *file1, const void *file2) {
	return ((file_t *)file1)->name - ((file_t *)file2)->name;
}

int job_cmp(const void *job1, const void *job2) {
	const job_t *_job1 = job1, *_job2 = job2;
	if (_job1->file1->num_references != _job2->file1->num_references) {
		return -(_job1->file1->num_references - _job2->file1->num_references);
	}
	return -(_job1->file2->num_references - _job2->file2->num_references);
}

void printf_job(const void *job, VISIT value, int level) {
	job_t *_job = *(job_t **)job;
	if (value == leaf || value == postorder) {
		printf("%d %d %d\n", _job->name, _job->file1->name, _job->file2->name);
	}
}

void printf_file(const void *file, VISIT value, int level) {
	file_t *_file = *(file_t **)file;
	if (value == leaf || value == postorder) {
		printf("%d %d\n", _file->name, _file->num_references);
	}
}

