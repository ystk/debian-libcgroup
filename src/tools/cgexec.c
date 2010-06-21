/*
 * Copyright RedHat Inc. 2008
 *
 * Authors:	Vivek Goyal <vgoyal@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <errno.h>
#include <grp.h>
#include <libcgroup.h>
#include <limits.h>
#include <pwd.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tools-common.h"

static struct option longopts[] = {
	{"sticky", no_argument, NULL, 's'}, 
	{0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
	int ret = 0, i;
	int cg_specified = 0;
	int flag_child = 0;
	uid_t uid;
	gid_t gid;
	pid_t pid;
	int c;
	struct cgroup_group_spec *cgroup_list[CG_HIER_MAX];

	if (argc < 2) {
		fprintf(stderr, "Usage is %s"
			" [-g <list of controllers>:<relative path to cgroup>]"
			" [--sticky] command [arguments]  \n",
			argv[0]);
		exit(2);
	}

	memset(cgroup_list, 0, sizeof(cgroup_list));

	while ((c = getopt_long(argc, argv, "+g:s", longopts, NULL)) > 0) {
		switch (c) {
		case 'g':
			ret = parse_cgroup_spec(cgroup_list, optarg,
					CG_HIER_MAX);
			if (ret) {
				fprintf(stderr, "cgroup controller and path"
						"parsing failed\n");
				return -1;
			}
			cg_specified = 1;
			break;
		case 's':
			flag_child |= CGROUP_DAEMON_UNCHANGE_CHILDREN;
			break;
		default:
			fprintf(stderr, "Invalid command line option\n");
			exit(1);
			break;
		}
	}

	/* Executable name */
	if (!argv[optind]) {
		fprintf(stderr, "No command specified\n");
		exit(1);
	}

	/* Initialize libcg */
	ret = cgroup_init();
	if (ret) {
		fprintf(stderr, "libcgroup initialization failed: %s\n",
			cgroup_strerror(ret));
		return ret;
	}

	uid = getuid();
	gid = getgid();
	pid = getpid();

	ret = cgroup_register_unchanged_process(pid, flag_child);
	if (ret) {
		fprintf(stderr, "registration of process failed\n");
		return ret;
	}

	/*
	 * 'cgexec' command file needs the root privilege for executing
	 * a cgroup_register_unchanged_process() by using unix domain
	 * socket, and an euid should be changed to the executing user
	 * from a root user.
	 */
	if (seteuid(uid)) {
		fprintf(stderr, "%s", strerror(errno));
		return -1;
	}
	if (cg_specified) {
		/*
		 * User has specified the list of control group and
		 * controllers
		 * */
		for (i = 0; i < CG_HIER_MAX; i++) {
			if (!cgroup_list[i])
				break;

			ret = cgroup_change_cgroup_path(cgroup_list[i]->path,
							pid,
                                                        (const char*const*) cgroup_list[i]->controllers);
			if (ret) {
				fprintf(stderr,
					"cgroup change of group failed\n");
				return ret;
			}
		}
	} else {

		/* Change the cgroup by determining the rules based on uid */
		ret = cgroup_change_cgroup_flags(uid, gid,
						 argv[optind], pid, 0);
		if (ret) {
			fprintf(stderr, "cgroup change of group failed\n");
			return ret;
		}
	}

	/* Now exec the new process */
	ret = execvp(argv[optind], &argv[optind]);
	if (ret == -1) {
		fprintf(stderr, "%s", strerror(errno));
		return -1;
	}
	return 0;
}
