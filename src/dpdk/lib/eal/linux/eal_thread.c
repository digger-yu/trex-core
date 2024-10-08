/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#include <rte_eal.h>
#include <rte_lcore.h>
#include <rte_log.h>
#include <rte_string_fns.h>

#include "eal_private.h"

/* require calling thread tid by gettid() */
int rte_sys_gettid(void)
{
	return (int)syscall(SYS_gettid);
}

void rte_thread_set_name(rte_thread_t thread_id, const char *thread_name)
{
	int ret = ENOSYS;
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 12)
	char truncated[RTE_THREAD_NAME_SIZE];
	const size_t truncatedsz = sizeof(truncated);

	if (strlcpy(truncated, thread_name, truncatedsz) >= truncatedsz)
		EAL_LOG(DEBUG, "Truncated thread name");

	ret = pthread_setname_np((pthread_t)thread_id.opaque_id, truncated);
#endif
#endif
	RTE_SET_USED(thread_id);
	RTE_SET_USED(thread_name);

	if (ret != 0)
		EAL_LOG(DEBUG, "Failed to set thread name");
}

#ifdef TREX_PATCH
int rte_thread_setname(pthread_t id, const char *name)
{
	int ret = ENOSYS;
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 12)
	char truncated[16];

	strlcpy(truncated, name, sizeof(truncated));
	ret = pthread_setname_np(id, truncated);
#endif
#endif
	RTE_SET_USED(id);
	RTE_SET_USED(name);
	return -ret;
}
#endif
